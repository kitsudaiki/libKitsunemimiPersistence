/**
 *  @file    textFile_test.hpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef VECTORMETHODSTEST_HPP
#define VECTORMETHODSTEST_HPP

#include <testing/commonTest.hpp>

namespace Kitsune
{
namespace Persistence
{

class TextFile_Test
        : public Kitsune::CommonTest
{
public:
    TextFile_Test();

private:
    void writeFile_test();
    void readFile_test();

    std::string m_filePath = "/tmp/textFile_test.txt";
    std::string m_content = "this is a test\n"
                            "and this is a second line";
};

}
} // namespace Kitsune

#endif // VECTORMETHODSTEST_HPP
