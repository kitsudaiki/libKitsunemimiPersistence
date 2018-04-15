/**
 *  @file    ioBuffer.h
 *  @author  Tobias Anker
 *
 *  @section DESCRIPTION
 *
 *  TODO: Description
 */

#ifndef RAMMEMORY_H
#define RAMMEMORY_H

#include <stdint.h>
#include <string.h>
#include <iostream>
#include <string>

namespace PerformanceIO
{
class StorageMemory;

class DataBuffer
{
public:
    DataBuffer(const std::string filePath = "");
    ~DataBuffer();

    bool initBuffer();
    bool closeBuffer(const bool withoutStorage = false);

    bool allocateBlocks(const uint32_t numberOfBlocks, const bool withoutStorage = false);

    uint32_t getNumberOfBlocks() const;
    uint32_t getBlockSize() const;

    void *getBufferPointer();
    void *getBlock(const uint32_t blockNumber);

    bool syncAll();
    bool syncBlocks(const uint32_t blockNumber,
                    const uint32_t endBlockNumber);


private:
    uint32_t m_size = 0;
    uint32_t m_storagePosition = 0;
    PerformanceIO::StorageMemory* m_storage = nullptr;

    void *m_buffer = nullptr;

    void* aligned_malloc(const uint32_t numberOfBytes);
    bool aligned_free(void *ptr);
};

}

#endif // RAMMEMORY_H
