/**
 *  @file    ioBuffer.cpp
 *  @author  Tobias Anker
 *
 *  @section DESCRIPTION
 *
 *  TODO: Description
 */

#include <files/dataBuffer.h>
#include <files/storageMemory.h>

#define BLOCKSIZE 4096

namespace PerformanceIO
{

/**
 * @brief DataBuffer::IOBuffer creates and initialize a buffer with a file
 * @param filePath path to the file with the data
 */
DataBuffer::DataBuffer(const std::string filePath)
{
    if(filePath != "") {
        m_storage = new PerformanceIO::StorageMemory(filePath);
    }

    initBuffer();
}

/**
 * @brief DataBuffer::~IOBuffer close the buffer at the end
 */
DataBuffer::~DataBuffer()
{
    closeBuffer();
}

/**
 * @brief DataBuffer::initBuffer
 * @return
 */
bool DataBuffer::initBuffer()
{
    assert(BLOCKSIZE % 512 == 0);

    if(m_storage != nullptr)
    {
        // read number of bytes from the file
        uint32_t readedSize = m_storage->getFileSize(true);

        // check if file-size is valid
        assert(readedSize % BLOCKSIZE == 0);

        // init the buffer with the file-size or the the minimal size of a block
        if(readedSize != 0) {
            allocateBlocks(readedSize/BLOCKSIZE, true);
        }
        else {
            allocateBlocks(1);
        }

        // read data from the file to the buffer
        m_storage->readBlock(m_storagePosition, m_buffer, m_size * BLOCKSIZE);
    }
    else
    {
        allocateBlocks(1, true);
    }

    return true;
}

/**
 * @brief DataBuffer::closeBuffer sync and close the buffer
 * @param withoutStorage true, to not close the file-connection (default: false)
 * @return true, if successful, else false
 */
bool DataBuffer::closeBuffer(const bool withoutStorage)
{
    // sync at the end
    syncAll();

    // close the file if nessesary
    if(withoutStorage == false && m_storage != nullptr) {
        m_storage->closeFile();
        delete m_storage;
        m_storage = nullptr;
    }

    // deallocate the buffer
    if(aligned_free(m_buffer)) {
        m_buffer = nullptr;
        m_size = 0;
        return true;
    }
    return false;
}

/**
 * @brief DataBuffer::getNumberOfBlocks
 * @return number of current allocated blocks
 */
uint32_t DataBuffer::getNumberOfBlocks() const
{
    return m_size;
}

/**
 * @brief DataBuffer::getBlockSize
 * @return
 */
uint32_t DataBuffer::getBlockSize() const
{
    return BLOCKSIZE;
}

/**
 * @brief DataBuffer::getBufferPointer
 * @return
 */
void *DataBuffer::getBufferPointer()
{
    return m_buffer;
}

/**
 * @brief DataBuffer::getBlock
 * @param blockNumber
 * @return
 */
void* DataBuffer::getBlock(const uint32_t blockNumber)
{
    if(blockNumber >= m_size) {
        return nullptr;
    }

    return m_buffer+(blockNumber * BLOCKSIZE);
}

/**
 * @brief DataBuffer::allocateBlocks allocate more memory for the buffer
 * @param numberOfBlocks number of blocks to allocate
 * @param withoutStorage true, if the file should not be resized (default: false)
 * @return true, if successful, else false
 */
bool DataBuffer::allocateBlocks(const uint32_t numberOfBlocks, const bool withoutStorage)
{
    if(numberOfBlocks == 0) {
        return true;
    }

    // resize the file to the new size, if choosen
    if(withoutStorage == false && m_storage != nullptr) {
        if(m_storage->allocateMemory(numberOfBlocks * BLOCKSIZE) == false)
        {
            return false;
        }
    }

    // create the new buffer
    uint32_t newSize = m_size + numberOfBlocks;
    void* newBuffer = aligned_malloc(newSize * BLOCKSIZE);
    memset(newBuffer, 0, newSize * BLOCKSIZE);

    // copy the content of the old buffer to the new and deallocate the old
    if(m_buffer != nullptr) {
        memcpy(newBuffer, m_buffer, m_size * BLOCKSIZE);
        aligned_free(m_buffer);
    }

    // set the new values
    m_size = newSize;
    m_buffer = newBuffer;

    return true;
}

/**
 * @brief DataBuffer::syncBlocks write a number of blocks from the buffer to the file
 * @param beginBlockNumber number of the first block to write
 * @param endBlockNumber number of the last block to write
 * @return true, if successful, else false
 */
bool DataBuffer::syncBlocks(const uint32_t beginBlockNumber,
                          const uint32_t endBlockNumber)
{
    if(m_storage == nullptr) {
        return false;
    }

    // check the borders
    if(endBlockNumber >= m_size || beginBlockNumber >= endBlockNumber) {
        return false;
    }

    // write the buffer to the file
    return m_storage->writeBlock(beginBlockNumber * BLOCKSIZE,
                                 m_buffer,
                                 (endBlockNumber - beginBlockNumber) * BLOCKSIZE,
                                 beginBlockNumber * BLOCKSIZE);
}

/**
 * @brief DataBuffer::syncAll write the complete buffer to the file
 * @return true, if successful written
 */
bool DataBuffer::syncAll()
{
    return syncBlocks(0, m_size - 1);
}

/**
 * @brief DataBuffer::aligned_malloc allocate a number of aligned bytes
 * @param numberOfBytes bytes to allocate
 * @return pointer to the allocated memory
 */
void* DataBuffer::aligned_malloc(const uint32_t numberOfBytes)
{
    void *mem = malloc(numberOfBytes+BLOCKSIZE+sizeof(void*));
    void **ptr = (void**)((uintptr_t)(mem+BLOCKSIZE+sizeof(void*)) & ~(BLOCKSIZE-1));
    ptr[-1] = mem;
    memset(ptr, 0, numberOfBytes);
    return ptr;
}

/**
 * @brief DataBuffer::aligned_free to free aligned memory
 * @param ptr pointer to the memory to free
 * @return true, if pointer not nullptr, else false
 */
bool DataBuffer::aligned_free(void *ptr)
{
    if(ptr != nullptr) {
        free(((void**)ptr)[-1]);
        ptr = nullptr;
        return true;
    }
    return false;
}

}
