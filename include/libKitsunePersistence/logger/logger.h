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

namespace Kitsune
{
namespace Persistence
{

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

    bool debug(const std::string message);
    bool info(const std::string message);
    bool warning(const std::string message);
    bool error(const std::string message);

    std::string m_filePath = "";

private:
    bool m_debugLog = false;
    bool m_logOnConsole = false;
    std::string m_directoryPath = "";
    std::string m_baseFileName = "";

    std::mutex m_lock;
    std::ofstream m_outputFile;

    bool m_active = false;

    bool logData(const std::string message);

    const std::string getDatetime();
};

} // namespace Persistence
} // namespace Kitsune

#endif // LOGGER_H
