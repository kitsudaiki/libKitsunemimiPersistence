/**
 *  @file    logger_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef LOGGER_TEST_H
#define LOGGER_TEST_H

#include <libKitsuneCommon/unit_test.h>

namespace Kitsune
{
namespace Persistence
{

class Logger_Test
        : public Kitsune::Common::UnitTest
{
public:
    Logger_Test();

private:
    void logger_test();

    void deleteFile(const std::string filePath);
};

} // namespace Persistence
} // namespace Kitsune

#endif // LOGGER_TEST_H
