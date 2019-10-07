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

#include <files/binary_file.h>


bool LOG_debug(const std::string &message);
bool LOG_info(const std::string &message);
bool LOG_warning(const std::string &message);
bool LOG_error(const std::string &message);

namespace Kitsune
{
namespace Persistence
{

std::pair<bool, std::string> initLogger(const std::string directoryPath,
                                        const std::string baseFileName,
                                        const bool debugLog=false,
                                        const bool logOnConsole=false);
bool closeLogFile();

//==================================================================================================

class Logger
{
public:
    Logger(const std::string directoryPath,
           const std::string baseFileName,
           const bool debugLog=false,
           const bool logOnConsole=false);
    ~Logger();

    std::pair<bool, std::string> initLogger();
    void closeLogFile();

    bool logData(const std::string message);

    std::string m_filePath = "";
    bool m_debugLog = false;

    static Kitsune::Persistence::Logger* m_logger;

private:
    bool m_logOnConsole = false;
    std::string m_directoryPath = "";
    std::string m_baseFileName = "";

    std::mutex m_lock;
    std::ofstream m_outputFile;

    bool m_active = false;

    const std::string getDatetime();
};

} // namespace Persistence
} // namespace Kitsune

#endif // LOGGER_H
