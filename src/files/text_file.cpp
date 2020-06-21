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
#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiPersistence/files/file_methods.h>

namespace Kitsunemimi
{
namespace Persistence
{

/**
 * read text from a text-file
 *
 * @param readContentconst reference to the variable, where the content should be written into
 * @param filePath path the to file
 * @param errorMessage reference for error-message output
 *
 * @return true if successful, else false
 */
bool
readFile(std::string &readContentconst,
         const std::string &filePath,
         std::string &errorMessage)
{
    // check if exist
    if(bfs::exists(filePath))
    {
        // check for directory
        if(bfs::is_directory(filePath))
        {
            errorMessage = "failed to read destination of path \""
                           + filePath +
                           "\", because it already exist and it is a directory, "
                           "but must be a file or not existing";
            return false;
        }
    }
    else
    {
        errorMessage = "destination of path \"" + filePath + "\", doesn't exist";
        return false;
    }

    std::ifstream inFile;
    inFile.open(filePath);

    std::stringstream strStream;
    strStream << inFile.rdbuf();
    readContentconst = strStream.str();

    inFile.close();

    return true;
}

/**
 * write text into a file
 *
 * @param filePath path the to file
 * @param content text which be wirtten into the file
 * @param errorMessage reference for error-message output
 * @param force if true, it overwrites the file, if there already exist one (Default: true)
 *
 * @return true, if successful, else false
 */
bool
writeFile(const std::string &filePath,
          const std::string &content,
          std::string &errorMessage,
          const bool force)
{
    // check and create parent-directory, if necessary
    const bfs::path parentDirectory = bfs::path(filePath).parent_path();
    if(bfs::exists(parentDirectory) == false)
    {
        bool result = createDirectory(parentDirectory, errorMessage);
        if(result == false) {
            return false;
        }
    }

    // check if exist
    if(bfs::exists(filePath))
    {
        // check for directory
        if(bfs::is_directory(filePath))
        {
            errorMessage = "failed to write destination of path \""
                           + filePath +
                           "\", because it already exist and it is a directory, "
                           "but must be a file or not existing";
            return false;
        }

        // check for override
        if(force == false)
        {
            errorMessage = "failed to write destination of path \""
                           + filePath +
                           "\", because it already exist, but should not be overwrite";
            return false;
        }

        // TODO: check access-rights
        // fs::path p(argv[1]);
        // fs::file_status s = status(p);
        // printf("%o\n",s.permissions());

        // remove file
        deleteFileOrDir(filePath, errorMessage);
    }

    // create new file and write content
    std::ofstream outputFile;
    outputFile.open(filePath);
    outputFile << content;
    outputFile.flush();
    outputFile.close();

    return true;
}

/**
 * append text to a existing text-file
 *
 * @param filePath path the to file
 * @param newText text which should be append to the file
 * @param errorMessage reference for error-message output
 *
 * @return true, if successful, else false
 */
bool
appendText(const std::string &filePath,
           const std::string &newText,
           std::string &errorMessage)
{
    // check for directory
    if(bfs::is_regular_file(filePath) == false)
    {
        errorMessage = "Failed to append text to file \""
                       + filePath +
                       "\", because it is not a regular file.";
        return false;
    }

    // open, write and close file again
    std::ofstream outputFile;
    outputFile.open(filePath, std::ios_base::app);
    outputFile << newText;
    outputFile.flush();
    outputFile.close();

    return true;
}

/**
 * replace a specific line inside a text-file
 *
 * @param filePath path the to file
 * @param lineNumber number of the line inside the file, which should be replaced (beginning with 0)
 * @param newLineContent the new content string for the line, which should be replaced
 * @param errorMessage reference for error-message output
 *
 * @return true, if successful, else false
 */
bool
replaceLine(const std::string &filePath,
            const uint32_t lineNumber,
            const std::string &newLineContent,
            std::string &errorMessage)
{
    // read file
    std::string fileContent = "";
    bool result = readFile(fileContent, filePath, errorMessage);
    if(result == false) {
        return false;
    }

    // split content into a vector of lines
    std::vector<std::string> splitedContent;
    Kitsunemimi::splitStringByDelimiter(splitedContent, fileContent, '\n');
    if(splitedContent.size() <= lineNumber)
    {
        errorMessage = "failed to replace line in file \""
                       + filePath +
                       "\", because linenumber is too big for the file";
        return false;
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
    const bool writeResult = writeFile(filePath,
                                       newFileContent,
                                       errorMessage,
                                       true);

    return writeResult;
}

/**
 * replace a substring inside the file with another string
 *
 * @param filePath path the to file
 * @param oldContent substring which should be replaced
 * @param newContent new string for the replacement
 * @param errorMessage reference for error-message output
 *
 * @return true, if successful, else false
 */
bool
replaceContent(const std::string &filePath,
               const std::string &oldContent,
               const std::string &newContent,
               std::string &errorMessage)
{
    // read file
    std::string fileContent = "";
    bool result = readFile(fileContent, filePath, errorMessage);
    if(result == false) {
        return false;
    }

    // replace content
    std::string::size_type pos = 0u;
    while((pos = fileContent.find(oldContent, pos)) != std::string::npos)
    {
        fileContent.replace(pos, oldContent.length(), newContent);
        pos += newContent.length();
    }

    // write file back the new content
    const bool writeResult = writeFile(filePath,
                                       fileContent,
                                       errorMessage,
                                       true);

    return writeResult;
}

} // namespace Persistence
} // namespace Kitsunemimi
