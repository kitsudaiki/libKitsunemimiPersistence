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
bool doesFileExist(const std::string filePath);
bool doesDirExist(const std::string dirPath);
const std::pair<bool, std::string> renameFileOrDir(const std::string oldPath,
                                                   const std::string newPath);
const std::pair<bool, std::string> copyPath(const std::string sourcePath,
                                            const std::string targetPath,
                                            const bool force=true);
bool deleteFileOrDir(const std::string path);

}
}

#endif // FILE_METHODS_H
