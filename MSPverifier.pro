QT += core
QT -= gui

CONFIG += c++11

TARGET = MSPverifier
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    src/Logger.cpp \
    src/FileList.cpp \
    src/Line.cpp \
    src/LineContainer.cpp \
    src/File.cpp \
    src/dyskryminator.cpp \
    src/dyskryminatorSM.cpp \
    src/macro.cpp \
    src/segment.cpp \
    src/segmentList.cpp \
    src/fun.cpp \
    src/funContainer.cpp \
    src/core.cpp \
    src/reg.cpp \
    src/stack.cpp \
    src/conv.cpp \
    src/convs.cpp \
    src/MSPVerifier.cpp

LIBS += -L"$$_PRO_FILE_PWD_/src/lib/" -lpugixmlw
LIBS += -L"$$_PRO_FILE_PWD_/src/lib/" -lpugixml

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/Logger.hpp \
    src/FileList.hpp \
    src/Line.hpp \
    src/osrc/pugiconfig.hpp \
    src/osrc/pugixml.hpp \
    src/LineContainer.hpp \
    src/File.hpp \
    src/dyskryminator.hpp \
    src/dyskryminatorSM.hpp \
    src/macro.hpp \
    src/segment.hpp \
    src/segmentList.hpp \
    src/fun.hpp \
    src/funContainer.hpp \
    src/core.hpp \
    src/reg.hpp \
    src/stack.hpp \
    src/conv.hpp \
    src/convs.hpp \
    src/MSPVerifier.hpp

RESOURCES += \
    res.qrc
