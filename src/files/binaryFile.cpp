/**
 *  @file    binaryFile.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include <files/binaryFile.hpp>

namespace Kitsune
{
namespace Persistence
{

/**
 * @brief StorageMemory::StorageMemory
 * @param filePath
 * @param buffer
 */
BinaryFile::BinaryFile(const std::string filePath,
                       CommonDataBuffer* buffer)
{
    m_filePath = filePath;
    m_buffer = buffer;
    initFile();
}

/**
 * @brief StorageMemory::~StorageMemory
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
 * @brief StorageMemory::checkData
 * @return
 */
bool
BinaryFile::checkMetaData()
{
    if(m_buffer->data == nullptr
            || m_fileDescriptor == -1)
    {
        return false;
    }
    return true;
}

/**
 * allocate new storage at the end of the file
 *
 * @return true is successful, else false
 */
bool
BinaryFile::allocateStorage(const uint64_t numberOfBlocks)
{
    // precheck
    if(numberOfBlocks == 0
            || checkMetaData() == false)
    {
        return false;
    }

    // set first to the start of the file and allocate the new size at the end of the file
    lseek(m_fileDescriptor, 0, SEEK_SET);
    long ret = posix_fallocate(m_fileDescriptor,
                               static_cast<long>(m_totalFileSize),
                               static_cast<long>(numberOfBlocks * m_blockSize));

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
        m_numberOfBlocks += numberOfBlocks;
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
BinaryFile::readSegment(const uint64_t startBlockInFile,
                        const uint64_t numberOfBlocks,
                        const uint64_t startBlockInBuffer)
{
    // precheck
    if(numberOfBlocks != 0
            || checkMetaData() == false)
    {
        return false;
    }

    // check if requested block is in range of file
    if(startBlockInFile + numberOfBlocks > m_totalFileSize) {
       return false;
    }

    // go to the requested position and read the block
    lseek(m_fileDescriptor,
          static_cast<long>(startBlockInFile),
          SEEK_SET);
    ssize_t ret = read(m_fileDescriptor,
                       m_buffer->data,
                       numberOfBlocks);

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
BinaryFile::writeSegment(const uint64_t startBlockInFile,
                         const uint64_t numberOfBlocks,
                         const uint64_t startBlockInBuffer)
{
    // precheck
    if(numberOfBlocks != 0
            || startBlockInFile + numberOfBlocks > m_numberOfBlocks
            || startBlockInBuffer + numberOfBlocks > m_buffer->numberOfBlocks
            || checkMetaData() == false)
    {
        return false;
    }

    // go to the requested position and write the block
    lseek(m_fileDescriptor,
          static_cast<long>(startBlockInFile),
          SEEK_SET);
    ssize_t ret = write(m_fileDescriptor,
                        static_cast<uint8_t*>(m_buffer->data) + startBlockInBuffer,
                        numberOfBlocks);

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
