/**
 *  @file    stringMethods_test.hpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef STRINGMETHODSTEST_HPP
#define STRINGMETHODSTEST_HPP

#include <testing/commonTest.hpp>

namespace Kitsune
{

class StringMethods_Test
        : public Kitsune::CommonTest
{
public:
    StringMethods_Test();

private:
    void splitString_test();
};

} // namespace Kitsune

#endif // STRINGMETHODSTEST_HPP
