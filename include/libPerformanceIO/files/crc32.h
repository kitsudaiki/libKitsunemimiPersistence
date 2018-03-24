#ifndef CRC32_H
#define CRC32_H

#include <QtCore>
#include <QString>
#include <QMap>

namespace Persistence
{

class Crc32
{
private:
    quint32 crc_table[256];

public:
    Crc32();

    QString calculateFromFile(const QString filename);

};

}

#endif // CRC32_H
