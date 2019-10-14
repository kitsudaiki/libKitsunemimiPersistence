/**
 *  @file    binary_file_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef BINARY_FILE_WITHOUT_DIRECTIO_TEST_H
#define BINARY_FILE_WITHOUT_DIRECTIO_TEST_H

#include <libKitsuneCommon/unit_test.h>

namespace Kitsune
{
namespace Persistence
{

class BinaryFile_withoutDirectIO_Test
        : public Kitsune::Common::UnitTest
{
public:
    BinaryFile_withoutDirectIO_Test();

private:
    void initTest();
    void closeFile_test();
    void updateFileSize_test();
    void allocateStorage_test();
    void writeSegment_test();
    void readSegment_test();
    void closeTest();

    std::string m_filePath = "";
    void deleteFile();
};

} // namespace Persistence
} // namespace Kitsune

#endif // BINARY_FILE_WITHOUT_DIRECTIO_TEST_H
