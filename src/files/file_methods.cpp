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
 * @brief check if a path exist
 *
 * @param path path to check
 *
 * @return true, if path exist, else false
 */
bool
doesPathExist(const std::string path)
{
    return boost::filesystem::exists(path);
}

/**
 * @brief check if a path exist and is a directory
 *
 * @param filePath path to check
 *
 * @return true, if path exist and is a file, else false
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
 * @brief check if a path exist and is a directory
 *
 * @param dirPath path to check
 *
 * @return true, if path exist and is a directory, else false
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
 * @brief get parent-path of a path
 *
 * @param path original path
 *
 * @return parent-path, if path exist, else empty string
 */
const std::string
getParent(const std::string &path)
{
    if(boost::filesystem::exists(path) == false) {
        return std::string("");
    }

    boost::filesystem::path pathObj(path);
    return pathObj.parent_path().string();
}

/**
 * @brief iterate over a directory and subdirectory to file all containing files
 *
 * @param fileList resulting string-list with the absolute path of all found files
 * @param directory directory-path where to search
 * @param withSubdirs false, to list only files in the current directory, but not files from
 *                    subdirectories
 * @param exceptions list with directory-names, which should be skipped
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
 * @brief iterate over a directory and subdirectory to file all containing files
 *
 * @param fileList resulting string-list with the absolute path of all found files
 * @param path path where to search. This should be a directory. If this is a file-path, this path
 *             is the only one in the resulting list
 * @param withSubdirs false, to list only files in the current directory, but not files from
 *                    subdirectories (Default: true)
 * @param exceptions list with directory-names, which should be skipped (Default: empty list)
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
 * @brief rename a file or directory
 *
 * @param oldPath origial path
 * @param newPath new path after renaming
 *
 * @return pair of boolean and string
 *         success: first true and second empty string
 *         failed: first faile and second the error-message
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
 * @brief copy a file or directory
 *
 * @param sourcePath origial path
 * @param targetPath path of the copy
 *
 * @return pair of boolean and string
 *         success: first true and second empty string
 *         failed: first faile and second the error-message
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
 * @brief create a directory
 *
 * @param path path to create
 *
 * @return pair of boolean and string
 *         success: first true and second empty string
 *         failed: first faile and second the error-message
 */
const std::pair<bool, std::string>
createDirectory(const std::string &path)
{
    std::pair<bool, std::string> result;

    boost::system::error_code error;
    error.clear();

    result.first = boost::filesystem::create_directories(path, error);

    if(result.first == false)
    {
        result.second = error.message();
        return result;
    }

    return result;
}

/**
 * @brief delete a path
 *
 * @param path path to delete
 *
 * @return pair of boolean and string
 *         success: first true and second empty string
 *         failed: first faile and second the error-message
 */
const std::pair<bool, std::string>
deleteFileOrDir(const std::string &path)
{
    std::pair<bool, std::string> result;

    boost::system::error_code error;
    error.clear();

    result.first = boost::filesystem::remove_all(path, error);

    if(result.first == false)
    {
        result.second = error.message();
        return result;
    }

    return result;
}

}
}
