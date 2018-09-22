/**
 *  @file    storageMemory.cpp
 *  @author  Tobias Anker
 *
 *  @section DESCRIPTION
 *
 *  TODO: Description
 */

#include <files/storageMemory.h>

namespace Kitsune
{
namespace Persistence
{

/**
 * @brief StorageMemory::StorageMemory init the storage-class with an already existing file
 * @param filePath path to the existing file
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
 * @brief StorageMemory::~StorageMemory close the file
 */
StorageMemory::~StorageMemory()
{
    closeFile();
}

/**
 * @brief StorageMemory::initFile create a new file or open an existing file
 */
void StorageMemory::initFile()
{
    m_fileDescriptor = open(m_filePath.c_str(), O_CREAT | O_DIRECT | O_RDWR | O_LARGEFILE, 0666);
    // check if file is open
    assert(m_fileDescriptor != -1);
    getFileSize();
}

/**
 * @brief StorageMemory::allocateMemory allocate new storage at the end of the file
 * @param size size which show be additional be allocated
 * @return true is successful, else false
 */
bool StorageMemory::allocateMemory(const uint32_t size)
{
    if(size == 0) {
        return true;
    }

    // check if file is open and the new additional size is a multiple of 4096
    if(m_fileDescriptor == -1 || size % 4096 != 0) {
        return false;
    }

    // set first to the start of the file and allocate the new size at the end of the file
    lseek(m_fileDescriptor, 0, SEEK_SET);
    int ret = posix_fallocate(m_fileDescriptor, m_fileSize, size);

    // check if allocation was successful
    if(ret != 0) {
        // TODO: process errno
        return false;
    }

    // got the the end of the file
    m_fileSize = lseek(m_fileDescriptor, 0, SEEK_END);

    return true;
}

/**
 * @brief StorageMemory::getFileSize get the size of the file
 * @param makeCheck force a new check of the file-size
 * @return size of the file
 */
uint32_t StorageMemory::getFileSize(const bool makeCheck)
{
    if(m_fileSize == 0 || makeCheck) {
        // check if filesize is really 0 or check is requested
        m_fileSize = lseek(m_fileDescriptor, 0, SEEK_END);
        lseek(m_fileDescriptor, 0, SEEK_SET);
    }
    return m_fileSize;
}

/**
 * @brief StorageMemory::readBlock read a block of the file
 * @param storagePosition offset of the file in number of bytes
 * @param buffer pointer of the target-buffer
 * @param bufferSize number of bytes which should be read
 * @param bufferOffset offet inside of buffer in bytes
 * @return true, if successful, else false
 */
bool StorageMemory::readBlock(const uint32_t storagePosition,
                              void *buffer,
                              const uint32_t bufferSize,
                              const uint32_t bufferOffset)
{
    // check if blocksizes are compatible with direct read
    if(storagePosition % 512 != 0
            || storagePosition % 512 != 0
            || storagePosition % 512 != 0) {
        return false;
    }

    // check if file is open and buffer-pointer is valid
    if(m_fileDescriptor == -1  || buffer == nullptr) {
        return false;
    }

    // check if requested block is in range of file
    if(storagePosition + bufferSize > m_fileSize) {
       return false;
    }

    // go to the requested position and read the block
    lseek(m_fileDescriptor, storagePosition, SEEK_SET);
    ssize_t ret = read(m_fileDescriptor, buffer + bufferOffset, bufferSize);

    if(ret == -1) {
        // TODO: process errno
        return false;
    }
    return true;
}

/**
 * @brief StorageMemory::writeBlock write a block of the file
 * @param storagePosition offset of the file in number of bytes
 * @param buffer pointer of the source-buffer
 * @param bufferSize number of bytes which should be write
 * @param bufferOffset offet inside of buffer in bytes
 * @return true, if successful, else false
 */
bool StorageMemory::writeBlock(const uint32_t storagePosition,
                               void *buffer,
                               const uint32_t bufferSize,
                               const uint32_t bufferOffset)
{
    // check if blocksizes are compatible with direct write
    if(storagePosition % 512 != 0
            || bufferSize % 512 != 0
            || bufferOffset % 512 != 0) {
        return false;
    }

    // check if file is open and buffer-pointer is valid
    if(m_fileDescriptor == -1  || buffer == nullptr) {
        return false;
    }

    // check if requested block is in range of file
    if(storagePosition + bufferSize > m_fileSize) {
       return false;
    }

    // go to the requested position and write the block
    lseek(m_fileDescriptor, storagePosition, SEEK_SET);
    ssize_t ret = write(m_fileDescriptor, buffer + bufferOffset, bufferSize);

    if(ret == -1) {
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
    if(m_fileDescriptor > 0) {
        close(m_fileDescriptor);
        m_fileDescriptor = -1;
        return true;
    }
    return false;
}

}
}
