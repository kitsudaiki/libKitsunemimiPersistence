/**
 *  @file    logger.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief simple logger for events
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <ctime>
#include <mutex>

#include <libKitsunemimiPersistence/files/binary_file.h>

#define LOG_DEBUG Kitsunemimi::Persistence::LOG_debug
#define LOG_INFO Kitsunemimi::Persistence::LOG_info
#define LOG_WARNING Kitsunemimi::Persistence::LOG_warning
#define LOG_ERROR Kitsunemimi::Persistence::LOG_error

#define YELLOW_COLOR "\033[1;33m"
#define WHITE_COLOR "\033[0m"
#define GREEN_COLOR "\033[1;32m"
#define RED_COLOR "\033[1;31m"
#define BLUE_COLOR "\033[1;34m"
#define PINK_COLOR "\033[1;95m"

namespace Kitsunemimi
{
namespace Persistence
{

bool initFileLogger(const std::string &directoryPath,
                    const std::string &baseFileName,
                    const bool debugLog = false);
bool initConsoleLogger(const bool debugLog = false);
bool setDebugFlag(const bool debugLog);

bool LOG_debug(const std::string message);
bool LOG_warning(const std::string message);
bool LOG_error(const std::string message);
bool LOG_info(const std::string message,
              const std::string &color = WHITE_COLOR);

bool closeLogFile();

//==================================================================================================

class Logger
{
public:
    Logger();
    ~Logger();

    bool initFileLogger(const std::string &directoryPath,
                        const std::string &baseFileName,
                        const bool debugLog);
    bool initConsoleLogger(const bool debugLog);
    bool setDebugFlag(const bool debugLog);

    void closeLogFile();

    bool logData(const std::string &message,
                 const std::string &preTag,
                 const std::string &color,
                 const bool debug = false);

    std::string m_filePath = "";
    bool m_debugLog = false;

    static Kitsunemimi::Persistence::Logger* m_logger;

private:
    bool m_enableConsoleLog = false;
    bool m_consoleDebugLog = false;

    bool m_enableFileLog = false;
    bool m_fileDebugLog = false;
    std::string m_directoryPath = "";
    std::string m_baseFileName = "";

    std::mutex m_lock;
    std::ofstream m_outputFile;

    const std::string getDatetime();
};

} // namespace Persistence
} // namespace Kitsunemimi

#endif // LOGGER_H
