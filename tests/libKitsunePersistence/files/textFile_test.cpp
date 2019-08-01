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
    std::string content = "this is a test\n"
                          "and this is a second line";

    // write content to file in three different cases
    ret = writeFile(m_filePath, content, false);
    UNITTEST(ret.first, true);
    ret = writeFile(m_filePath, content, false);
    UNITTEST(ret.first, false);
    ret = writeFile(m_filePath, content, true);
    UNITTEST(ret.first, true);

    // cleanup
    deleteFile();
}

/**
 * readFile_test
 */
void
TextFile_Test::readFile_test()
{
    std::pair<bool, std::string> ret;
    std::string content = "this is a test\n"
                          "and this is a second line";

    // write initial file
    ret = writeFile(m_filePath, content, false);
    UNITTEST(ret.first, true);

    // run function and check result
    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);
    UNITTEST(ret.second, content);

    // negative test: file not exist
    ret = readFile(m_filePath + "_fake");
    UNITTEST(ret.first, false);

    // cleanup
    deleteFile();
}

/**
 * appendText_test
 */
void
TextFile_Test::appendText_test()
{
    std::pair<bool, std::string> ret;
    std::string content = "this is a test\n"
                          "and this is a second line";

    // write initial file
    ret = writeFile(m_filePath, content, false);
    UNITTEST(ret.first, true);

    // run function
    ret = appendText(m_filePath,
                     "\nasdfasdfasdf");
    UNITTEST(ret.first, true);

    // read updated file
    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);

    // check result
    std::string compare = "this is a test\n"
                          "and this is a second line\n"
                          "asdfasdfasdf";
    UNITTEST(ret.second, compare);

    // cleanup
    deleteFile();
}

/**
 * replaceLine_test
 */
void
TextFile_Test::replaceLine_test()
{
    std::pair<bool, std::string> ret;
    std::string content = "this is a test\n"
                          "and this is a second line\n"
                          "asdfasdfasdf";

    // write initial file
    ret = writeFile(m_filePath, content, false);
    UNITTEST(ret.first, true);

    // run function
    ret = replaceLine(m_filePath,
                      2,
                      "poi");
    UNITTEST(ret.first, true);

    // read updated file
    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);

    // check result
    std::string compare = "this is a test\n"
                          "and this is a second line\n"
                          "poi";

    UNITTEST(ret.second, compare);

    // cleanup
    deleteFile();
}

/**
 * replaceContent_test
 */
void
TextFile_Test::replaceContent_test()
{
    std::pair<bool, std::string> ret;
    std::string content = "this is a test\n"
                          "and this is a second line\n"
                          "poi";

    // write initial file
    ret = writeFile(m_filePath, content, false);
    UNITTEST(ret.first, true);

    // run function
    ret = replaceContent(m_filePath,
                         "poi",
                         "nani");
    UNITTEST(ret.first, true);

    // read updated file
    ret = readFile(m_filePath);
    UNITTEST(ret.first, true);

    // check result
    std::string compare = "this is a test\n"
                          "and this is a second line\n"
                          "nani";
    UNITTEST(ret.second, compare);

    // cleanup
    deleteFile();
}

/**
 * closeTest
 */
void
TextFile_Test::closeTest()
{
    deleteFile();
}

/**
 * common usage to delete test-file
 */
void
TextFile_Test::deleteFile()
{
    fs::path rootPathObj(m_filePath);
    if(fs::exists(rootPathObj)) {
        fs::remove(rootPathObj);
    }
}

} // namespace Persistence
} // namespace Kitsune

