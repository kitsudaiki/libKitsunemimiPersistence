/**
 *  @file    binary_file.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief class for binary-file-handling
 */

#include <libKitsunemimiPersistence/files/binary_file.h>

using Kitsunemimi::DataBuffer;

namespace Kitsunemimi
{
namespace Persistence
{

/**
 * @brief constructor
 *
 * @param filePath file-path of the binary-file
 * @param directIO true, to enable direct io, which allows faster data transfers without buffering
 *                 within the kernel, which requires, that each read and write call use a
 *                 block-size of a multiple of 512 bytes
 */
BinaryFile::BinaryFile(const std::string &filePath,
                       const bool directIO)
{
    m_filePath = filePath;
    m_directIO = directIO;

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
    if(m_directIO)
    {
        m_fileDescriptor = open(m_filePath.c_str(),
                                O_CREAT | O_DIRECT | O_RDWR | O_LARGEFILE,
                                0666);
        m_blockSize = 512;
    }
    else
    {
        m_fileDescriptor = open(m_filePath.c_str(),
                                O_CREAT | O_RDWR | O_LARGEFILE,
                                0666);
        m_blockSize = 1;
    }

    // check if file is open
    assert(m_fileDescriptor != -1);
    updateFileSize();
}

/**
 * @brief allocate new storage at the end of the file
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
            || blockSize % m_blockSize != 0)
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
 * @brief update size-information from the file
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
 * @brief read a complete binary file into a data-buffer object
 *
 * @param buffer reference to the buffer, where the data should be written into
 *
 * @return true, if successful, else false
 */
bool
BinaryFile::readCompleteFile(DataBuffer &buffer)
{
    if(m_directIO) {
        return false;
    }

    const long size = lseek(m_fileDescriptor, 0, SEEK_END);
    assert(size > 0);

    const uint64_t numberOfBlocks = (static_cast<uint64_t>(size) / buffer.blockSize) + 1;
    allocateBlocks_DataBuffer(buffer, numberOfBlocks);

    lseek(m_fileDescriptor, 0, SEEK_SET);
    const ssize_t ret = read(m_fileDescriptor, buffer.data, static_cast<uint64_t>(size));

    if(ret == -1)
    {
        // TODO: process errno
        return false;
    }

    buffer.bufferPosition = static_cast<uint64_t>(size);

    return true;
}

/**
 * @brief write a complete buffer into a binary-file
 *
 * @param buffer reference to the buffer with the data, which should be written into the file
 *
 * @return true, if successful, else false
 */
bool
BinaryFile::writeCompleteFile(DataBuffer &buffer)
{
    if(m_directIO) {
        return false;
    }

    bool success = allocateStorage(buffer.bufferPosition, 1);
    if(success == false) {
        return false;
    }

    lseek(m_fileDescriptor, 0, SEEK_SET);

    // write data to file
    ssize_t ret = write(m_fileDescriptor, buffer.data, buffer.bufferPosition);

    if(ret == -1)
    {
        // TODO: process errno
        return false;
    }
    return true;
}

/**
 * @brief read a readSegment of the file
 *
 * @return true, if successful, else false
 */
bool
BinaryFile::readSegment(DataBuffer &buffer,
                        const uint64_t startBlockInFile,
                        const uint64_t numberOfBlocks,
                        const uint64_t startBlockInBuffer)
{
    uint16_t blockSize = buffer.blockSize;
    if(m_directIO == false) {
        blockSize = 1;
    }

    const uint64_t numberOfBytes = numberOfBlocks * blockSize;
    const uint64_t startBytesInFile = startBlockInFile * blockSize;
    const uint64_t startBytesInBuffer = startBlockInBuffer * blockSize;

    // precheck
    if(numberOfBlocks == 0
            || startBytesInFile + numberOfBytes > m_totalFileSize
            || startBytesInBuffer + numberOfBytes > buffer.numberOfBlocks * buffer.blockSize
            || m_fileDescriptor < 0)
    {
        return false;
    }

    // go to the requested position and read the block
    lseek(m_fileDescriptor,
          static_cast<long>(startBytesInFile),
          SEEK_SET);
    ssize_t ret = read(m_fileDescriptor,
                       static_cast<uint8_t*>(buffer.data) + (startBytesInBuffer),
                       numberOfBytes);

    if(ret == -1)
    {
        // TODO: process errno
        return false;
    }
    return true;
}

/**
 * @brief write a block of the file
 *
 * @return true, if successful, else false
 */
bool
BinaryFile::writeSegment(DataBuffer &buffer,
                         const uint64_t startBlockInFile,
                         const uint64_t numberOfBlocks,
                         const uint64_t startBlockInBuffer)
{
    uint16_t blockSize = buffer.blockSize;
    if(m_directIO == false) {
        blockSize = 1;
    }

    const uint64_t numberOfBytes = numberOfBlocks * blockSize;
    const uint64_t startBytesInFile = startBlockInFile * blockSize;
    const uint64_t startBytesInBuffer = startBlockInBuffer * blockSize;

    // precheck
    if(numberOfBlocks == 0
            || startBytesInFile + numberOfBytes > m_totalFileSize
            || startBytesInBuffer + numberOfBytes > buffer.numberOfBlocks * buffer.blockSize
            || m_fileDescriptor < 0)
    {
        return false;
    }

    // go to the requested position and write the block
    long retSeek = lseek(m_fileDescriptor,
                         static_cast<long>(startBytesInFile),
                         SEEK_SET);
    assert(retSeek >= 0);

    // write data to file
    ssize_t ret = write(m_fileDescriptor,
                        static_cast<uint8_t*>(buffer.data) + startBytesInBuffer,
                        numberOfBytes);

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
 * @brief close the cluser-file
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
} // namespace Kitsunemimi
