#include "textFile.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <fstream>
#include <boost/filesystem.hpp>

namespace Kitsune
{
namespace Persistence
{

std::string
readFile(const std::string &filePath)
{
    std::ifstream inFile;
    inFile.open(filePath);

    std::stringstream strStream;
    strStream << inFile.rdbuf();
    std::string fileContent = strStream.str();

    inFile.close();

    return fileContent;
}

bool
writeFile(const std::string &filePath,
          const std::string &content,
          const bool force)
{
    boost::filesystem::path rootPathObj(filePath);
    if(boost::filesystem::exists(rootPathObj))
    {
        if(boost::filesystem::is_directory(rootPathObj)
                || force == false)
        {
            return false;
        }
        boost::filesystem::remove(rootPathObj);
    }

    std::ofstream outputFile;
    outputFile.open(filePath);
    outputFile << content;
    outputFile.close();

    return true;
}

}
}
