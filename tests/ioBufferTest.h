#ifndef IOBUFFERTEST_H
#define IOBUFFERTEST_H

#include <QObject>
#include <QString>
#include <QtTest>
#include <QFile>
#include <files/ioBuffer.h>

#include <persistence_global.h>

namespace Persistence
{

class PERSISTENCESHARED_EXPORT IoBufferTest : public QObject
{
    Q_OBJECT

public:
    IoBufferTest();

private slots:
    void initTestCase();
    void getNumberOfBlocksAfterInit();
    void allocateBlocks();
    void getNumberOfBlocksAfterResize();
    void writeToBuffer();
    void readFromBuffer();
    void closeAndReopen();
    void readFromBufferAfterReopen();
    void closeAndReopenWithoutFile();
    void cleanupTestCase();

private:
     IOBuffer *m_buffer = nullptr;
};

}

#endif // IOBUFFERTEST_H
