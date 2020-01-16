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

const std::pair<bool, std::string> renameFileOrDir(const std::string oldPath,
                                                   const std::string newPath);
const std::pair<bool, std::string> copyPath(const std::string sourcePath,
                                            const std::string targetPath,
                                            const bool force=true);
const std::pair<bool, std::string> createDirectory(const std::string &path);
const std::pair<bool, std::string> deleteFileOrDir(const std::string &path);




}
}

#endif // FILE_METHODS_H
