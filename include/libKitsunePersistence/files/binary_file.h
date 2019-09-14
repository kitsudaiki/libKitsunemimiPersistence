/**
 *  @file    binary_file.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief class for binary-file-handling
 *
 *  @detail This file contains the class for read and write of binary-files.
 *          It use the data-buffer of libKitsuneCommon as cache for all operations.
 *          The operations using posix-method with direct-flag to skip the page-chache
 *          of the linux-kernel. This makes operations with big block a bit faster
 *          because the data are less often copied. This has the result, that all
 *          read and write operations are synchronized.
 *          => segments to read from disc or write to disc should be as big as possible
 *             or else the latency makes the whole thing very very slow.
 *             The class should be run in an extra thread, with handle all operations and
 *             makes the whole sync asynchon again.
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

#include <buffering/data_buffer.h>

namespace fs=boost::filesystem;

namespace Kitsune
{
namespace Persistence
{

class BinaryFile
{
public:
    BinaryFile(const std::string filePath);
    ~BinaryFile();

    bool allocateStorage(const uint64_t numberOfBlocks,
                         const uint32_t blockSize = 4096);
    bool updateFileSize();

    bool readSegment(Common::DataBuffer* buffer,
                     const uint64_t startBlockInFile,
                     const uint64_t numberOfBlocks,
                     const uint64_t startBlockInBuffer = 0);
    bool writeSegment(Common::DataBuffer* buffer,
                      const uint64_t startBlockInFile,
                      const uint64_t numberOfBlocks,
                      const uint64_t startBlockInBuffer = 0);
    bool closeFile();

    // public variables to avoid stupid getter
    uint64_t m_totalFileSize = 0;
    std::string m_filePath = "";

private:
    int m_fileDescriptor = -1;

    void initFile();
};

} // namespace Persistence
} // namespace Kitsune

#endif // BINARY_FILE_H
