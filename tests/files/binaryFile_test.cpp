/**
 *  @file    stringMethods_test.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "binaryFile_test.hpp"

#include <commonMethods/stringMethods.hpp>

namespace Kitsune
{

StringMethods_Test::StringMethods_Test()
    : Kitsune::CommonTest("StringMethods_Test")
{
    splitString_test();
}

void StringMethods_Test::splitString_test()
{
    // init
    std::string testString = "this is a test-string";

    // run task
    std::vector<std::string> result = splitString(testString, ' ');

    // check result
    UNITTEST(result.size(), 4);
    UNITTEST(result[0], "this");
    UNITTEST(result[1], "is");
    UNITTEST(result[2], "a");
    UNITTEST(result[3], "test-string");

    // make negative checks
    std::string testStringNeg = "";
    std::vector<std::string> resultNeg = splitString(testStringNeg, ' ');
    UNITTEST(resultNeg.size(), 0);
}

} // namespace Kitsune
