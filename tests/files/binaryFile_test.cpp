/**
 *  @file    binaryFile_test.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "binaryFile_test.hpp"

#include <boost/filesystem.hpp>
#include <files/binaryFile.hpp>

namespace fs=boost::filesystem;

namespace Kitsune
{
namespace Persistence
{

struct TestStruct
{
    uint8_t a = 0;
    uint8_t b = 0;
    uint64_t c = 0;
} __attribute__((packed));

BinaryFile_Test::BinaryFile_Test()
    : Kitsune::CommonTest("BinaryFile_Test")
{
    initTest();
    closeFile_test();
    allocateStorage_test();
    writeSegment_test();
    readSegment_test();
    closeTest();
}

/**
 * initTest
 */
void
BinaryFile_Test::initTest()
{
    m_filePath = "/tmp/binaryFile_test.bin";
    deleteFile();
}

/**
 * closeFile_test
 */
void
BinaryFile_Test::closeFile_test()
{
    CommonDataBuffer buffer;
    BinaryFile binaryFile(m_filePath, &buffer);

    UNITTEST(binaryFile.closeFile(), true);
    UNITTEST(binaryFile.closeFile(), false);

    deleteFile();
}

/**
 * allocateStorage_test
 */
void
BinaryFile_Test::allocateStorage_test()
{
    CommonDataBuffer buffer;
    BinaryFile binaryFile(m_filePath, &buffer);

    UNITTEST(binaryFile.allocateStorage(4), true);
    UNITTEST(binaryFile.allocateStorage(4), true);
    UNITTEST(binaryFile.allocateStorage(0), false);
    binaryFile.closeFile();
    UNITTEST(binaryFile.allocateStorage(4), false);

    UNITTEST(binaryFile.m_numberOfBlocks, 8);
    UNITTEST(binaryFile.m_totalFileSize, 8*binaryFile.m_blockSize);

    deleteFile();
}

/**
 * writeSegment_test
 */
void
BinaryFile_Test::writeSegment_test()
{
    CommonDataBuffer buffer;
    BinaryFile binaryFile(m_filePath, &buffer);

    TestStruct testStruct;
    testStruct.a = 42;
    testStruct.c = 1337;

    buffer.addData(&testStruct);

    UNITTEST(binaryFile.allocateStorage(4), true);
    UNITTEST(binaryFile.writeSegment(0, 1, 0), true);

    UNITTEST(binaryFile.closeFile(), true);

    deleteFile();
}

/**
 * readSegment_test
 */
void
BinaryFile_Test::readSegment_test()
{

}

/**
 * closeTest
 */
void
BinaryFile_Test::closeTest()
{
}

/**
 * common usage to delete test-file
 */
void
BinaryFile_Test::deleteFile()
{
    fs::path rootPathObj(m_filePath);
    if(fs::exists(rootPathObj)) {
        fs::remove(rootPathObj);
    }
}

} // namespace Persistence
} // namespace Kitsune

