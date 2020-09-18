/**
 *  @file    binary_file.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief class for binary-file-handling
 */

#ifndef BINARY_FILE_H
#define BINARY_FILE_H

#include <deque>
#include <sstream>
#include <mutex>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <boost/filesystem.hpp>

#include <libKitsunemimiCommon/buffer/data_buffer.h>

namespace fs=boost::filesystem;

namespace Kitsunemimi
{
namespace Persistence
{

class BinaryFile
{
public:
    BinaryFile(const std::string &filePath,
               const bool directIO = false);
    ~BinaryFile();

    bool allocateStorage(const uint64_t numberOfBlocks,
                         const uint32_t blockSize = 4096);
    bool updateFileSize();

    bool readCompleteFile(DataBuffer &buffer);
    bool writeCompleteFile(DataBuffer &buffer);

    bool readSegment(DataBuffer &buffer,
                     const uint64_t startBlockInFile,
                     const uint64_t numberOfBlocks,
                     const uint64_t startBlockInBuffer = 0);
    bool writeSegment(DataBuffer &buffer,
                      const uint64_t startBlockInFile,
                      const uint64_t numberOfBlocks,
                      const uint64_t startBlockInBuffer = 0);
    bool closeFile();

    // public variables to avoid stupid getter
    uint64_t m_totalFileSize = 0;
    std::string m_filePath = "";

private:
    int m_fileDescriptor = -1;
    bool m_directIO = true;
    uint16_t m_blockSize = 512;

    bool initFile();
};

} // namespace Persistence
} // namespace Kitsunemimi

#endif // BINARY_FILE_H
