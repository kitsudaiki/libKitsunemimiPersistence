/**
 *  @file    logger_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef LOGGER_TEST_H
#define LOGGER_TEST_H

#include <libKitsunemimiCommon/test.h>

namespace Kitsunemimi
{
namespace Persistence
{

class Logger_Test
        : public Kitsunemimi::Common::Test
{
public:
    Logger_Test();

private:
    void logger_test();

    void deleteFile(const std::string filePath);
};

} // namespace Persistence
} // namespace Kitsunemimi

#endif // LOGGER_TEST_H
