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
        progwindow.cpp

HEADERS  += mainwindow.h \
    startup.h \
    optionsdialog.h \
    commandwidget.h \
    clickcommandwidget.h \
    newprogramdialog.h \
    newcommanddialog.h \
    keyboardutilities.h \
    progwindow.h

FORMS    += mainwindow.ui \
    startup.ui \
    optionsdialog.ui \
    commandwidget.ui \
    clickcommandwidget.ui \
    newprogramdialog.ui \
    newcommanddialog.ui \
    progwindow.ui

RESOURCES += \
    resources.qrc
