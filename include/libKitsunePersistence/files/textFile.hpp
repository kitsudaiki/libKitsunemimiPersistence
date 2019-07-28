#ifndef TEXTFILE_HPP
#define TEXTFILE_HPP

#include <iostream>

namespace Kitsune
{
namespace Persistence
{

std::pair<bool, std::string> readFile(const std::string &filePath);
std::pair<bool, std::string> writeFile(const std::string &filePath,
               const std::string &content,
               const bool force);

}
}

#endif // TEXTFILE_HPP
