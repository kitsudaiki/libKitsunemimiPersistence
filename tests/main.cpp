/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include <iostream>
#include <libKitsunePersistence/files/textFile_test.hpp>
#include <libKitsunePersistence/files/binaryFile_test.hpp>

int main()
{
    Kitsune::Persistence::TextFile_Test();
    Kitsune::Persistence::BinaryFile_Test();
}
