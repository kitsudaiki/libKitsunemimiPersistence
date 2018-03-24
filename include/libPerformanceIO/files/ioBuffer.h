#ifndef RAMMEMORY_H
#define RAMMEMORY_H

#include <QObject>

namespace Persistence
{
class StorageMemory;

class IOBuffer
{
public:
    IOBuffer(const QString filePath);
    ~IOBuffer();

    bool initBuffer();
    bool closeBuffer(const bool withoutStorage = false);

    bool allocateBlocks(const quint32 numberOfBlocks, const bool withoutStorage = false);

    quint32 getNumberOfBlocks() const;
    quint32 getBlockSize() const;

    void *getBufferPointer();
    void *getBlock(const quint32 blockNumber);

    bool syncAll();
    bool syncBlocks(const quint32 blockNumber,
                    const quint32 endBlockNumber);


private:
    quint32 m_size = 0;
    quint32 m_storagePosition = 0;
    Persistence::StorageMemory* m_storage = nullptr;

    void *m_buffer = nullptr;

    void* aligned_malloc(const quint32 numberOfBytes);
    bool aligned_free(void *ptr);
};

}

#endif // RAMMEMORY_H
