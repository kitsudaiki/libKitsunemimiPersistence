/**
 *  @file       file_methods.h
 *
 *  @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright  MIT License
 */

#ifndef FILE_METHODS_H
#define FILE_METHODS_H

#include <sstream>
#include <assert.h>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

namespace Kitsunemimi
{
namespace Persistence
{

void listFiles(std::vector<std::string> &fileList,
               const std::string &path,
               const bool withSubdirs=true,
               const std::vector<std::string> &exceptions = {});

bool renameFileOrDir(const bfs::path &oldPath,
                     const bfs::path &newPath,
                     std::string &errorMessage);
bool copyPath(const bfs::path &sourcePath,
              const bfs::path &targetPath,
              std::string &errorMessage,
              const bool force=true);
bool createDirectory(const bfs::path &path,
                     std::string &errorMessage);
bool deleteFileOrDir(const bfs::path &path,
                     std::string &errorMessage);




}
}

#endif // FILE_METHODS_H
