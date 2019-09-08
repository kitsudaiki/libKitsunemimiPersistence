/**
 *  @file    logger.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief simple logger for events
 */

#include <logger/logger.h>

namespace Kitsune
{
namespace Persistence
{

/**
 * @brief constructor
 */
Logger::Logger(const std::string directoryPath,
               const std::string baseFileName)
{
    m_filePath= directoryPath + "/" + baseFileName + ".log";
    // TODO: check if directory exist

    m_outputFile.open(m_filePath, std::ios_base::app);
}

/**
 * @brief destructor
 */
Logger::~Logger()
{
    closeLogFile();
}

/**
 * @brief Logger::closeLogFile
 */
void
Logger::closeLogFile()
{
    while (m_lock.test_and_set(std::memory_order_acquire))
         ; // spin

    m_closed = true;
    m_outputFile.close();

    m_lock.clear(std::memory_order_release);
}

/**
 * @brief write info-message to logfile
 */
bool
Logger::info(const std::string message)
{
    return logData("INFO: " + message);
}

/**
 * @brief write debug-message to logfile
 */
bool
Logger::debug(const std::string message)
{
    return logData("DEBUG: " + message);
}

/**
 * @brief write warnign-message to logfile
 */
bool
Logger::warning(const std::string message)
{
    return logData("WARNING: " + message);
}

/**
 * @brief write error-message to logfile
 */
bool
Logger::error(const std::string message)
{
    return logData("ERROR: " + message);
}

/**
 * @brief write message to logfile
 */
bool
Logger::logData(const std::string message)
{
    while (m_lock.test_and_set(std::memory_order_acquire))
         ; // spin


    if(m_closed) {
        return false;
    }

    const std::string line(getDatetime() + " " + message + "\n");
    m_outputFile << line;
    m_outputFile.flush();

    m_lock.clear(std::memory_order_release);

    return true;
}

/**
 * @brief get the current datetime of the system
 *
 * @return datetime as string
 */
const std::string
Logger::getDatetime()
{
    const time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    const std::string datatime =
            std::to_string(1900 + ltm->tm_year)
            + "-"
            + std::to_string(1 + ltm->tm_mon)
            + "-"
            + std::to_string(ltm->tm_mday)
            + " "
            + std::to_string(ltm->tm_hour)
            + ":"
            + std::to_string(ltm->tm_min)
            + ":"
            + std::to_string(ltm->tm_sec);

    return datatime;
}

} // namespace Persistence
} // namespace Kitsune
