QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Personalmacro
TEMPLATE = app

CONFIG += static

HEADERS  += mainwindow.h \
    commands.h\
    keyboardutilities.h \
    defaultdelaywidget.h \
    CmdWidgets/cmdwidget.h \
    CmdWidgets/delaycmdwidget.h \
    CmdWidgets/clickcmdwidget.h \
    CmdWidgets/dragcmdwidget.h \
    CmdWidgets/scrollcmdwidget.h \
    CmdWidgets/setcursorposcmdwidget.h \
    CmdWidgets/writetextcmdwidget.h \
    CmdWidgets/runexecmdwidget.h \
    CmdWidgets/presskeycmdwidget.h

SOURCES += main.cpp \
    mainwindow.cpp \
    commands.cpp \
    keyboardutilities.cpp \
    defaultdelaywidget.cpp \
    CmdWidgets/cmdwidget.cpp \
    CmdWidgets/delaycmdwidget.cpp \
    CmdWidgets/clickcmdwidget.cpp \
    CmdWidgets/dragcmdwidget.cpp \
    CmdWidgets/scrollcmdwidget.cpp \
    CmdWidgets/setcursorposcmdwidget.cpp \
    CmdWidgets/writetextcmdwidget.cpp \
    CmdWidgets/runexecmdwidget.cpp \
    CmdWidgets/presskeycmdwidget.cpp

FORMS    += mainwindow.ui \
    defaultdelaywidget.ui \
    CmdWidgets/cmdwidget.ui \
    CmdWidgets/delaycmdwidget.ui \
    CmdWidgets/clickcmdwidget.ui \
    CmdWidgets/dragcmdwidget.ui \
    CmdWidgets/scrollcmdwidget.ui \
    CmdWidgets/setcursorposcmdwidget.ui \
    CmdWidgets/writetextcmdwidget.ui \
    CmdWidgets/runexecmdwidget.ui \
    CmdWidgets/presskeycmdwidget.ui

RESOURCES += \
    resources.qrc
