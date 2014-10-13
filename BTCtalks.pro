# BTCtalks Qt project file.

QT       += core
QT       += network
QT       -= gui

QMAKE_CXXFLAGS += -std=c++0x

TARGET = BTCTalks
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    DownloadManager.cpp \
    btctalks.cpp

HEADERS += \
    DownloadManager.h \
    btctalks.h
