QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Personalmacro
TEMPLATE = app

CONFIG += static

HEADERS  += mainwindow.h \
    commands.h\
    keyboardutilities.h \
    CmdWidgets/cmdwidget.h \
    CmdWidgets/clickcmdwidget.h \
    CmdWidgets/dragcmdwidget.h \
    CmdWidgets/scrollcmdwidget.h \
    CmdWidgets/setcursorposcmdwidget.h \
    CmdWidgets/waitcmdwidget.h

SOURCES += main.cpp \
        mainwindow.cpp \
        commands.cpp \
        keyboardutilities.cpp \
    CmdWidgets/cmdwidget.cpp \
    CmdWidgets/clickcmdwidget.cpp \
    CmdWidgets/dragcmdwidget.cpp \
    CmdWidgets/scrollcmdwidget.cpp \
    CmdWidgets/waitcmdwidget.cpp \
    CmdWidgets/setcursorposcmdwidget.cpp

FORMS    += mainwindow.ui \
    CmdWidgets/cmdwidget.ui \
    CmdWidgets/clickcmdwidget.ui \
    CmdWidgets/dragcmdwidget.ui \
    CmdWidgets/scrollcmdwidget.ui \
    CmdWidgets/setcursorposcmdwidget.ui \
    CmdWidgets/waitcmdwidget.ui

RESOURCES += \
    resources.qrc
