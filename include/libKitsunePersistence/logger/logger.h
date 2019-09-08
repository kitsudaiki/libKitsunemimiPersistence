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
#include <atomic>

#include <files/binary_file.h>

namespace Kitsune
{
namespace Persistence
{

class Logger
{
public:
    Logger(const std::string directoryPath,
           const std::string baseFileName);
    ~Logger();

    void closeLogFile();

    bool info(const std::string message);
    bool debug(const std::string message);
    bool warning(const std::string message);
    bool error(const std::string message);

    std::string m_filePath = "";

private:
    std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
    std::ofstream m_outputFile;
    bool m_closed = false;

    bool logData(const std::string message);

    const std::string getDatetime();
};

} // namespace Persistence
} // namespace Kitsune

#endif // LOGGER_H
