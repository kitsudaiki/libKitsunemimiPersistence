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

namespace Kitsune
{
namespace Persistence
{

std::pair<bool, std::string>
readFile(const std::string &filePath);

std::pair<bool, std::string>
writeFile(const std::string &filePath,
          const std::string &content,
          const bool force);

std::pair<bool, std::string>
appendText(const std::string &filePath,
           const std::string &newText);

std::pair<bool, std::string>
replaceLine(const std::string &filePath,
            const uint32_t lineNumber,
            const std::string newLineContent);

std::pair<bool, std::string>
replaceContent(const std::string &filePath,
               const std::string oldContent,
               const std::string newContent);

} // namespace Persistence
} // namespace Kitsune

#endif // TEXT_FILE_H
