QT       -= qt core gui

TARGET = KitsunePersistence
TEMPLATE = lib
CONFIG += c++14

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

LIBS +=  -lboost_filesystem -lboost_system

INCLUDEPATH += $$PWD \
            ../include/libKitsunePersistence

SOURCES += \
        files/textFile.cpp \
    files/binaryFile.cpp

HEADERS += \
        ../include/libKitsunePersistence/files/textFile.hpp \
    ../include/libKitsunePersistence/files/binaryFile.hpp
unix {
    target.path = /usr/lib
    INSTALLS += target
}
