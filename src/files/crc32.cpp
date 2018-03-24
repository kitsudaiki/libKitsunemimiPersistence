#include <files/crc32.h>

#include <QFile>

namespace Persistence
{

/**
 * @brief Crc32::Crc32
 */
Crc32::Crc32()
{
    quint32 crc;

    for (int i = 0; i < 256; i++)
    {
        crc = i;
        for (int j = 0; j < 8; j++)
        {
            if(crc & 1)
            {
               crc = (crc >> 1) ^ 0xEDB88320UL;
            }
            else
            {
                crc = crc >> 1;
            }
        }

        crc_table[i] = crc;
    }
}

/**
 * @brief Crc32::calculateFromFile
 * @param filename
 * @return
 */
QString Crc32::calculateFromFile(const QString filename)
{
    QString outputString = "00000000";

    quint32 crc = 0xFFFFFFFFUL;
    char buffer[16000];

    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            int len = file.read(buffer, 16000);
            for(int i = 0; i < len; i++)
            {
                crc = crc_table[(crc ^ buffer[i]) & 0xFF] ^ (crc >> 8);
            }
        }
        file.close();
    }

    crc = crc ^ 0xFFFFFFFFUL;

    QString tempString = QString::number(crc, 16);
    tempString = tempString.toUpper();
    for(int i = 0; i < tempString.size(); i++)
    {
        outputString[outputString.size()-(1+i)] = tempString[tempString.size()-(1+i)];
    }
    return outputString;
}

}
