#include <files/ioBuffer.h>
#include <files/storageMemory.h>

#define BLOCKSIZE 4096

namespace Persistence
{

/**
 * @brief IOBuffer::IOBuffer creates and initialize a buffer with a file
 * @param filePath path to the file with the data
 */
IOBuffer::IOBuffer(const QString filePath)
{
    m_storage = new Persistence::StorageMemory(filePath);

    initBuffer();
}

/**
 * @brief IOBuffer::~IOBuffer close the buffer at the end
 */
IOBuffer::~IOBuffer()
{
    closeBuffer();
}

/**
 * @brief IOBuffer::initBuffer
 * @return
 */
bool IOBuffer::initBuffer()
{
    assert(BLOCKSIZE % 512 == 0);

    // read number of bytes from the file
    quint32 readedSize = m_storage->getFileSize(true);

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

    return true;
}

/**
 * @brief IOBuffer::closeBuffer sync and close the buffer
 * @param withoutStorage true, to not close the file-connection (default: false)
 * @return true, if successful, else false
 */
bool IOBuffer::closeBuffer(const bool withoutStorage)
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
 * @brief IOBuffer::getNumberOfBlocks
 * @return number of current allocated blocks
 */
quint32 IOBuffer::getNumberOfBlocks() const
{
    return m_size;
}

/**
 * @brief IOBuffer::getBlockSize
 * @return
 */
quint32 IOBuffer::getBlockSize() const
{
    return BLOCKSIZE;
}

/**
 * @brief IOBuffer::getBufferPointer
 * @return
 */
void *IOBuffer::getBufferPointer()
{
    return m_buffer;
}

/**
 * @brief IOBuffer::getBlock
 * @param blockNumber
 * @return
 */
void* IOBuffer::getBlock(const quint32 blockNumber)
{
    if(blockNumber >= m_size) {
        return nullptr;
    }

    return m_buffer+(blockNumber * BLOCKSIZE);
}

/**
 * @brief IOBuffer::allocateBlocks allocate more memory for the buffer
 * @param numberOfBlocks number of blocks to allocate
 * @param withoutStorage true, if the file should not be resized (default: false)
 * @return true, if successful, else false
 */
bool IOBuffer::allocateBlocks(const quint32 numberOfBlocks, const bool withoutStorage)
{
    if(numberOfBlocks == 0) {
        return true;
    }

    // resize the file to the new size, if choosen
    if(withoutStorage == false) {
        if(m_storage->allocateMemory(numberOfBlocks * BLOCKSIZE) == false)
        {
            return false;
        }
    }

    // create the new buffer
    quint32 newSize = m_size + numberOfBlocks;
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
 * @brief IOBuffer::syncBlocks write a number of blocks from the buffer to the file
 * @param beginBlockNumber number of the first block to write
 * @param endBlockNumber number of the last block to write
 * @return true, if successful, else false
 */
bool IOBuffer::syncBlocks(const quint32 beginBlockNumber,
                          const quint32 endBlockNumber)
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
 * @brief IOBuffer::syncAll write the complete buffer to the file
 * @return true, if successful written
 */
bool IOBuffer::syncAll()
{
    return syncBlocks(0, m_size - 1);
}

/**
 * @brief IOBuffer::aligned_malloc allocate a number of aligned bytes
 * @param numberOfBytes bytes to allocate
 * @return pointer to the allocated memory
 */
void* IOBuffer::aligned_malloc(const quint32 numberOfBytes)
{
    void *mem = malloc(numberOfBytes+BLOCKSIZE+sizeof(void*));
    void **ptr = (void**)((uintptr_t)(mem+BLOCKSIZE+sizeof(void*)) & ~(BLOCKSIZE-1));
    ptr[-1] = mem;
    memset(ptr, 0, numberOfBytes);
    return ptr;
}

/**
 * @brief IOBuffer::aligned_free to free aligned memory
 * @param ptr pointer to the memory to free
 * @return true, if pointer not nullptr, else false
 */
bool IOBuffer::aligned_free(void *ptr)
{
    if(ptr != nullptr) {
        free(((void**)ptr)[-1]);
        ptr = nullptr;
        return true;
    }
    return false;
}

}
