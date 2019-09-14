/**
 *  @file    binary_file.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief class for binary-file-handling
 *
 *  @detail This file contains the class for read and write of binary-files.
 *          It use the data-buffer of libKitsuneCommon as cache for all operations.
 *          The operations using posix-method with direct-flag to skip the page-chache
 *          of the linux-kernel. This makes operations with big block a bit faster
 *          because the data are less often copied. This has the result, that all
 *          read and write operations are synchronized.
 *          => segments to read from disc or write to disc should be as big as possible
 *             or else the latency makes the whole thing very very slow.
 *             The class should be run in an extra thread, with handle all operations and
 *             makes the whole sync asynchon again.
 */

#include <files/binary_file.h>

using Kitsune::Common::DataBuffer;

namespace Kitsune
{
namespace Persistence
{

/**
 * @brief StorageMemory::StorageMemory
 * @param filePath
 * @param buffer
 */
BinaryFile::BinaryFile(const std::string filePath)
{
    m_filePath = filePath;

    initFile();
}

/**
 * @brief destructor
 */
BinaryFile::~BinaryFile()
{
    closeFile();
}

/**
 * @brief create a new file or open an existing file
 */
void
BinaryFile::initFile()
{
    m_fileDescriptor = open(m_filePath.c_str(),
                            O_CREAT | O_DIRECT | O_RDWR | O_LARGEFILE,
                            0666);
    // check if file is open
    assert(m_fileDescriptor != -1);
    updateFileSize();
}

/**
 * allocate new storage at the end of the file
 *
 * @return true is successful, else false
 */
bool
BinaryFile::allocateStorage(const uint64_t numberOfBlocks,
                            const uint32_t blockSize)
{
    // precheck
    if(numberOfBlocks == 0
            || m_fileDescriptor < 0
            || blockSize % 512 != 0)
    {
        return false;
    }

    // set first to the start of the file and allocate the new size at the end of the file
    lseek(m_fileDescriptor, 0, SEEK_SET);
    long ret = posix_fallocate(m_fileDescriptor,
                               static_cast<long>(m_totalFileSize),
                               static_cast<long>(numberOfBlocks * blockSize));

    // check if allocation was successful
    if(ret != 0)
    {
        // TODO: process errno
        return false;
    }

    // got the the end of the file
    ret = lseek(m_fileDescriptor, 0, SEEK_END);
    if(ret >= 0)
    {
        m_totalFileSize = static_cast<uint64_t>(ret);
    }

    return true;
}

/**
 * update size-information from the file
 *
 * @return false, if file not open, else true
 */
bool
BinaryFile::updateFileSize()
{
    if(m_fileDescriptor == -1) {
        return false;
    }

    // check if filesize is really 0 or check is requested
    long ret = lseek(m_fileDescriptor, 0, SEEK_END);
    if(ret >= 0) {
        m_totalFileSize = static_cast<uint64_t>(ret);
    }

    lseek(m_fileDescriptor,
          0,
          SEEK_SET);

    return true;
}

/**
 * read a readSegment of the file
 *
 * @return true, if successful, else false
 */
bool
BinaryFile::readSegment(Common::DataBuffer* buffer,
                        const uint64_t startBlockInFile,
                        const uint64_t numberOfBlocks,
                        const uint64_t startBlockInBuffer)
{
    const uint16_t blockSize = buffer->blockSize;

    // precheck
    if(numberOfBlocks == 0
            || startBlockInFile + numberOfBlocks > m_totalFileSize / blockSize
            || startBlockInBuffer + numberOfBlocks > buffer->numberOfBlocks
            || m_fileDescriptor < 0)
    {
        return false;
    }

    // go to the requested position and read the block
    lseek(m_fileDescriptor,
          static_cast<long>(startBlockInFile * blockSize),
          SEEK_SET);
    ssize_t ret = read(m_fileDescriptor,
                       static_cast<uint8_t*>(buffer->data) + (startBlockInBuffer * blockSize),
                       numberOfBlocks * blockSize);

    if(ret == -1)
    {
        // TODO: process errno
        return false;
    }
    return true;
}

/**
 * write a block of the file
 *
 * @return true, if successful, else false
 */
bool
BinaryFile::writeSegment(Common::DataBuffer *buffer,
                         const uint64_t startBlockInFile,
                         const uint64_t numberOfBlocks,
                         const uint64_t startBlockInBuffer)
{
    const uint16_t blockSize = buffer->blockSize;

    // precheck
    if(numberOfBlocks == 0
            || startBlockInFile + numberOfBlocks > m_totalFileSize / blockSize
            || startBlockInBuffer + numberOfBlocks > buffer->numberOfBlocks
            || m_fileDescriptor < 0)
    {
        return false;
    }

    // go to the requested position and write the block
    long retSeek = lseek(m_fileDescriptor,
                         static_cast<long>(startBlockInFile * blockSize),
                         SEEK_SET);
    assert(retSeek >= 0);

    // write data to file
    ssize_t ret = write(m_fileDescriptor,
                        static_cast<uint8_t*>(buffer->data) + (startBlockInBuffer * blockSize),
                        numberOfBlocks * blockSize);

    if(ret == -1)
    {
        // TODO: process errno
        return false;
    }

    // sync file
    fdatasync(m_fileDescriptor);

    return true;
}

/**
 * close the cluser-file
 *
 * @return true, if file-descriptor is valid, else false
 */
bool
BinaryFile::closeFile()
{
    // check file-deskriptor
    if(m_fileDescriptor > 0)
    {
        close(m_fileDescriptor);
        m_fileDescriptor = -1;
        return true;
    }
    return false;
}

} // namespace Persistence
} // namespace Kitsune
