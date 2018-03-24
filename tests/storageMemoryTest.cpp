#include <tests/storageMemoryTest.h>

namespace Persistence
{

StorageMemoryTest::StorageMemoryTest()
{

}

void* StorageMemoryTest::aligned_malloc(const quint32 size)
{
    void *mem = malloc(size+4096+sizeof(void*));
    void **ptr = (void**)((uintptr_t)(mem+4096+sizeof(void*)) & ~(4096-1));
    ptr[-1] = mem;
    memset(ptr, 0, size);
    return ptr;
}

bool StorageMemoryTest::aligned_free(void *ptr)
{
    if(ptr != nullptr) {
        free(((void**)ptr)[-1]);
        ptr = nullptr;
        return true;
    }
    return false;
}

void StorageMemoryTest::initTestCase()
{
    QFile::remove("/tmp/cluster0");
    m_storage = new Persistence::StorageMemory("/tmp/testfile");
    m_buffer = aligned_malloc(4096);
}

void StorageMemoryTest::resizeFile()
{
    QCOMPARE(m_storage->allocateMemory(0), true);
    QCOMPARE(m_storage->allocateMemory(123), false);
    QCOMPARE(m_storage->allocateMemory(16*4096), true);
}

void StorageMemoryTest::getSizeOfFile()
{
    QCOMPARE((int)m_storage->getFileSize(false), 16*4096);
    QCOMPARE((int)m_storage->getFileSize(true), 16*4096);
}

void StorageMemoryTest::writeFile()
{
    ((quint8*)m_buffer)[0] = 7;
    ((quint8*)m_buffer)[16] = 2;
    QCOMPARE(m_storage->writeBlock(0, m_buffer, 123,0),false);
    QCOMPARE(m_storage->writeBlock(0, nullptr, 123,0), false);
    QCOMPARE(m_storage->writeBlock(0, m_buffer, 4096,0), true);
    QCOMPARE(m_storage->writeBlock(2*4096, m_buffer, 4096,0), true);
}

void StorageMemoryTest::readFile()
{
    memset(m_buffer, 0, 4096);
    QCOMPARE(m_storage->readBlock(0, nullptr, 4096,0), false);
    QCOMPARE(m_storage->readBlock(0, m_buffer, 123,0), false);
    QCOMPARE(m_storage->readBlock(0, m_buffer, 4096,0), true);

    QCOMPARE((int)((quint8*)m_buffer)[0], 7);
    QCOMPARE((int)((quint8*)m_buffer)[16], 2);

    memset(m_buffer, 0, 4096);
    QCOMPARE(m_storage->readBlock(2*4096, m_buffer, 4096,0), true);
    QCOMPARE((int)((quint8*)m_buffer)[0], 7);
    QCOMPARE((int)((quint8*)m_buffer)[16], 2);
}

void StorageMemoryTest::reopenFile()
{
    QCOMPARE(m_storage->closeFile(), true);
    delete m_storage;
    m_storage = new Persistence::StorageMemory("/tmp/cluster0");

    QCOMPARE(m_storage->readBlock(0, nullptr, 4096,0), false);
    QCOMPARE(m_storage->readBlock(0, m_buffer, 123,0), false);
    QCOMPARE(m_storage->readBlock(0, m_buffer, 4096,0), true);

    QCOMPARE((int)((quint8*)m_buffer)[0], 7);
    QCOMPARE((int)((quint8*)m_buffer)[16], 2);
}

void StorageMemoryTest::cleanupTestCase()
{
    delete m_storage;
    QFile::remove("/tmp/cluster0");
}

}
