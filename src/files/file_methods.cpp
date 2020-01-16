/**
 *  @file       file_methods.cpp
 *
 *  @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright  MIT License
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
isFile(const std::string filePath)
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
isDir(const std::string dirPath)
{
    if(boost::filesystem::exists(dirPath) == false
            || boost::filesystem::is_directory(dirPath) == false)
    {
        return false;
    }

    return true;
}

/**
 * @brief getParent
 * @param path
 * @return
 */
const std::string
getParent(const std::string &path)
{
    boost::filesystem::path pathObj(path);
    return pathObj.parent_path().string();
}

/**
 * @brief getFilesInDir
 * @param fileList
 * @param directory
 */
void
getFilesInDir(std::vector<std::string> &fileList,
              const boost::filesystem::path &directory,
              const bool withSubdirs,
              const std::vector<std::string> &exceptions)
{
    boost::filesystem::directory_iterator end_itr;
    for(boost::filesystem::directory_iterator itr(directory);
        itr != end_itr;
        ++itr)
    {
        if(is_directory(itr->path()))
        {
            if(withSubdirs == true)
            {
                if(exceptions.size() != 0)
                {
                    for(uint64_t i = 0; i < exceptions.size(); i++)
                    {
                        bool found = false;
                        if(itr->path().leaf().string() == exceptions.at(i)) {
                            found = true;
                        }

                        if(found == false) {
                            getFilesInDir(fileList, itr->path(), withSubdirs, exceptions);
                        }
                    }
                }
                else
                {
                    getFilesInDir(fileList, itr->path(), withSubdirs, exceptions);
                }
            }

        }
        else
        {
            fileList.push_back(itr->path().string());
        }
    }
}

/**
 * @brief listFiles
 * @param path
 * @param withSubdirs
 * @param exceptions
 * @return
 */
void
listFiles(std::vector<std::string> &fileList,
          const std::string &path,
          const bool withSubdirs,
          const std::vector<std::string> &exceptions)
{
    boost::filesystem::path pathObj(path);

    if(is_directory(pathObj))
    {
        getFilesInDir(fileList,
                      pathObj,
                      withSubdirs,
                      exceptions);
    }
    else
    {
        fileList.push_back(path);
    }
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
