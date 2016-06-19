QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Personalmacro
TEMPLATE = app

CONFIG += static

HEADERS  += mainwindow.h \
    commandwidget.h \
    clickcommandwidget.h \
    setcursorposcommandwidget.h \
    waitcommandwidget.h \
    commands.h\
    keyboardutilities.h \
    dragcommandwidget.h

SOURCES += main.cpp \
        mainwindow.cpp \
        commandwidget.cpp \
        clickcommandwidget.cpp \
    setcursorposcommandwidget.cpp \
    waitcommandwidget.cpp \
    commands.cpp \
    keyboardutilities.cpp \
    dragcommandwidget.cpp

FORMS    += mainwindow.ui \
    commandwidget.ui \
    clickcommandwidget.ui \
    setcursorposcommandwidget.ui \
    waitcommandwidget.ui \
    dragcommandwidget.ui

RESOURCES += \
    resources.qrc
