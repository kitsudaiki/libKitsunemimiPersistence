/**
 * @file        file_methods.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiPersistence/files/file_methods.h>

namespace Kitsunemimi
{
namespace Persistence
{

/**
 * @brief doesPathExist
 * @param path
 * @return
 */
bool
doesPathExist(const std::string path)
{
    return boost::filesystem::exists(path);
}

/**
 * @brief doesFileExist
 * @param filePath
 * @return
 */
bool
doesFileExist(const std::string filePath)
{
    if(boost::filesystem::exists(filePath) == false
            || boost::filesystem::is_directory(filePath))
    {
        return false;
    }

    return true;
}

/**
 * @brief doesDirExist
 * @param dirPath
 * @return
 */
bool
doesDirExist(const std::string dirPath)
{
    if(boost::filesystem::exists(dirPath) == false
            || boost::filesystem::is_directory(dirPath) == false)
    {
        return false;
    }

    return true;
}

/**
 * @brief renameFileOrDir
 * @param oldPath
 * @param newPath
 * @return
 */
const std::pair<bool, std::string>
renameFileOrDir(const std::string oldPath,
                const std::string newPath)
{
    std::pair<bool, std::string> result;

    boost::system::error_code error;
    error.clear();

    boost::filesystem::rename(oldPath, newPath, error);

    if(error.value() != 0)
    {
        result.first = false;
        result.second = error.message();
        return result;
    }

    result.first = true;

    return result;
}

/**
 * @brief copyPath
 * @param sourcePath
 * @param targetPath
 * @return
 */
const std::pair<bool, std::string>
copyPath(const std::string sourcePath,
         const std::string targetPath,
         const bool force)
{
    std::pair<bool, std::string> result;

    boost::system::error_code error;
    error.clear();

    if(force) {
        boost::filesystem::remove(targetPath);
    }
    boost::filesystem::copy(sourcePath, targetPath, error);

    if(error.value() != 0)
    {
        result.first = false;
        result.second = error.message();
        return result;
    }

    result.first = true;

    return result;
}

/**
 * @brief deleteFileOrDis
 * @param path
 * @return
 */
bool
deleteFileOrDir(const std::string path)
{
    return boost::filesystem::remove(path);
}

}
}
