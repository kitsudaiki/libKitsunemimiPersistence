/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <iostream>
#include <libKitsunemimiPersistence/files/text_file_test.h>
#include <libKitsunemimiPersistence/files/binary_file_with_directIO_test.h>
#include <libKitsunemimiPersistence/files/binary_file_without_directIO_test.h>
#include <libKitsunemimiPersistence/database/sqlite_test.h>
#include <libKitsunemimiPersistence/logger/logger_test.h>

int main()
{
    Kitsunemimi::Persistence::TextFile_Test();
    Kitsunemimi::Persistence::BinaryFile_withDirectIO_Test();
    Kitsunemimi::Persistence::BinaryFile_withoutDirectIO_Test();
    Kitsunemimi::Persistence::Sqlite_Test();
    Kitsunemimi::Persistence::Logger_Test();
}
