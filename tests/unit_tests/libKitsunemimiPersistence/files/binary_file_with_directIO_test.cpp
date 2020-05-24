/**
 *  @file    binary_file_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "binary_file_with_directIO_test.h"

#include <boost/filesystem.hpp>
#include <libKitsunemimiPersistence/files/binary_file.h>

namespace fs=boost::filesystem;

namespace Kitsunemimi
{
namespace Persistence
{

struct TestStruct
{
    uint8_t a = 0;
    uint8_t b = 0;
    uint64_t c = 0;
} __attribute__((packed));

BinaryFile_withDirectIO_Test::BinaryFile_withDirectIO_Test()
    : Kitsunemimi::CompareTestHelper("BinaryFile_withDirectIO_Test")
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
BinaryFile_withDirectIO_Test::initTest()
{
    m_filePath = "/tmp/binaryFile_test.bin";
    deleteFile();
}

/**
 * closeFile_test
 */
void
BinaryFile_withDirectIO_Test::closeFile_test()
{
    // init buffer and file
    DataBuffer buffer;
    BinaryFile binaryFile(m_filePath, true);

    // test close
    TEST_EQUAL(binaryFile.closeFile(), true);
    TEST_EQUAL(binaryFile.closeFile(), false);

    deleteFile();
}

/**
 * @brief BinaryFile_Test::updateFileSize_test
 */
void
BinaryFile_withDirectIO_Test::updateFileSize_test()
{
    // init buffer and file
    DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, true);
    binaryFile.allocateStorage(4);
    binaryFile.closeFile();

    BinaryFile binaryFileNew(m_filePath);
    TEST_EQUAL(binaryFileNew.updateFileSize(), true);
    TEST_EQUAL(binaryFileNew.m_totalFileSize, 4*4096);

    TEST_EQUAL(binaryFileNew.m_totalFileSize, binaryFileNew.m_totalFileSize);
}

/**
 * allocateStorage_test
 */
void
BinaryFile_withDirectIO_Test::allocateStorage_test()
{
    // init buffer and file
    DataBuffer buffer;
    BinaryFile binaryFile(m_filePath, true);

    // test allocation
    TEST_EQUAL(binaryFile.allocateStorage(4), true);
    TEST_EQUAL(binaryFile.allocateStorage(4), true);
    TEST_EQUAL(binaryFile.allocateStorage(0), false);

    // check meta-data
    TEST_EQUAL(binaryFile.m_totalFileSize, 8*4096);

    binaryFile.closeFile();

    // negative test
    TEST_EQUAL(binaryFile.allocateStorage(4), false);

    deleteFile();
}

/**
 * writeSegment_test
 */
void
BinaryFile_withDirectIO_Test::writeSegment_test()
{
    // init buffer and file
    DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, true);
    binaryFile.allocateStorage(4);

    // prepare test-buffer
    TestStruct testStruct;
    testStruct.a = 42;
    testStruct.c = 1337;
    addObject_DataBuffer(buffer, &testStruct);
    buffer.bufferPosition = 2 * buffer.blockSize;
    addObject_DataBuffer(buffer, &testStruct);

    // write-tests
    TEST_EQUAL(binaryFile.writeSegment(buffer, 1, 1, 0), true);
    TEST_EQUAL(binaryFile.writeSegment(buffer, 2, 1, 2), true);

    // negative tests
    TEST_EQUAL(binaryFile.writeSegment(buffer, 2, 0, 3), false);
    TEST_EQUAL(binaryFile.writeSegment(buffer, 42, 1, 3), false);
    TEST_EQUAL(binaryFile.writeSegment(buffer, 2, 42, 3), false);
    TEST_EQUAL(binaryFile.writeSegment(buffer, 2, 1, 42), false);

    // cleanup
    TEST_EQUAL(binaryFile.closeFile(), true);
    deleteFile();
}

/**
 * readSegment_test
 */
void
BinaryFile_withDirectIO_Test::readSegment_test()
{
    // init buffer and file
    DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, true);
    binaryFile.allocateStorage(4);

    // prepare test-buffer
    TestStruct testStruct;
    testStruct.a = 42;
    testStruct.c = 1337;
    addObject_DataBuffer(buffer, &testStruct);
    testStruct.a = 10;
    testStruct.c = 1234;
    buffer.bufferPosition = 2 * buffer.blockSize;
    addObject_DataBuffer(buffer, &testStruct);

    // write the two blocks of the buffer
    TEST_EQUAL(binaryFile.writeSegment(buffer, 1, 1, 0), true);
    TEST_EQUAL(binaryFile.writeSegment(buffer, 2, 1, 2), true);

    // clear orinial buffer
    memset(buffer.data, 0, buffer.totalBufferSize);
    testStruct.a = 0;
    testStruct.c = 0;

    // read the two blocks back
    TEST_EQUAL(binaryFile.readSegment(buffer, 1, 1, 1), true);
    TEST_EQUAL(binaryFile.readSegment(buffer, 2, 1, 3), true);

    // negative tests
    TEST_EQUAL(binaryFile.readSegment(buffer, 2, 0, 3), false);
    TEST_EQUAL(binaryFile.readSegment(buffer, 42, 1, 3), false);
    TEST_EQUAL(binaryFile.readSegment(buffer, 2, 42, 3), false);
    TEST_EQUAL(binaryFile.readSegment(buffer, 2, 1, 42), false);

    // copy and check the first block
    mempcpy(&testStruct,
            static_cast<uint8_t*>(buffer.data) + (1 * buffer.blockSize),
            sizeof(TestStruct));
    TEST_EQUAL(testStruct.a, 42);
    TEST_EQUAL(testStruct.c, 1337);

    // copy and check the second block
    mempcpy(&testStruct,
            static_cast<uint8_t*>(buffer.data) + (3 * buffer.blockSize),
            sizeof(TestStruct));
    TEST_EQUAL(testStruct.a, 10);
    TEST_EQUAL(testStruct.c, 1234);

    // cleanup
    TEST_EQUAL(binaryFile.closeFile(), true);
    deleteFile();
}

/**
 * closeTest
 */
void
BinaryFile_withDirectIO_Test::closeTest()
{
    deleteFile();
}

/**
 * common usage to delete test-file
 */
void
BinaryFile_withDirectIO_Test::deleteFile()
{
    fs::path rootPathObj(m_filePath);
    if(fs::exists(rootPathObj)) {
        fs::remove(rootPathObj);
    }
}

} // namespace Persistence
} // namespace Kitsunemimi

