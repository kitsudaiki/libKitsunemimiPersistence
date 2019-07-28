#include "textFile.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <fstream>
#include <boost/filesystem.hpp>

namespace fs=boost::filesystem;

namespace Kitsune
{
namespace Persistence
{

/**
 * @brief readFile
 * @param filePath
 * @return
 */
std::pair<bool, std::string>
readFile(const std::string &filePath)
{
    fs::path rootPathObj(filePath);

    // check if exist
    if(fs::exists(rootPathObj))
    {
        // check for directory
        if(fs::is_directory(rootPathObj))
        {
            std::string errorMessage = "failed to read destination of path \""
                                       + filePath +
                                       "\", because it already exist and it is a directory, "
                                       "but must be a file or not existing";
            return std::pair<bool, std::string>(false, errorMessage);
        }
    }
    else
    {
        std::string errorMessage = "destination of path \"" + filePath + "\", doesn't exist";
        return std::pair<bool, std::string>(false, errorMessage);
    }

    std::ifstream inFile;
    inFile.open(filePath);

    std::stringstream strStream;
    strStream << inFile.rdbuf();
    std::string fileContent = strStream.str();

    inFile.close();

    return std::pair<bool, std::string>(true, fileContent);
}

/**
 * write text into a file
 *
 * @param filePath path the to file
 * @param content text which be wirtten into the file
 * @param force if true, it overwrites the file, if there already exist one
 *
 * @return pair of bool and string, where the first is true, if write was successful or false
 *         if write into file failed. If Failed the second entry contains the error-message
 */
std::pair<bool, std::string>
writeFile(const std::string &filePath,
          const std::string &content,
          const bool force)
{
    fs::path rootPathObj(filePath);

    // check if exist
    if(fs::exists(rootPathObj))
    {
        // check for directory
        if(fs::is_directory(rootPathObj))
        {
            std::string errorMessage = "failed to write destination of path \""
                                       + filePath +
                                       "\", because it already exist and it is a directory, "
                                       "but must be a file or not existing";
            return std::pair<bool, std::string>(false, errorMessage);
        }

        // check for override
        if(force == false)
        {
            std::string errorMessage = "failed to write destination of path \""
                                       + filePath +
                                       "\", because it already exist, but should not be overwrite";
            return std::pair<bool, std::string>(false, errorMessage);
        }

        // TODO: check access-rights
        // fs::path p(argv[1]);
        // fs::file_status s = status(p);
        // printf("%o\n",s.permissions());

        // remove file
        fs::remove(rootPathObj);
    }

    // create new file and write content
    std::ofstream outputFile;
    outputFile.open(filePath);
    outputFile << content;
    outputFile.close();

    return std::pair<bool, std::string>(true, "");
}

}
}
