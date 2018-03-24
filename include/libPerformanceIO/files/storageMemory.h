#ifndef SSDMEMORY_H
#define SSDMEMORY_H

#include <QObject>
#include <deque>
#include <sstream>
#include <mutex>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <QDebug>

#include <persistence_global.h>

namespace Persistence
{

class PERSISTENCESHARED_EXPORT StorageMemory
{
public:
    StorageMemory(const QString filePath);
    ~StorageMemory();

    bool allocateMemory(const quint32 size);
    quint32 getFileSize(const bool makeCheck = false);

    bool readBlock(const quint32 storagePosition,
                   void *buffer,
                   const quint32 bufferSize,
                   const quint32 bufferOffset = 0);
    bool writeBlock(const quint32 storagePosition,
                    void *buffer,
                    const quint32 bufferSize,
                    const quint32 bufferOffset = 0);
    bool closeFile();

private:
    QString m_filePath = "";
    quint32 m_fileSize = 0;
    int m_fileDescriptor = -1;

    void initFile();
};

}

#endif // SSDMEMORY_H
