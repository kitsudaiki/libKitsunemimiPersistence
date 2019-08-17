/**
 *  @file    text_file_test.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef TEXT_FILE_TEST_H
#define TEXT_FILE_TEST_H

#include <testing/test.h>

namespace Kitsune
{
namespace Persistence
{

class TextFile_Test
        : public Kitsune::Common::Test
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
    void deleteFile();
};

} // namespace Persistence
} // namespace Kitsune

#endif // TEXT_FILE_TEST_H
