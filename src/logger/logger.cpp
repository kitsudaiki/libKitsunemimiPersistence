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


    // init buffer
    m_buffer = new Common::DataBuffer(100, 512);
    resetBuffer(100);

    // init binary-file
    m_binaryFile = new Persistence::BinaryFile(m_filePath, m_buffer);
    m_binaryFile->updateFileSize();
    m_filePosition = m_binaryFile->m_numberOfBlocks;
    m_binaryFile->allocateStorage(10);

    // when opend an already existing log-file, then write a linebreak to be sure
    if(m_filePosition != 0)
    {
        // add a line-break a the end of the last block
        uint8_t* castedData = static_cast<uint8_t*>(m_buffer->data);
        castedData[511] = '\n';
        m_binaryFile->writeSegment(m_filePosition, 1);
        m_filePosition++;
    }
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

    if(m_closed == false)
    {
        m_closed = true;
        delete m_binaryFile;
        delete m_buffer;
    }

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

    // build new line
    const std::string line(getDatetime() + " " + message);

    // reinit buffer and write new line to buffer
    const uint64_t numberOfBlocks = static_cast<uint16_t>(line.size() / 512) + 1;
    resetBuffer(numberOfBlocks);
    memcpy(m_buffer->data, line.c_str(), line.size());

    // add a line-break a the end of the last block
    uint8_t* castedData = static_cast<uint8_t*>(m_buffer->data);
    castedData[(numberOfBlocks * 512) - 1] = '\n';

    // resize file, if necessary and as big as necessary
    while(m_filePosition + numberOfBlocks > m_binaryFile->m_numberOfBlocks) {
        m_binaryFile->allocateStorage(10);
    }

    // write data to file
    const bool ret = m_binaryFile->writeSegment(m_filePosition, numberOfBlocks);
    m_filePosition += numberOfBlocks;

    m_lock.clear(std::memory_order_release);

    return ret;
}

/**
 * @brief overwrite a number of block of the data-buffer with whitespaces
 *
 * @param numberOfBlocks number of block to overwrite
 */
void
Logger::resetBuffer(const uint64_t numberOfBlocks)
{
    memset(m_buffer->data, ' ', numberOfBlocks*512);
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
