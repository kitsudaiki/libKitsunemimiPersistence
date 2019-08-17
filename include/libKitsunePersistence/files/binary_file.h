/**
 *  @file    binary_file.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef BINARYFILE_H
#define BINARYFILE_H

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

#include <buffering/data_buffer.h>

namespace fs=boost::filesystem;

namespace Kitsune
{
namespace Persistence
{

class BinaryFile
{
public:
    BinaryFile(const std::string filePath,
               Common::DataBuffer* buffer);
    ~BinaryFile();

    bool allocateStorage(const uint64_t numberOfBlocks);
    bool updateFileSize();

    bool readSegment(const uint64_t startBlockInFile,
                     const uint64_t numberOfBlocks,
                     const uint64_t startBlockInBuffer = 0);
    bool writeSegment(const uint64_t startBlockInFile,
                      const uint64_t numberOfBlocks,
                      const uint64_t startBlockInBuffer = 0);
    bool closeFile();

    // public variables to avoid stupid getter
    const uint16_t m_blockSize = 4096;
    uint64_t m_numberOfBlocks = 0;
    uint64_t m_totalFileSize = 0;

private:
    std::string m_filePath = "";
    int m_fileDescriptor = -1;

    uint64_t m_blockPositionInFile = 0;

    Common::DataBuffer* m_buffer = nullptr;

    void initFile();
    bool checkMetaData();
};

} // namespace Persistence
} // namespace Kitsune

#endif // BINARYFILE_H
