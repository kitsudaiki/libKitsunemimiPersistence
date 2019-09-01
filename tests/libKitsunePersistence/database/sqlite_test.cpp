/**
 *  @file    sqlite_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "sqlite_test.h"

#include <database/sqlite.h>
#include <common_items/table_item.h>
#include <boost/filesystem.hpp>

namespace fs=boost::filesystem;

namespace Kitsune
{
namespace Persistence
{

Sqlite_Test::Sqlite_Test()
    : Kitsune::Common::UnitTest("Sqlite_Test")
{
    initTest();
    initDB_test();
    execSqlCommand_test();
    closeDB_test();
    closeTest();
}

/**
 * @brief initTest
 */
void
Sqlite_Test::initTest()
{
    m_filePath = "/tmp/testdb.db";
    deleteFile();
}

/**
 * @brief initDB_test
 */
void
Sqlite_Test::initDB_test()
{
    Sqlite testDB;

    std::pair<bool, std::string> result;
    result = testDB.initDB(m_filePath);

    UNITTEST(result.first, true);

    deleteFile();
}

/**
 * @brief execSqlCommand_test
 */
void
Sqlite_Test::execSqlCommand_test()
{
    Sqlite testDB;
    testDB.initDB(m_filePath);
    std::pair<bool, std::string> result;
    Kitsune::Common::TableItem resultItem;

    //-----------------------------------------------------------------
    // CREATE TABLE
    //-----------------------------------------------------------------
    std::string sql =
            "CREATE TABLE COMPANY("
            "ID INT PRIMARY KEY     NOT NULL,"
            "NAME           TEXT    NOT NULL,"
            "AGE            INT     NOT NULL,"
            "ADDRESS        CHAR(50),"
            "SALARY         REAL );";

    result = testDB.execSqlCommand(nullptr, sql);
    UNITTEST(result.first, true);

    //-----------------------------------------------------------------
    // INSERT
    //-----------------------------------------------------------------
    sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
           "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
           "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
           "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
           "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
           "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
           "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
           "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

    result = testDB.execSqlCommand(nullptr, sql);
    UNITTEST(result.first, true);

    //-----------------------------------------------------------------
    // SELECT
    //-----------------------------------------------------------------
    sql = "SELECT * from COMPANY";

    resultItem.clearTable();
    result = testDB.execSqlCommand(&resultItem, sql);
    UNITTEST(result.first, true);

    std::string compare =
            "+----+-------+-----+------------+---------+\n"
            "| ID | NAME  | AGE | ADDRESS    | SALARY  |\n"
            "+====+=======+=====+============+=========+\n"
            "| 1  | Paul  | 32  | California | 20000.0 |\n"
            "+----+-------+-----+------------+---------+\n"
            "| 2  | Allen | 25  | Texas      | 15000.0 |\n"
            "+----+-------+-----+------------+---------+\n"
            "| 3  | Teddy | 23  | Norway     | 20000.0 |\n"
            "+----+-------+-----+------------+---------+\n"
            "| 4  | Mark  | 25  | Rich-Mond  | 65000.0 |\n"
            "+----+-------+-----+------------+---------+\n";
    UNITTEST(resultItem.toString(), compare);

    //-----------------------------------------------------------------
    // UPDATE
    //-----------------------------------------------------------------
    sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
          "SELECT * from COMPANY";

    resultItem.clearTable();
    result = testDB.execSqlCommand(&resultItem, sql);
    UNITTEST(result.first, true);

    compare =
            "+----+-------+-----+------------+---------+\n"
            "| ID | NAME  | AGE | ADDRESS    | SALARY  |\n"
            "+====+=======+=====+============+=========+\n"
            "| 1  | Paul  | 32  | California | 25000.0 |\n"
            "+----+-------+-----+------------+---------+\n"
            "| 2  | Allen | 25  | Texas      | 15000.0 |\n"
            "+----+-------+-----+------------+---------+\n"
            "| 3  | Teddy | 23  | Norway     | 20000.0 |\n"
            "+----+-------+-----+------------+---------+\n"
            "| 4  | Mark  | 25  | Rich-Mond  | 65000.0 |\n"
            "+----+-------+-----+------------+---------+\n";
    UNITTEST(resultItem.toString(), compare);

    //-----------------------------------------------------------------
    // DELETE
    //-----------------------------------------------------------------
    sql = "DELETE from COMPANY where ID=2; "
          "SELECT * from COMPANY";

    resultItem.clearTable();
    result = testDB.execSqlCommand(&resultItem, sql);
    UNITTEST(result.first, true);

    compare =
           "+----+-------+-----+------------+---------+\n"
           "| ID | NAME  | AGE | ADDRESS    | SALARY  |\n"
           "+====+=======+=====+============+=========+\n"
           "| 1  | Paul  | 32  | California | 25000.0 |\n"
           "+----+-------+-----+------------+---------+\n"
           "| 3  | Teddy | 23  | Norway     | 20000.0 |\n"
           "+----+-------+-----+------------+---------+\n"
           "| 4  | Mark  | 25  | Rich-Mond  | 65000.0 |\n"
           "+----+-------+-----+------------+---------+\n";
    UNITTEST(resultItem.toString(), compare);


    testDB.closeDB();

    deleteFile();
}

/**
 * @brief closeDB_test
 */
void
Sqlite_Test::closeDB_test()
{
    Sqlite testDB;

    UNITTEST(testDB.closeDB(), false);

    testDB.initDB(m_filePath);

    UNITTEST(testDB.closeDB(), true);
    UNITTEST(testDB.closeDB(), false);

    deleteFile();
}

/**
 * @brief closeTest
 */
void
Sqlite_Test::closeTest()
{
    deleteFile();
}

/**
 * common usage to delete test-file
 */
void
Sqlite_Test::deleteFile()
{
    fs::path rootPathObj(m_filePath);
    if(fs::exists(rootPathObj)) {
        fs::remove(rootPathObj);
    }
}

} // namespace Persistence
} // namespace Kitsune
