/**
 *  @file    sqlite_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef SQLITE_TEST_H
#define SQLITE_TEST_H

#include <libKitsunemimiCommon/test.h>

namespace Kitsunemimi
{
namespace Persistence
{

class Sqlite_Test
        : public Kitsunemimi::Common::Test
{
public:
    Sqlite_Test();

private:
    void initTest();
    void initDB_test();
    void execSqlCommand_test();
    void closeDB_test();
    void closeTest();

    std::string m_filePath = "";
    void deleteFile();
};

} // namespace Persistence
} // namespace Kitsunemimi

#endif // SQLITE_TEST_H
