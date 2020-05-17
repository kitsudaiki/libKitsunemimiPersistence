/**
 *  @file    logger.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief simple logger for events
 */

#include <libKitsunemimiPersistence/logger/logger.h>

namespace Kitsunemimi
{
namespace Persistence
{

Kitsunemimi::Persistence::Logger* Logger::m_logger = nullptr;

/**
 * @brief initialize file logger
 *
 * @param directoryPath directory path where the log should be written
 * @param baseFileName base name of the log-file
 * @param debugLog true to enable debug-output
 *
 * @return false, if initializing failed, else true
 */
bool initFileLogger(const std::string directoryPath,
                    const std::string baseFileName,
                    const bool debugLog)
{
    if(Logger::m_logger == nullptr) {
        Logger::m_logger = new Kitsunemimi::Persistence::Logger();
    }

    return Logger::m_logger->initFileLogger(directoryPath, baseFileName, debugLog);
}

/**
 * @brief initialize console logger
 *
 * @param debugLog true to enable debug-output
 *
 * @return always true
 */
bool initConsoleLogger(const bool debugLog)
{
    if(Logger::m_logger == nullptr) {
        Logger::m_logger = new Kitsunemimi::Persistence::Logger();
    }

    return Logger::m_logger->initConsoleLogger(debugLog);
}


/**
 * @brief set debug-flag after the logger was already created
 *
 * @param debugLog new debug-flag
 *
 * @return false, if logger is not initialized, else true
 */
bool
setDebugFlag(const bool debugLog)
{
    if(Logger::m_logger == nullptr) {
        return false;
    }

    return Logger::m_logger->setDebugFlag(debugLog);
}

/**
 * @brief write debug-message to logfile
 */
bool
LOG_debug(const std::string message)
{
    if(Kitsunemimi::Persistence::Logger::m_logger == nullptr) {
        return false;
    }

    return Kitsunemimi::Persistence::Logger::m_logger->logData("DEBUG: " + message, true);
}

/**
 * @brief write info-message to logfile
 */
bool
LOG_info(const std::string message)
{
    if(Kitsunemimi::Persistence::Logger::m_logger == nullptr) {
        return false;
    }

    return Kitsunemimi::Persistence::Logger::m_logger->logData("INFO: " + message);
}

/**
 * @brief write warnign-message to logfile
 */
bool
LOG_warning(const std::string message)
{
    if(Kitsunemimi::Persistence::Logger::m_logger == nullptr) {
        return false;
    }

    return Kitsunemimi::Persistence::Logger::m_logger->logData("WARNING: " + message);
}

/**
 * @brief write error-message to logfile
 */
bool
LOG_error(const std::string message)
{
    if(Kitsunemimi::Persistence::Logger::m_logger == nullptr) {
        return false;
    }

    return Kitsunemimi::Persistence::Logger::m_logger->logData("ERROR: " + message);
}

/**
 * @brief closeLogFile
 * @return
 */
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
Logger::Logger()
{
}

/**
 * @brief destructor
 */
Logger::~Logger()
{
    closeLogFile();
}

/**
 * @brief initialize file logger
 *
 * @param directoryPath directory path where the log should be written
 * @param baseFileName base name of the log-file
 * @param debugLog true to enable debug-output
 *
 * @return false, if initializing failed, else true
 */
bool
Logger::initFileLogger(const std::string directoryPath,
                       const std::string baseFileName,
                       const bool debugLog)
{
    m_directoryPath = directoryPath;
    m_baseFileName = baseFileName;
    m_fileDebugLog = debugLog;

    // check if already init
    if(m_enableFileLog)
    {
        std::cout<<"file logger is already initialized."<<std::endl;
        return false;
    }

    fs::path rootPathObj(m_directoryPath);

    // check if exist
    if(fs::exists(rootPathObj) == false)
    {
        std::cout<<"failed to initialize logger, because the path \""
                 << m_directoryPath
                 << "\" does not exist."
                 <<std::endl;
        return false;
    }

    // check for directory
    if(fs::is_directory(rootPathObj) == false)
    {
        std::cout<<"failed to initialize logger, because the path \""
                 << m_directoryPath
                 << "\" is not an directory."
                 <<std::endl;
        return false;
    }

    m_lock.lock();

    // create new logger-file
    m_filePath = m_directoryPath + "/" + m_baseFileName + ".log";
    m_outputFile.open(m_filePath, std::ios_base::app);
    m_enableFileLog = true;

    m_lock.unlock();

    return true;
}

/**
 * @brief initialize console logger
 *
 * @param debugLog true to enable debug-output
 *
 * @return always true
 */
bool
Logger::initConsoleLogger(const bool debugLog)
{
    m_enableConsoleLog = true;
    m_consoleDebugLog = debugLog;

    return true;
}

/**
 * @brief set debug-flag after the logger was already created
 *
 * @param debugLog new debug-flag
 *
 * @return always true
 */
bool
Logger::setDebugFlag(const bool debugLog)
{
    m_consoleDebugLog = debugLog;
    m_fileDebugLog = debugLog;

    return true;
}

/**
 * @brief close log-file if file-logging was initialized
 */
void
Logger::closeLogFile()
{
    m_lock.lock();

    if(m_enableFileLog) {
        m_outputFile.close();
    }

    m_lock.unlock();
}

/**
 * @brief write message to logfile
 */
bool
Logger::logData(const std::string message,
                const bool debug)
{
    m_lock.lock();

    // write to terminal
    if(m_enableConsoleLog)
    {
        if(debug
                && m_consoleDebugLog == false)
        {
            m_lock.unlock();
            return false;
        }

        std::cout<<message<<std::endl;
    }

    // build and write new line
    if(m_enableFileLog)
    {
        if(debug
                && m_fileDebugLog == false)
        {
            m_lock.unlock();
            return false;
        }

        const std::string line(getDatetime() + " " + message + "\n");
        m_outputFile << line;
        m_outputFile.flush();
    }

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
} // namespace Kitsunemimi
