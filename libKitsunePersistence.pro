QT       -= qt core gui

TARGET = KitsunePersistence
TEMPLATE = lib
CONFIG += c++14

LIBS += -L../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../libKitsuneCommon/include/libKitsuneCommon

LIBS +=  -lboost_filesystem -lboost_system -lboost_program_options -lboost_iostreams

INCLUDEPATH += $$PWD \
            include/libKitsunePersistence

SOURCES += \
        src/files/storageMemory.cpp \
    include/libKitsunePersistence/files/textFile.cpp

HEADERS += \
        include/libKitsunePersistence/files/storageMemory.h \
    include/libKitsunePersistence/files/textFile.hpp
unix {
    target.path = /usr/lib
    INSTALLS += target
}
