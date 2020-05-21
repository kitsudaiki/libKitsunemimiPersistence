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

namespace Kitsunemimi
{
namespace Persistence
{

bool doesPathExist(const std::string path);
bool isFile(const std::string filePath);
bool isDir(const std::string dirPath);

const std::string getParent(const std::string &path);
void listFiles(std::vector<std::string> &fileList,
               const std::string &path,
               const bool withSubdirs=true,
               const std::vector<std::string> &exceptions = {});
const std::string getRelativePath(const std::string &childPath,
                              const std::string &parentPath);

bool renameFileOrDir(const std::string &oldPath,
                     const std::string &newPath,
                     std::string &errorMessage);
bool copyPath(const std::string &sourcePath,
              const std::string &targetPath,
              std::string &errorMessage,
              const bool force=true);
bool createDirectory(const std::string &path,
                     std::string &errorMessage);
bool deleteFileOrDir(const std::string &path,
                     std::string &errorMessage);




}
}

#endif // FILE_METHODS_H
