include(../defaults.pri)

QT -= qt core gui

CONFIG   -= app_bundle
CONFIG += c++14 console

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

LIBS +=  -lboost_filesystem -lboost_system

INCLUDEPATH += $$PWD

LIBS += -L../src -lKitsunePersistence

SOURCES += \
    main.cpp \
    files/binaryFile_test.cpp \
    files/textFile_test.cpp

HEADERS += \
    files/binaryFile_test.hpp \
    files/textFile_test.hpp
