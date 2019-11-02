/**
 *  @file    binary_file_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "binary_file_without_directIO_test.h"

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

BinaryFile_withoutDirectIO_Test::BinaryFile_withoutDirectIO_Test()
    : Kitsunemimi::Common::Test("BinaryFile_withoutDirectIO_Test")
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
BinaryFile_withoutDirectIO_Test::initTest()
{
    m_filePath = "/tmp/binaryFile_test.bin";
    deleteFile();
}

/**
 * closeFile_test
 */
void
BinaryFile_withoutDirectIO_Test::closeFile_test()
{
    // init buffer and file
    Common::DataBuffer buffer;
    BinaryFile binaryFile(m_filePath, false);

    // test close
    TEST_EQUAL(binaryFile.closeFile(), true);
    TEST_EQUAL(binaryFile.closeFile(), false);

    deleteFile();
}

/**
 * @brief BinaryFile_Test::updateFileSize_test
 */
void
BinaryFile_withoutDirectIO_Test::updateFileSize_test()
{
    // init buffer and file
    Common::DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, false);
    binaryFile.allocateStorage(4, 4000);
    binaryFile.closeFile();

    BinaryFile binaryFileNew(m_filePath, false);
    TEST_EQUAL(binaryFileNew.updateFileSize(), true);
    TEST_EQUAL(binaryFileNew.m_totalFileSize, 4*4096);

    TEST_EQUAL(binaryFileNew.m_totalFileSize, binaryFileNew.m_totalFileSize);
}

/**
 * allocateStorage_test
 */
void
BinaryFile_withoutDirectIO_Test::allocateStorage_test()
{
    // init buffer and file
    Common::DataBuffer buffer;
    BinaryFile binaryFile(m_filePath, false);

    // test allocation
    TEST_EQUAL(binaryFile.allocateStorage(4, 4000), true);
    TEST_EQUAL(binaryFile.allocateStorage(4, 4000), true);
    TEST_EQUAL(binaryFile.allocateStorage(0, 4000), false);

    // check meta-data
    TEST_EQUAL(binaryFile.m_totalFileSize, 8*4000);

    binaryFile.closeFile();

    // negative test
    TEST_EQUAL(binaryFile.allocateStorage(4, 4000), false);

    deleteFile();
}

/**
 * writeSegment_test
 */
void
BinaryFile_withoutDirectIO_Test::writeSegment_test()
{
    // init buffer and file
    Common::DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, false);
    binaryFile.allocateStorage(4);

    // prepare test-buffer
    TestStruct testStruct;
    testStruct.a = 42;
    testStruct.c = 1337;
    buffer.addData(&testStruct);
    buffer.bufferPosition = 2000;
    buffer.addData(&testStruct);

    // write-tests
    TEST_EQUAL(binaryFile.writeSegment(&buffer, 1000, 1000, 0), true);
    TEST_EQUAL(binaryFile.writeSegment(&buffer, 2000, 1000, 2000), true);

    // negative tests
    TEST_EQUAL(binaryFile.writeSegment(&buffer, 2000, 0, 3000), false);
    TEST_EQUAL(binaryFile.writeSegment(&buffer, 42000, 1000, 3000), false);
    TEST_EQUAL(binaryFile.writeSegment(&buffer, 2000, 42000, 3000), false);
    TEST_EQUAL(binaryFile.writeSegment(&buffer, 2000, 1000, 42000), false);

    // cleanup
    TEST_EQUAL(binaryFile.closeFile(), true);
    deleteFile();
}

/**
 * readSegment_test
 */
void
BinaryFile_withoutDirectIO_Test::readSegment_test()
{
    // init buffer and file
    Common::DataBuffer buffer(5);
    BinaryFile binaryFile(m_filePath, false);
    binaryFile.allocateStorage(4);

    // prepare test-buffer
    TestStruct testStruct;
    testStruct.a = 42;
    testStruct.c = 1337;
    buffer.addData(&testStruct);
    testStruct.a = 10;
    testStruct.c = 1234;
    buffer.bufferPosition = 2000;
    buffer.addData(&testStruct);

    // write the two blocks of the buffer
    TEST_EQUAL(binaryFile.writeSegment(&buffer, 1000, 1000, 0), true);
    TEST_EQUAL(binaryFile.writeSegment(&buffer, 2000, 1000, 2000), true);

    // clear orinial buffer
    memset(buffer.data, 0, buffer.totalBufferSize);
    testStruct.a = 0;
    testStruct.c = 0;

    // read the two blocks back
    TEST_EQUAL(binaryFile.readSegment(&buffer, 1000, 1000, 1000), true);
    TEST_EQUAL(binaryFile.readSegment(&buffer, 2000, 1000, 3000), true);

    // negative tests
    TEST_EQUAL(binaryFile.readSegment(&buffer, 2000, 0, 3000), false);
    TEST_EQUAL(binaryFile.readSegment(&buffer, 42000, 1000, 3000), false);
    TEST_EQUAL(binaryFile.readSegment(&buffer, 2000, 42000, 3000), false);
    TEST_EQUAL(binaryFile.readSegment(&buffer, 2000, 1000, 42000), false);

    // copy and check the first block
    mempcpy(&testStruct,
            static_cast<uint8_t*>(buffer.data) + 1000,
            sizeof(TestStruct));
    TEST_EQUAL(testStruct.a, 42);
    TEST_EQUAL(testStruct.c, 1337);

    // copy and check the second block
    mempcpy(&testStruct,
            static_cast<uint8_t*>(buffer.data) + 3000,
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
BinaryFile_withoutDirectIO_Test::closeTest()
{
    deleteFile();
}

/**
 * common usage to delete test-file
 */
void
BinaryFile_withoutDirectIO_Test::deleteFile()
{
    fs::path rootPathObj(m_filePath);
    if(fs::exists(rootPathObj)) {
        fs::remove(rootPathObj);
    }
}

} // namespace Persistence
} // namespace Kitsunemimi

