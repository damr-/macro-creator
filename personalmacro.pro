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
    defaultdelaywidget.h \
    CmdWidgets/delaycmdwidget.h \
    CmdWidgets/hitkeycmdwidget.h \
    CmdWidgets/writetextcmdwidget.h

SOURCES += main.cpp \
        mainwindow.cpp \
        commands.cpp \
        keyboardutilities.cpp \
    CmdWidgets/cmdwidget.cpp \
    CmdWidgets/clickcmdwidget.cpp \
    CmdWidgets/dragcmdwidget.cpp \
    CmdWidgets/scrollcmdwidget.cpp \
    CmdWidgets/setcursorposcmdwidget.cpp \
    defaultdelaywidget.cpp \
    CmdWidgets/delaycmdwidget.cpp \
    CmdWidgets/hitkeycmdwidget.cpp \
    CmdWidgets/writetextcmdwidget.cpp

FORMS    += mainwindow.ui \
    CmdWidgets/cmdwidget.ui \
    CmdWidgets/clickcmdwidget.ui \
    CmdWidgets/dragcmdwidget.ui \
    CmdWidgets/scrollcmdwidget.ui \
    CmdWidgets/setcursorposcmdwidget.ui \
    defaultdelaywidget.ui \
    CmdWidgets/delaycmdwidget.ui \
    CmdWidgets/hitkeycmdwidget.ui \
    CmdWidgets/writetextcmdwidget.ui

RESOURCES += \
    resources.qrc
