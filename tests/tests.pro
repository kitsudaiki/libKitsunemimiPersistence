include(../defaults.pri)

QT -= qt core gui

CONFIG   -= app_bundle
CONFIG += c++14 console

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include

LIBS +=  -lboost_filesystem -lboost_system

INCLUDEPATH += $$PWD

LIBS += -L../src -lKitsunePersistence

SOURCES += \
    main.cpp \
    libKitsunePersistence/files/text_file_test.cpp \
    libKitsunePersistence/database/sqlite_test.cpp \
    libKitsunePersistence/logger/logger_test.cpp \
    libKitsunePersistence/files/binary_file_with_directIO_test.cpp \
    libKitsunePersistence/files/binary_file_without_directIO_test.cpp

HEADERS += \
    libKitsunePersistence/files/text_file_test.h \
    libKitsunePersistence/database/sqlite_test.h \
    libKitsunePersistence/logger/logger_test.h \
    libKitsunePersistence/files/binary_file_with_directIO_test.h \
    libKitsunePersistence/files/binary_file_without_directIO_test.h
