/**
 *  @file    text_file.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief data-items for universal data-structures
 *
 *  @detail different methods for simple text-file-operations
 */

#ifndef TEXT_FILE_H
#define TEXT_FILE_H

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

namespace Kitsunemimi
{
namespace Persistence
{

bool
readFile(std::string &readContentconst,
         const std::string &filePath,
         std::string &errorMessage);

bool
writeFile(const std::string &filePath,
          const std::string &content,
          std::string &errorMessage,
          const bool force=true);

bool
appendText(const std::string &filePath,
           const std::string &newText,
           std::string &errorMessage);

bool
replaceLine(const std::string &filePath,
            const uint32_t lineNumber,
            const std::string &newLineContent,
            std::string &errorMessage);

bool
replaceContent(const std::string &filePath,
               const std::string &oldContent,
               const std::string &newContent,
               std::string &errorMessage);

} // namespace Persistence
} // namespace Kitsunemimi

#endif // TEXT_FILE_H
