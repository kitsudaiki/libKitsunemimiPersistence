QT       -= qt core gui

TARGET = KitsunePersistence
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.5.0

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include

LIBS +=  -lboost_filesystem -lboost_system -lsqlite3

INCLUDEPATH += $$PWD \
            ../include

SOURCES += \
    files/binary_file.cpp \
    files/text_file.cpp \
    database/sqlite.cpp \
    logger/logger.cpp

HEADERS += \
    ../include/libKitsunePersistence/files/binary_file.h \
    ../include/libKitsunePersistence/files/text_file.h \
    ../include/libKitsunePersistence/database/sqlite.h \
    ../include/libKitsunePersistence/logger/logger.h

