/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <iostream>
#include <libKitsunePersistence/files/text_file_test.h>
#include <libKitsunePersistence/files/binary_file_test.h>

int main()
{
    Kitsune::Persistence::TextFile_Test();
    Kitsune::Persistence::BinaryFile_Test();
}
