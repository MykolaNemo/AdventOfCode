TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += /std:c++17

SOURCES += \
        main.cpp

DISTFILES += \
    input.txt
