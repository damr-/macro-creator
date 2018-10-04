QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Macrocreator
TEMPLATE = app

CONFIG += static

RC_FILE = app.rc

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
    CmdWidgets/presskeycmdwidget.h \
    CmdWidgets/gotocmdwidget.h \
    poshint.h \
    version.h \
    CmdWidgets/applyregexcmdwidget.h

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
    CmdWidgets/presskeycmdwidget.cpp \
    CmdWidgets/gotocmdwidget.cpp \
    poshint.cpp \
    CmdWidgets/applyregexcmdwidget.cpp

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
    CmdWidgets/presskeycmdwidget.ui \
    CmdWidgets/gotocmdwidget.ui \
    poshint.ui \
    CmdWidgets/applyregexcmdwidget.ui

RESOURCES += \
    resources.qrc
