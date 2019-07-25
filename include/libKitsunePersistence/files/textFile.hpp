#ifndef TEXTFILE_HPP
#define TEXTFILE_HPP

#include <iostream>

namespace Kitsune
{
namespace Persistence
{

std::string readFile(const std::string &filePath);
bool writeFile(const std::string &filePath,
               const std::string &content,
               const bool force);

}
}

#endif // TEXTFILE_HPP
