TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

QMAKE_CXXFLAGS += -std=gnu++1y

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    bts.h \
    threadedbts.h

