/**
 *  @file    binaryFile.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef BINARYFILE_HPP
#define BINARYFILE_HPP

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

#include <buffering/commonDataBuffer.hpp>

namespace fs=boost::filesystem;

namespace Kitsune
{
namespace Persistence
{

class BinaryFile
{
public:
    BinaryFile(const std::string filePath,
               CommonDataBuffer* buffer);
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

    Kitsune::CommonDataBuffer* m_buffer = nullptr;

    void initFile();
    bool checkMetaData();
};

} // namespace Persistence
} // namespace Kitsune

#endif // BINARYFILE_HPP
