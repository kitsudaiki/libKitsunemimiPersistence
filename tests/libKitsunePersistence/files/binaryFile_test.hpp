/**
 *  @file    binaryFile_test.hpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef BINARYFILE_TEST_HPP
#define BINARYFILE_TEST_HPP

#include <testing/commonTest.hpp>

namespace Kitsune
{
namespace Persistence
{

class BinaryFile_Test
        : public Kitsune::CommonTest
{
public:
    BinaryFile_Test();

private:
    void initTest();
    void closeFile_test();
    void allocateStorage_test();
    void writeSegment_test();
    void readSegment_test();
    void closeTest();

    std::string m_filePath = "";
    void deleteFile();
};

} // namespace Persistence
} // namespace Kitsune

#endif // BINARYFILE_TEST_HPP
