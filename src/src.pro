QT       -= qt core gui

TARGET = KitsunemimiPersistence
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.5.0

LIBS += -L../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../libKitsunemimiCommon/include

LIBS +=  -lboost_filesystem -lboost_system -lsqlite3

INCLUDEPATH += $$PWD \
            ../include

SOURCES += \
    files/binary_file.cpp \
    files/text_file.cpp \
    database/sqlite.cpp \
    logger/logger.cpp

HEADERS += \
    ../include/libKitsunemimiPersistence/files/binary_file.h \
    ../include/libKitsunemimiPersistence/files/text_file.h \
    ../include/libKitsunemimiPersistence/database/sqlite.h \
    ../include/libKitsunemimiPersistence/logger/logger.h

