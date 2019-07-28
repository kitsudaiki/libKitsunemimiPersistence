/**
 *  @file    textFile_test.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "textFile_test.hpp"

#include <boost/filesystem.hpp>
#include <files/textFile.hpp>

namespace fs=boost::filesystem;

namespace Kitsune
{
namespace Persistence
{

TextFile_Test::TextFile_Test()
    : Kitsune::CommonTest("TextFile_Test")
{
    writeFile_test();
    readFile_test();

    fs::remove(m_filePath);
}

void TextFile_Test::writeFile_test()
{
    std::pair<bool, std::string> ret;

    ret = writeFile(m_filePath, m_content, false);
    UNITTEST(ret.first, true);
    ret = writeFile(m_filePath, m_content, false);
    UNITTEST(ret.first, false);
    ret = writeFile(m_filePath, m_content, true);
    UNITTEST(ret.first, true);
}

void TextFile_Test::readFile_test()
{
    std::pair<bool, std::string> ret;

    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);
    UNITTEST(ret.second, m_content);

    ret = readFile(m_filePath + "_fake");
    UNITTEST(ret.first, false);
}

}
} // namespace Kitsune
