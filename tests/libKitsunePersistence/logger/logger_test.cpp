/**
 *  @file    logger_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "logger_test.h"

#include <libKitsunePersistence/logger/logger.h>
#include <libKitsunePersistence/files/text_file.h>

namespace Kitsune
{
namespace Persistence
{

Logger_Test::Logger_Test()
    : Kitsune::Common::UnitTest("Logger_Test")
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
    std::pair<bool, std::string> ret = initLogger("/tmp", "testlog", true);
    UNITTEST(ret.first, true);

    // negative-test to try reinit the logger
    ret = initLogger("/tmp", "testlog", true);
    UNITTEST(ret.first, false);

    // write test-data
    UNITTEST(LOG_error("error1"), true);
    UNITTEST(LOG_error("error2"), true);
    UNITTEST(LOG_error("error3"), true);

    UNITTEST(LOG_warning("warning1"), true);
    UNITTEST(LOG_warning("warning2"), true);
    UNITTEST(LOG_warning("warning3"), true);

    UNITTEST(LOG_debug("debug1"), true);
    UNITTEST(LOG_debug("debug2"), true);
    UNITTEST(LOG_debug("debug3"), true);

    UNITTEST(LOG_info("info1"), true);
    UNITTEST(LOG_info("info2"), true);
    UNITTEST(LOG_info("info3"), true);

    const std::string logContent = readFile(Logger::m_logger->m_filePath).second;
    std::size_t found;

    // error
    found = logContent.find("ERROR");
    UNITTEST_NEG(found, std::string::npos);
    found = logContent.find("error1");
    UNITTEST_NEG(found, std::string::npos);

    // warning
    found = logContent.find("WARNING");
    UNITTEST_NEG(found, std::string::npos);
    found = logContent.find("warning1");
    UNITTEST_NEG(found, std::string::npos);

    // debug
    found = logContent.find("DEBUG");
    UNITTEST_NEG(found, std::string::npos);
    found = logContent.find("debug2");
    UNITTEST_NEG(found, std::string::npos);

    // info
    found = logContent.find("INFO");
    UNITTEST_NEG(found, std::string::npos);
    found = logContent.find("info3");
    UNITTEST_NEG(found, std::string::npos);


    // negative test
    found = logContent.find("ASDF");
    UNITTEST(found, std::string::npos);
    found = logContent.find("poi");
    UNITTEST(found, std::string::npos);

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
} // namespace Kitsune
