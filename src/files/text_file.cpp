/**
 *  @file    text_file.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief data-items for universal data-structures
 *
 *  @detail different methods for simple text-file-operations
 */

#include <libKitsunemimiPersistence/files/text_file.h>

#include <boost/filesystem.hpp>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

namespace fs=boost::filesystem;

namespace Kitsunemimi
{
namespace Persistence
{

/**
 * read text from a text-file
 *
 * @param filePath path the to file
 *
 * @return pair of bool and string
 *         success: first element is true and the second contains the content of the file
 *         fail: first element is false and the second contains the error-message
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
 * @return pair of bool and string
 *         success: first element is true and the second contains an empty string
 *         fail: first element is false and the second contains the error-message
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
    outputFile.flush();
    outputFile.close();

    return std::pair<bool, std::string>(true, "");
}

/**
 * append text to a existing text-file
 *
 * @param filePath path the to file
 * @param newText text which should be append to the file
 *
 * @return pair of bool and string
 *         success: first element is true and the second contains an empty string
 *         fail: first element is false and the second contains the error-message
 */
std::pair<bool, std::string>
appendText(const std::string &filePath,
           const std::string &newText)
{
    fs::path rootPathObj(filePath);

    // check for directory
    if(fs::exists(rootPathObj)
            && fs::is_regular_file(rootPathObj) == false)
    {
        std::string errorMessage = "Failed to append text to file \""
                                   + filePath +
                                   "\", because it is not a regular file.";
        return std::pair<bool, std::string>(false, errorMessage);
    }

    // open, write and close file again
    std::ofstream outputFile;
    outputFile.open(filePath, std::ios_base::app);
    outputFile << newText;
    outputFile.flush();
    outputFile.close();

    return std::pair<bool, std::string>(true, "");
}

/**
 * replace a specific line inside a text-file
 *
 * @param filePath path the to file
 * @param lineNumber number of the line inside the file, which should be replaced (beginning with 0)
 * @param newLineContent the new content string for the line, which should be replaced
 *
 * @return pair of bool and string
 *         success: first element is true and the second contains an empty string
 *         fail: first element is false and the second contains the error-message
 */
std::pair<bool, std::string>
replaceLine(const std::string &filePath,
            const uint32_t lineNumber,
            const std::string newLineContent)
{
    // read file
    std::pair<bool, std::string> result = readFile(filePath);
    if(result.first == false) {
        return result;
    }

    // split content into a vector of lines
    std::vector<std::string> splitedContent;
    splitStringByDelimiter(splitedContent, result.second, '\n');
    if(splitedContent.size() <= lineNumber)
    {
        std::string errorMessage = "failed to replace line in file \""
                                   + filePath +
                                   "\", because linenumber is too big for the file";
        return std::pair<bool, std::string>(false, errorMessage);
    }

    // build new file-content
    splitedContent[lineNumber] = newLineContent;
    std::string newFileContent = "";
    for(uint64_t i = 0; i < splitedContent.size(); i++)
    {
        if(i != 0) {
           newFileContent.append("\n");
        }
        newFileContent.append(splitedContent.at(i));
    }

    // write file back the new content
    result = writeFile(filePath, newFileContent, true);

    return result;
}

/**
 * replace a substring inside the file with another string
 *
 * @param filePath path the to file
 * @param oldContent substring which should be replaced
 * @param newContent new string for the replacement
 *
 * @return pair of bool and string
 *         success: first element is true and the second contains an empty string
 *         fail: first element is false and the second contains the error-message
 */
std::pair<bool, std::string>
replaceContent(const std::string &filePath,
               const std::string oldContent,
               const std::string newContent)
{
    // read file
    std::pair<bool, std::string> result = readFile(filePath);
    if(result.first == false) {
        return result;
    }

    // replace content
    std::string::size_type pos = 0u;
    while((pos = result.second.find(oldContent, pos)) != std::string::npos)
    {
        result.second.replace(pos, oldContent.length(), newContent);
        pos += newContent.length();
    }

    // write file back the new content
    result = writeFile(filePath, result.second, true);

    return result;
}

} // namespace Persistence
} // namespace Kitsunemimi
