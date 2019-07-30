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
    initTest();
    writeFile_test();
    readFile_test();
    appendText_test();
    replaceLine_test();
    replaceContent_test();
    closeTest();
}

/**
 * initTest
 */
void
TextFile_Test::initTest()
{
    m_filePath = "/tmp/textFile_test.txt";
    m_content = "this is a test\n"
                "and this is a second line";

    fs::path rootPathObj(m_filePath);
    if(fs::exists(rootPathObj)) {
        fs::remove(rootPathObj);
    }
}

/**
 * writeFile_test
 */
void
TextFile_Test::writeFile_test()
{
    std::pair<bool, std::string> ret;

    ret = writeFile(m_filePath, m_content, false);
    UNITTEST(ret.first, true);
    ret = writeFile(m_filePath, m_content, false);
    UNITTEST(ret.first, false);
    ret = writeFile(m_filePath, m_content, true);
    UNITTEST(ret.first, true);
}

/**
 * readFile_test
 */
void
TextFile_Test::readFile_test()
{
    std::pair<bool, std::string> ret;

    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);
    UNITTEST(ret.second, m_content);

    ret = readFile(m_filePath + "_fake");
    UNITTEST(ret.first, false);
}

/**
 * appendText_test
 */
void
TextFile_Test::appendText_test()
{
    std::pair<bool, std::string> ret;
    ret = appendText(m_filePath,
                     "\nasdfasdfasdf");
    UNITTEST(ret.first, true);

    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);
    UNITTEST(ret.second, m_content + "\nasdfasdfasdf");
}

/**
 * replaceLine_test
 */
void
TextFile_Test::replaceLine_test()
{
    std::pair<bool, std::string> ret;

    ret = replaceLine(m_filePath,
                      2,
                      "poi");
    UNITTEST(ret.first, true);

    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);
    UNITTEST(ret.second, m_content + "\npoi");
}

/**
 * replaceContent_test
 */
void
TextFile_Test::replaceContent_test()
{
    std::pair<bool, std::string> ret;

    ret = replaceContent(m_filePath,
                         "poi",
                         "nani");
    UNITTEST(ret.first, true);

    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);
    UNITTEST(ret.second, m_content + "\nnani");
}

/**
 * closeTest
 */
void
TextFile_Test::closeTest()
{
    fs::remove(m_filePath);
}

} // namespace Persistence
} // namespace Kitsune

