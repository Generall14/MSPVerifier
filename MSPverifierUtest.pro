TEMPLATE = app
CONFIG += console c++14 core qt
CONFIG -= app_bundle

SOURCES += \ 
    utests/FunUtest.cpp \
    src/conv.cpp \
    src/convs.cpp \
    src/core.cpp \
    src/dyskryminator.cpp \
    src/dyskryminatorSM.cpp \
    src/File.cpp \
    src/FileList.cpp \
    src/fun.cpp \
    src/funContainer.cpp \
    src/Line.cpp \
    src/LineContainer.cpp \
    src/Logger.cpp \
    src/macro.cpp \
    src/MSPVerifier.cpp \
    src/reg.cpp \
    src/segment.cpp \
    src/segmentList.cpp \
    src/stack.cpp \
    utests/FunMoreUtest.cpp

LIBS += -lpugixml
LIBS += -lgmock_main -lgtest -lgmock -lpthread

HEADERS += \ 
    src/conv.hpp \
    src/convs.hpp \
    src/core.hpp \
    src/dyskryminator.hpp \
    src/dyskryminatorSM.hpp \
    src/File.hpp \
    src/FileList.hpp \
    src/fun.hpp \
    src/funContainer.hpp \
    src/Line.hpp \
    src/LineContainer.hpp \
    src/Logger.hpp \
    src/macro.hpp \
    src/MSPVerifier.hpp \
    src/reg.hpp \
    src/segment.hpp \
    src/segmentList.hpp \
    src/stack.hpp
