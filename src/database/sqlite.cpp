/**
 *  @file    sqlite.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 *
 *  @brief simple class for easier handling of sqlite-database
 *
 *  @detail This class provides only three abilities: open and close sqlite databases and
 *          execute sql-commands. The results of the database request are converted into
 *          table-itmes of libKitsunemimiCommon.
 *
 *          This class was created with the help of:
 *              https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
 */

#include <libKitsunemimiPersistence/database/sqlite.h>
#include <libKitsunemimiCommon/common_items/table_item.h>
#include <libKitsunemimiCommon/common_items/data_items.h>

using Kitsunemimi::Common::DataItem;
using Kitsunemimi::Common::DataMap;
using Kitsunemimi::Common::DataArray;
using Kitsunemimi::Common::DataValue;

namespace Kitsunemimi
{
namespace Persistence
{

/**
 * @brief constructor
 */
Sqlite::Sqlite() {}

/**
 * @brief destcutor
 */
Sqlite::~Sqlite()
{
    closeDB();
}

/**
 * @brief initialize database
 *
 * @param path file-path of the existing or new sqlite file
 *
 * @return pair of bool and string
 *         if success: first is true and second is empty string
 *         if failed: first is false and second is the error-message
 */
std::pair<bool, std::string>
Sqlite::initDB(const std::string path)
{
    std::pair<bool, std::string> result;

    m_rc = sqlite3_open(path.c_str(), &m_db);

    if(m_rc != 0)
    {
        result.second = "Can't open database: \n";
        result.second.append(sqlite3_errmsg(m_db));
        result.first = false;
        return result;
    }

    result.first = true;
    return result;
}

/**
 * @brief callback-fucntion, which is called for every row of the result of the sql-request
 *
 * @param data Data provided in the 4th argument of sqlite3_exec()
 * @param argc The number of columns in row
 * @param argv An array of strings representing fields in the row
 * @param azColName An array of strings representing column names
 *
 * @return 0
 */
static int callback(void* data,
                    int argc,
                    char** argv,
                    char** azColName)
{
    // precheck
    if(data == nullptr) {
        return 0;
    }

    Common::TableItem* result = static_cast<Common::TableItem*>(data);

    // add columns to the table-item, but only the first time
    // because this callback is called for every row
    if(result->getNumberOfColums() == 0)
    {
        for(int i = 0; i < argc; i++)
        {
            const std::string columnName = azColName[i];
            result->addColumn(columnName);
        }
    }

    // collect row-data
    std::vector<std::string> row;
    for(int i = 0; i < argc; i++)
    {
        row.push_back(std::string(argv[i] ? argv[i] : "NULL"));
    }

    // add row to the table-item
    result->addRow(row);

    return 0;
}

/**
 * @brief execute a sql-command on the sqlite database
 *
 * @param resultTable table-item, which should contain the result
 * @param command sql-command for execution
 *
 * @return pair of bool and string
 *         if success: first is true and second is the string-form of the table with the result
 *         if failed: first is false and second is the error-message
 */
std::pair<bool, std::string>
Sqlite::execSqlCommand(Common::TableItem* resultTable,
                       const std::string command)
{
    // init
    std::pair<bool, std::string> result;
    char* errorMessage = nullptr;

    // run command
    m_rc = sqlite3_exec(m_db,
                        command.c_str(),
                        callback,
                        static_cast<void*>(resultTable),
                        &errorMessage);

    // check result state
    if(m_rc != SQLITE_OK)
    {
        // build error-result
        result.first = false;
        result.second = "SQL error: " + std::string(errorMessage);
        sqlite3_free(errorMessage);

        return result;
    }

    // build success-result
    result.first = true;
    result.second = "";

    return result;
}

/**
 * @brief close the database
 *
 * @return false if db was not open, else true
 */
bool
Sqlite::closeDB()
{
    if(m_db != nullptr)
    {
        sqlite3_close(m_db);
        m_db = nullptr;
        return true;
    }

    return false;
}

} // namespace Persistence
} // namespace Kitsunemimi
