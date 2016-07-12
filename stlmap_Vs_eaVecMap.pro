TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

CONFIG+=c++11

unix|win32: LIBS += -L/usr/local/lib -lEASTL -lstdc++


