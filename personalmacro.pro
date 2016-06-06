QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Personalmacro
TEMPLATE = app

CONFIG += static

SOURCES += main.cpp \
        mainwindow.cpp \
        startup.cpp \
        optionsdialog.cpp \
        commandwidget.cpp \
        clickcommandwidget.cpp \
        newprogramdialog.cpp \
        newcommanddialog.cpp \
        progwindow.cpp \
    setcursorposcommandwidget.cpp \
    commandlistitem.cpp \
    waitcommandwidget.cpp

HEADERS  += mainwindow.h \
    startup.h \
    optionsdialog.h \
    commandwidget.h \
    clickcommandwidget.h \
    newprogramdialog.h \
    newcommanddialog.h \
    keyboardutilities.h \
    progwindow.h \
    commands.h \
    setcursorposcommandwidget.h \
    commandlistitem.h \
    waitcommandwidget.h

FORMS    += mainwindow.ui \
    startup.ui \
    optionsdialog.ui \
    commandwidget.ui \
    clickcommandwidget.ui \
    newprogramdialog.ui \
    newcommanddialog.ui \
    progwindow.ui \
    setcursorposcommandwidget.ui \
    waitcommandwidget.ui

RESOURCES += \
    resources.qrc
