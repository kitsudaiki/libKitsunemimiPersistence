/**
 *  @file    text_file_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "text_file_test.h"

#include <boost/filesystem.hpp>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace fs=boost::filesystem;

namespace Kitsunemimi
{
namespace Persistence
{

TextFile_Test::TextFile_Test()
    : Kitsunemimi::CompareTestHelper("TextFile_Test")
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
    bool ret;
    std::string errorMessage = "";
    std::string content = "this is a test\n"
                          "and this is a second line";

    // write content to file in three different cases
    ret = writeFile(m_filePath, content, errorMessage, false);
    TEST_EQUAL(ret, true);
    ret = writeFile(m_filePath, content, errorMessage, false);
    TEST_EQUAL(ret, false);
    ret = writeFile(m_filePath, content, errorMessage, true);
    TEST_EQUAL(ret, true);

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
    std::string errorMessage = "";
    std::string content = "this is a test\n"
                          "and this is a second line";

    // write initial file
    writeFile(m_filePath, content, errorMessage, true);

    // run function and check result
    ret = readFile(m_filePath, errorMessage);
    TEST_EQUAL(ret.first, true);
    TEST_EQUAL(ret.second, content);

    // negative test: file not exist
    ret = readFile(m_filePath + "_fake", errorMessage);
    TEST_EQUAL(ret.first, false);

    // cleanup
    deleteFile();
}

/**
 * appendText_test
 */
void
TextFile_Test::appendText_test()
{
    bool ret;
    std::string errorMessage = "";
    std::string content = "this is a test\n"
                          "and this is a second line";

    // write initial file
    writeFile(m_filePath, content, errorMessage, false);

    // run function
    ret = appendText(m_filePath, "\nasdfasdfasdf", errorMessage);
    TEST_EQUAL(ret, true);

    // read updated file
    std::pair<bool, std::string> readResult = readFile(m_filePath, errorMessage);
    TEST_EQUAL(readResult.first, true);

    // check result
    std::string compare = "this is a test\n"
                          "and this is a second line\n"
                          "asdfasdfasdf";
    TEST_EQUAL(readResult.second, compare);

    // cleanup
    deleteFile();
}

/**
 * replaceLine_test
 */
void
TextFile_Test::replaceLine_test()
{
    bool ret;
    std::string errorMessage = "";
    std::string content = "this is a test\n"
                          "and this is a second line\n"
                          "asdfasdfasdf";

    // write initial file
    writeFile(m_filePath, content, errorMessage, false);

    // run function
    ret = replaceLine(m_filePath,
                      2,
                      "poi",
                      errorMessage);
    TEST_EQUAL(ret, true);

    // read updated file
    std::pair<bool, std::string> readResult = readFile(m_filePath, errorMessage);
    TEST_EQUAL(readResult.first, true);

    // check result
    std::string compare = "this is a test\n"
                          "and this is a second line\n"
                          "poi";

    TEST_EQUAL(readResult.second, compare);

    // cleanup
    deleteFile();
}

/**
 * replaceContent_test
 */
void
TextFile_Test::replaceContent_test()
{
    bool ret;
    std::string errorMessage = "";
    std::string content = "this is a test\n"
                          "and this is a second line\n"
                          "poi";

    // write initial file
    writeFile(m_filePath, content, errorMessage, false);

    // run function
    ret = replaceContent(m_filePath,
                         "poi",
                         "nani",
                         errorMessage);
    TEST_EQUAL(ret, true);

    // read updated file
    std::pair<bool, std::string> readResult = readFile(m_filePath, errorMessage);
    TEST_EQUAL(readResult.first, true);

    // check result
    std::string compare = "this is a test\n"
                          "and this is a second line\n"
                          "nani";
    TEST_EQUAL(readResult.second, compare);

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
} // namespace Kitsunemimi

