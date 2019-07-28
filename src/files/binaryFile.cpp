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
StorageMemory::StorageMemory(const std::string filePath,
                             Kitsune::CommonDataBuffer* buffer)
{
    m_filePath = filePath;
    m_buffer = buffer;
    initFile();
}

/**
 * @brief StorageMemory::~StorageMemory
 */
StorageMemory::~StorageMemory()
{
    closeFile();
}

/**
 * @brief create a new file or open an existing file
 */
void
StorageMemory::initFile()
{
    m_fileDescriptor = open(m_filePath.c_str(), O_CREAT | O_DIRECT | O_RDWR | O_LARGEFILE, 0666);
    // check if file is open
    assert(m_fileDescriptor != -1);
    getFileSize();
}

/**
 * @brief StorageMemory::checkData
 * @return
 */
bool
StorageMemory::checkMetaData()
{
    if(m_buffer->data == nullptr
            || m_fileDescriptor == -1)
    {
        return false;
    }
    return true;
}

/**
 * @brief allocate new storage at the end of the file
 *
 * @return true is successful, else false
 */
bool
StorageMemory::allocateStorage(const uint64_t numberOfBlocks)
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
                               m_totalFileSize,
                               numberOfBlocks * m_blockSize);

    // check if allocation was successful
    if(ret != 0)
    {
        // TODO: process errno
        return false;
    }

    // got the the end of the file
    ret = lseek(m_fileDescriptor, 0, SEEK_END);
    if(ret >= 0) {
        m_totalFileSize = ret;
    }

    return true;
}

/**
 * @brief StorageMemory::getFileSize get the size of the file
 *
 * @return size of the file
 */
uint64_t
StorageMemory::getFileSize(const bool makeCheck)
{
    if(m_totalFileSize == 0
            || makeCheck)
    {
        // check if filesize is really 0 or check is requested
        m_totalFileSize = lseek(m_fileDescriptor, 0, SEEK_END);
        lseek(m_fileDescriptor, 0, SEEK_SET);
    }
    return m_totalFileSize;
}

/**
 * read a readSegment of the file
 *
 * @return true, if successful, else false
 */
bool
StorageMemory::readSegment(const uint64_t startBlockInFile,
                           const uint64_t numberOfBlocks,
                           const uint64_t startBlockInBuffer)
{
    // precheck
    if(numberOfBlocks != 0
            || m_fileDescriptor == -1
            || m_buffer->data == nullptr)
    {
        return false;
    }

    // check if requested block is in range of file
    if(startBlockInFile + numberOfBlocks > m_totalFileSize) {
       return false;
    }

    // go to the requested position and read the block
    lseek(m_fileDescriptor, startBlockInFile, SEEK_SET);
    ssize_t ret = read(m_fileDescriptor, m_buffer->data, numberOfBlocks);

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
bool StorageMemory::writeSegment(const uint64_t startBlockInFile,
                                 const uint64_t numberOfBlocks,
                                 const uint64_t startBlockInBuffer)
{
    // precheck
    if(numberOfBlocks != 0
            || m_fileDescriptor == -1
            || m_buffer->data == nullptr)
    {
        return false;
    }

    // check if requested block is in range of file
    if(startBlockInFile + numberOfBlocks > m_totalFileSize) {
       return false;
    }

    // go to the requested position and write the block
    lseek(m_fileDescriptor, startBlockInFile, SEEK_SET);
    ssize_t ret = write(m_fileDescriptor, m_buffer->data, numberOfBlocks);

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
 * @brief StorageMemory::closeFile close the cluser-file
 * @return true, if file-descriptor is valid, else false
 */
bool StorageMemory::closeFile()
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

}
}
