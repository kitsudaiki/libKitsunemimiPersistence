/**
 *  @file    logger_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "logger_test.h"

#include <libKitsunemimiPersistence/logger/logger.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace Kitsunemimi
{
namespace Persistence
{

Logger_Test::Logger_Test()
    : Kitsunemimi::CompareTestHelper("Logger_Test")
{
    logger_test();
}

/**
 * @brief logger_test
 */
void
Logger_Test::logger_test()
{
    // init logger
    bool ret = initFileLogger("/tmp", "testlog", true);
    TEST_EQUAL(ret, true);

    // negative-test to try reinit the logger
    ret = initFileLogger("/tmp", "testlog", true);
    TEST_EQUAL(ret, false);

    // write test-data
    TEST_EQUAL(LOG_ERROR("error1"), true);
    TEST_EQUAL(LOG_ERROR("error2"), true);
    TEST_EQUAL(LOG_ERROR("error3"), true);

    TEST_EQUAL(LOG_WARNING("warning1"), true);
    TEST_EQUAL(LOG_WARNING("warning2"), true);
    TEST_EQUAL(LOG_WARNING("warning3"), true);

    TEST_EQUAL(LOG_DEBUG("debug1"), true);
    TEST_EQUAL(LOG_DEBUG("debug2"), true);
    TEST_EQUAL(LOG_DEBUG("debug3"), true);

    TEST_EQUAL(LOG_INFO("info1"), true);
    TEST_EQUAL(LOG_INFO("info2"), true);
    TEST_EQUAL(LOG_INFO("info3"), true);

    std::string errorMessage = "";
    std::string logContent = "";
    ret = readFile(logContent, Logger::m_logger->m_filePath, errorMessage);
    std::size_t found;

    // error
    found = logContent.find("ERROR");
    TEST_NOT_EQUAL(found, std::string::npos);
    found = logContent.find("error1");
    TEST_NOT_EQUAL(found, std::string::npos);

    // warning
    found = logContent.find("WARNING");
    TEST_NOT_EQUAL(found, std::string::npos);
    found = logContent.find("warning1");
    TEST_NOT_EQUAL(found, std::string::npos);

    // debug
    found = logContent.find("DEBUG");
    TEST_NOT_EQUAL(found, std::string::npos);
    found = logContent.find("debug2");
    TEST_NOT_EQUAL(found, std::string::npos);

    // info
    found = logContent.find("INFO");
    TEST_NOT_EQUAL(found, std::string::npos);
    found = logContent.find("info3");
    TEST_NOT_EQUAL(found, std::string::npos);


    // negative test
    found = logContent.find("ASDF");
    TEST_EQUAL(found, std::string::npos);
    found = logContent.find("poi");
    TEST_EQUAL(found, std::string::npos);

    deleteFile(Logger::m_logger->m_filePath);
    closeLogFile();
}

/**
 * common usage to delete test-file
 */
void
Logger_Test::deleteFile(const std::string filePath)
{
    fs::path rootPathObj(filePath);
    if(fs::exists(rootPathObj)) {
        fs::remove(rootPathObj);
    }
}

} // namespace Persistence
} // namespace Kitsunemimi
