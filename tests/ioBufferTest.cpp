#include <tests/ioBufferTest.h>

namespace Persistence
{

IoBufferTest::IoBufferTest()
{
}

void IoBufferTest::initTestCase()
{
    m_buffer = new IOBuffer(QString("/tmp/testfile"));
}

void IoBufferTest::getNumberOfBlocksAfterInit()
{
    QCOMPARE((int)m_buffer->getNumberOfBlocks(), 1);
}

void IoBufferTest::allocateBlocks()
{
    QCOMPARE(m_buffer->allocateBlocks(10), true);
}

void IoBufferTest::getNumberOfBlocksAfterResize()
{
    QCOMPARE((int)m_buffer->getNumberOfBlocks(), 11);
}

void IoBufferTest::writeToBuffer()
{
    quint8 *buffer = (quint8*)m_buffer->getBlock(0);
    buffer[4] = 42;
    buffer[1024] = 24;
    buffer = (quint8*)m_buffer->getBlock(8);
    buffer[4] = 42;
    buffer[1024] = 24;
    QCOMPARE(m_buffer->syncBlocks(0, 10), true);
}

void IoBufferTest::readFromBuffer()
{
    quint8 *buffer = (quint8*)m_buffer->getBlock(0);
    QCOMPARE((int)buffer[4], 42);
    QCOMPARE((int)buffer[1024], 24);
    buffer = (quint8*)m_buffer->getBlock(8);
    QCOMPARE((int)buffer[4], 42);
    QCOMPARE((int)buffer[1024], 24);
}

void IoBufferTest::closeAndReopen()
{
    QCOMPARE(m_buffer->closeBuffer(), true);
    QCOMPARE(m_buffer->closeBuffer(), false);
    m_buffer = new IOBuffer("/tmp/cluster0");
}

void IoBufferTest::readFromBufferAfterReopen()
{
    quint8 *buffer = (quint8*)m_buffer->getBlock(0);
    QCOMPARE((int)buffer[4], 42);
    QCOMPARE((int)buffer[1024], 24);
    buffer = (quint8*)m_buffer->getBlock(8);
    QCOMPARE((int)buffer[4], 42);
    QCOMPARE((int)buffer[1024], 24);
}

void IoBufferTest::closeAndReopenWithoutFile()
{
    QCOMPARE(m_buffer->closeBuffer(true), true);
    QCOMPARE(m_buffer->initBuffer(), true);
    quint8 *buffer = (quint8*)m_buffer->getBlock(0);
    QCOMPARE((int)buffer[4], 42);
    QCOMPARE((int)buffer[1024], 24);
    buffer = (quint8*)m_buffer->getBlock(8);
    QCOMPARE((int)buffer[4], 42);
    QCOMPARE((int)buffer[1024], 24);
}

void IoBufferTest::cleanupTestCase()
{
    delete m_buffer;
    QFile::remove("/tmp/cluster0");
}

}
