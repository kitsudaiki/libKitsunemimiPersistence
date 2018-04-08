/**
 *  @file    storageMemory.h
 *  @author  Tobias Anker
 *
 *  @section DESCRIPTION
 *
 *  TODO: Description
 */

#ifndef SSDMEMORY_H
#define SSDMEMORY_H

#include <deque>
#include <sstream>
#include <mutex>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

namespace PerformanceIO
{

class StorageMemory
{
public:
    StorageMemory(const std::string filePath);
    ~StorageMemory();

    bool allocateMemory(const uint32_t size);
    uint32_t getFileSize(const bool makeCheck = false);

    bool readBlock(const uint32_t storagePosition,
                   void *buffer,
                   const uint32_t bufferSize,
                   const uint32_t bufferOffset = 0);
    bool writeBlock(const uint32_t storagePosition,
                    void *buffer,
                    const uint32_t bufferSize,
                    const uint32_t bufferOffset = 0);
    bool closeFile();

private:
    std::string m_filePath = "";
    uint32_t m_fileSize = 0;
    int m_fileDescriptor = -1;

    void initFile();
};

}

#endif // SSDMEMORY_H
