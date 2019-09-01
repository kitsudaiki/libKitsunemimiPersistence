QT       -= qt core gui

TARGET = KitsunePersistence
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.2.0

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

LIBS +=  -lboost_filesystem -lboost_system -lsqlite3

INCLUDEPATH += $$PWD \
            ../include/libKitsunePersistence

SOURCES += \
    files/binary_file.cpp \
    files/text_file.cpp \
    database/sqlite.cpp

HEADERS += \
    ../include/libKitsunePersistence/files/binary_file.h \
    ../include/libKitsunePersistence/files/text_file.h \
    ../include/libKitsunePersistence/database/sqlite.h

