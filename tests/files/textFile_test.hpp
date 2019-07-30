/**
 *  @file    textFile_test.hpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef TEXTFILE_TEST_HPP
#define TEXTFILE_TEST_HPP

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
    void initTest();
    void writeFile_test();
    void readFile_test();
    void appendText_test();
    void replaceLine_test();
    void replaceContent_test();
    void closeTest();

    std::string m_filePath = "";
    std::string m_content = "";
};

} // namespace Persistence
} // namespace Kitsune

#endif // TEXTFILE_TEST_HPP
