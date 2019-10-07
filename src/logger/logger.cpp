/**
 *  @file    logger.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief simple logger for events
 */

#include <libKitsunePersistence/logger/logger.h>

namespace Kitsune
{
namespace Persistence
{

Kitsune::Persistence::Logger* Logger::m_logger = nullptr;

/**
 * @brief initLogger
 * @param directoryPath
 * @param baseFileName
 * @param debugLog
 * @param logOnConsole
 * @return
 */
std::pair<bool, std::string>
initLogger(const std::string directoryPath,
           const std::string baseFileName,
           const bool debugLog,
           const bool logOnConsole)
{
    if(Logger::m_logger != nullptr)
    {
        std::string errorMessage = "logger is already initialized.";
        return std::pair<bool, std::string>(false, errorMessage);
    }

    Logger::m_logger = new Kitsune::Persistence::Logger(directoryPath,
                                                        baseFileName,
                                                        debugLog,
                                                        logOnConsole);

    return Logger::m_logger->initLogger();
}

/**
 * @brief write debug-message to logfile
 */
bool
LOG_debug(const std::string message)
{
    if(Logger::m_logger == nullptr) {
        return false;
    }

    if(Logger::m_logger->m_debugLog == false) {
        return false;
    }

    return Logger::m_logger->logData("DEBUG: " + message);
}

/**
 * @brief write info-message to logfile
 */
bool
LOG_info(const std::string message)
{
    if(Logger::m_logger == nullptr) {
        return false;
    }

    return Logger::m_logger->logData("INFO: " + message);
}

/**
 * @brief write warnign-message to logfile
 */
bool
LOG_warning(const std::string message)
{
    if(Logger::m_logger == nullptr) {
        return false;
    }

    return Logger::m_logger->logData("WARNING: " + message);
}

/**
 * @brief write error-message to logfile
 */
bool
LOG_error(const std::string message)
{
    if(Logger::m_logger == nullptr) {
        return false;
    }

    return Logger::m_logger->logData("ERROR: " + message);
}

bool
closeLogFile()
{
    if(Logger::m_logger == nullptr) {
        return false;
    }

    Logger::m_logger->closeLogFile();
    delete Logger::m_logger;
    Logger::m_logger = nullptr;

    return true;
}

//==================================================================================================

/**
 * @brief constructor
 */
Logger::Logger(const std::string directoryPath,
               const std::string baseFileName,
               const bool debugLog,
               const bool logOnConsole)
{
    m_debugLog = debugLog;
    m_logOnConsole = logOnConsole;
    m_directoryPath = directoryPath;
    m_baseFileName = baseFileName;
}

/**
 * @brief destructor
 */
Logger::~Logger()
{
    closeLogFile();
}

/**
 * @brief init logger
 *
 * @return pair of bool and string
 *         success: first element is true and the second contains an empty string
 *         fail: first element is false and the second contains the error-message
 */
std::pair<bool, std::string>
Logger::initLogger()
{
    std::pair<bool, std::string> result;

    // check if already init
    if(m_active)
    {
        std::string errorMessage = "logger is already initialized.";
        return std::pair<bool, std::string>(false, errorMessage);
    }

    fs::path rootPathObj(m_directoryPath);

    // check if exist
    if(fs::exists(rootPathObj) == false)
    {
        std::string errorMessage = "failed to initialize logger"
                                   ", because the path \""
                                   + m_directoryPath
                                   + "\" does not exist.";
        return std::pair<bool, std::string>(false, errorMessage);
    }

    // check for directory
    if(fs::is_directory(rootPathObj) == false)
    {
        std::string errorMessage = "failed to initialize logger"
                                   ", because the path \""
                                   + m_directoryPath
                                   + "\" is not an directory.";
        return std::pair<bool, std::string>(false, errorMessage);
    }

    m_lock.lock();

    // create new logger-file
    m_filePath = m_directoryPath + "/" + m_baseFileName + ".log";
    m_outputFile.open(m_filePath, std::ios_base::app);
    m_active = true;

    m_lock.unlock();

    return std::pair<bool, std::string>(true, "");
}

/**
 * @brief Logger::closeLogFile
 */
void
Logger::closeLogFile()
{
    m_lock.lock();

    m_active = false;
    m_outputFile.close();

    m_lock.unlock();
}

/**
 * @brief write message to logfile
 */
bool
Logger::logData(const std::string message)
{
    m_lock.lock();

    // check if log already closed
    if(m_active == false)
    {
        m_lock.unlock();
        return false;
    }

    // write to terminal
    if(m_logOnConsole) {
        std::cout<<message<<std::endl;
    }

    // build and write new line
    const std::string line(getDatetime() + " " + message + "\n");
    m_outputFile << line;
    m_outputFile.flush();

    m_lock.unlock();

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
