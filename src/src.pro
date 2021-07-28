QT       -= qt core gui

TARGET = KitsunemimiPersistence
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.10.2

LIBS += -L../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../libKitsunemimiCommon/include

LIBS +=  -lboost_filesystem -lboost_system

with_sqlite {
    LIBS += -lsqlite3
}

INCLUDEPATH += $$PWD \
               $$PWD/../include

SOURCES += \
    files/binary_file.cpp \
    files/text_file.cpp \
    logger/logger.cpp \
    files/file_methods.cpp

with_sqlite {
    SOURCES += database/sqlite.cpp
}

HEADERS += \
    ../include/libKitsunemimiPersistence/files/binary_file.h \
    ../include/libKitsunemimiPersistence/files/text_file.h \
    ../include/libKitsunemimiPersistence/logger/logger.h \
    ../include/libKitsunemimiPersistence/files/file_methods.h

with_sqlite {
    HEADERS += ../include/libKitsunemimiPersistence/database/sqlite.h 
}
