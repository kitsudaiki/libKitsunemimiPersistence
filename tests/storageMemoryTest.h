#ifndef STORAGEMEMORYTEST_H
#define STORAGEMEMORYTEST_H

#include <QObject>
#include <QString>
#include <QtTest>
#include <QFile>
#include <files/storageMemory.h>

#include <persistence_global.h>

namespace Persistence
{

class PERSISTENCESHARED_EXPORT StorageMemoryTest : public QObject
{
    Q_OBJECT

public:
    StorageMemoryTest();

private slots:
    void initTestCase();
    void resizeFile();
    void getSizeOfFile();
    void writeFile();
    void readFile();
    void reopenFile();
    void cleanupTestCase();

private:
    Persistence::StorageMemory *m_storage = nullptr;
    void *m_buffer = nullptr;

    void* aligned_malloc(const quint32 size);
    bool aligned_free(void *ptr);
};

}

#endif // STORAGEMEMORYTEST_H
