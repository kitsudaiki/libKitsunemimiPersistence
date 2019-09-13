/**
 *  @file    binary_file_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "binary_file_test.h"

#include <boost/filesystem.hpp>
#include <files/binary_file.h>

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
    : Kitsune::Common::UnitTest("BinaryFile_Test")
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
    // init buffer and file
    Common::DataBuffer buffer;
    BinaryFile binaryFile(m_filePath, &buffer);

    // test close
    UNITTEST(binaryFile.closeFile(), true);
    UNITTEST(binaryFile.closeFile(), false);

    deleteFile();
}

/**
 * @brief BinaryFile_Test::updateFileSize_test
 */
void
BinaryFile_Test::updateFileSize_test()
{
    // init buffer and file
    Common::DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, &buffer);
    binaryFile.allocateStorage(4);
    binaryFile.closeFile();

    BinaryFile binaryFileNew(m_filePath, &buffer);
    UNITTEST(binaryFileNew.updateFileSize(), true);
    UNITTEST(binaryFileNew.m_numberOfBlocks, 4);

    uint64_t totalReadSize = binaryFileNew.m_numberOfBlocks * binaryFileNew.m_blockSize;
    UNITTEST(binaryFileNew.m_totalFileSize, totalReadSize);
}

/**
 * allocateStorage_test
 */
void
BinaryFile_Test::allocateStorage_test()
{
    // init buffer and file
    Common::DataBuffer buffer;
    BinaryFile binaryFile(m_filePath, &buffer);

    // test allocation
    UNITTEST(binaryFile.allocateStorage(4), true);
    UNITTEST(binaryFile.allocateStorage(4), true);
    UNITTEST(binaryFile.allocateStorage(0), false);

    // check meta-data
    UNITTEST(binaryFile.m_numberOfBlocks, 8);
    UNITTEST(binaryFile.m_totalFileSize, 8*binaryFile.m_blockSize);

    binaryFile.closeFile();

    // negative test
    UNITTEST(binaryFile.allocateStorage(4), false);

    deleteFile();
}

/**
 * writeSegment_test
 */
void
BinaryFile_Test::writeSegment_test()
{
    // init buffer and file
    Common::DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, &buffer);
    binaryFile.allocateStorage(4);

    // prepare test-buffer
    TestStruct testStruct;
    testStruct.a = 42;
    testStruct.c = 1337;
    buffer.addData(&testStruct);
    buffer.bufferPosition = 2 * buffer.blockSize;
    buffer.addData(&testStruct);

    // write-tests
    UNITTEST(binaryFile.writeSegment(1, 1, 0), true);
    UNITTEST(binaryFile.writeSegment(2, 1, 2), true);

    // negative tests
    UNITTEST(binaryFile.writeSegment(2, 0, 3), false);
    UNITTEST(binaryFile.writeSegment(42, 1, 3), false);
    UNITTEST(binaryFile.writeSegment(2, 42, 3), false);
    UNITTEST(binaryFile.writeSegment(2, 1, 42), false);

    // cleanup
    UNITTEST(binaryFile.closeFile(), true);
    deleteFile();
}

/**
 * readSegment_test
 */
void
BinaryFile_Test::readSegment_test()
{
    // init buffer and file
    Common::DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, &buffer);
    binaryFile.allocateStorage(4);

    // prepare test-buffer
    TestStruct testStruct;
    testStruct.a = 42;
    testStruct.c = 1337;
    buffer.addData(&testStruct);
    testStruct.a = 10;
    testStruct.c = 1234;
    buffer.bufferPosition = 2 * buffer.blockSize;
    buffer.addData(&testStruct);

    // write the two blocks of the buffer
    UNITTEST(binaryFile.writeSegment(1, 1, 0), true);
    UNITTEST(binaryFile.writeSegment(2, 1, 2), true);

    // clear orinial buffer
    memset(buffer.data, 0, buffer.totalBufferSize);
    testStruct.a = 0;
    testStruct.c = 0;

    // read the two blocks back
    UNITTEST(binaryFile.readSegment(1, 1, 1), true);
    UNITTEST(binaryFile.readSegment(2, 1, 3), true);

    // negative tests
    UNITTEST(binaryFile.readSegment(2, 0, 3), false);
    UNITTEST(binaryFile.readSegment(42, 1, 3), false);
    UNITTEST(binaryFile.readSegment(2, 42, 3), false);
    UNITTEST(binaryFile.readSegment(2, 1, 42), false);

    // copy and check the first block
    mempcpy(&testStruct,
            static_cast<uint8_t*>(buffer.data) + (1 * buffer.blockSize),
            sizeof(TestStruct));
    UNITTEST(testStruct.a, 42);
    UNITTEST(testStruct.c, 1337);

    // copy and check the second block
    mempcpy(&testStruct,
            static_cast<uint8_t*>(buffer.data) + (3 * buffer.blockSize),
            sizeof(TestStruct));
    UNITTEST(testStruct.a, 10);
    UNITTEST(testStruct.c, 1234);

    // cleanup
    UNITTEST(binaryFile.closeFile(), true);
    deleteFile();
}

/**
 * closeTest
 */
void
BinaryFile_Test::closeTest()
{
    deleteFile();
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

