#include "commands.h"

#include <windows.h>
#include <QProcess>
#include <QDebug>

#include "keyboardutilities.h"
#include "CmdWidgets/clickcmdwidget.h"
#include "CmdWidgets/delaycmdwidget.h"
#include "CmdWidgets/dragcmdwidget.h"
#include "CmdWidgets/presskeycmdwidget.h"
#include "CmdWidgets/runexecmdwidget.h"
#include "CmdWidgets/scrollcmdwidget.h"
#include "CmdWidgets/setcursorposcmdwidget.h"
#include "CmdWidgets/writetextcmdwidget.h"

#define SCROLLUP 120
#define SCROLLDOWN -120

void Commands::ExecuteCmd(QString cmd)
{
    QStringList cmdParts = cmd.split("|");
    CmdType type = static_cast<CmdType>(cmdParts[CmdWidget::CmdTypeIdx].toInt());

    switch(type)
    {
        case CmdType::DELAY:
            Delay(cmdParts); break;
        case CmdType::GOTO:
            break;
        case CmdType::CLICK:
            Click(cmdParts); break;
        case CmdType::CURPOS:
            CursorPos(cmdParts); break;
        case CmdType::DRAG:
            Drag(cmdParts); break;
        case CmdType::SCROLL:
            Scroll(cmdParts); break;
        case CmdType::PRESSKEY:
            PressKey(cmdParts); break;
        case CmdType::WRITETEXT:
            WriteText(cmdParts); break;
        case CmdType::RUNEXE:
            RunExe(cmdParts); break;
    }
}

void Commands::Delay(QStringList cmd)
{
    DWORD amount = DWORD(cmd[DelayCmdWidget::AmountIdx].toInt());
    DWORD timeScale = (cmd[DelayCmdWidget::ScaleIdx] == "0" ? 1000 : 1);
    Sleep(amount * timeScale);
}

void Commands::Click(QStringList cmd)
{
    ClickType clickType = static_cast<ClickType>(cmd[ClickCmdWidget::TypeIdx].toInt());

    for(int i = 0, amount = cmd[ClickCmdWidget::AmountIdx].toInt(); i < amount; ++i)
    {
        switch(clickType)
        {
        case(ClickType::Left):
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        }
        case(ClickType::Right):
        {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            break;
        }
        case(ClickType::Middle):
        {
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
            break;
        }
        case(ClickType::DoubleLeft):
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        }
        }
    }
}

void Commands::CursorPos(QStringList cmd)
{
    SetCursorPos(cmd[SetCursorPosCmdWidget::XIdx].toInt(), cmd[SetCursorPosCmdWidget::YIdx].toInt());
}

void Commands::Drag(QStringList cmd)
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(10);
    SetCursorPos(cmd[DragCmdWidget::XIdx].toInt(), cmd[DragCmdWidget::YIdx].toInt());
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Commands::Scroll(QStringList cmd)
{
    DWORD direction = (cmd[ScrollCmdWidget::DirIdx] == "0" ? DWORD(SCROLLUP) : DWORD(SCROLLDOWN));
    for(int i = 0, amount = cmd[ScrollCmdWidget::AmountIdx].toInt(); i <= amount; ++i)
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, direction, 0);
        Sleep(10);
    }
}

void Commands::PressKey(QStringList cmd)
{
    bool ctrl = cmd[PressKeyCmdWidget::ModCTRLIdx].toInt();
    bool shift = cmd[PressKeyCmdWidget::ModSHIFTIdx].toInt();
    bool alt = cmd[PressKeyCmdWidget::ModALTIdx].toInt();
    KeyType keyType = KeyType(cmd[PressKeyCmdWidget::KeyTypeIdx].toInt());
    QString letter = cmd[PressKeyCmdWidget::LetterIdx];
    QString keySequLetter = cmd[PressKeyCmdWidget::SeqLetterIdx];
    int specialKeyIndex = cmd[PressKeyCmdWidget::SpcKeyIndexIdx].toInt();

    if(ctrl)
        KeyboardUtilities::PressSpecialKey("Ctrl");
    if(shift)
        KeyboardUtilities::PressSpecialKey("Shift");
    if(alt)
        KeyboardUtilities::PressSpecialKey("Alt");

    if(keyType == KeyType::LETTER)
    {
        KeyboardUtilities::WriteText(letter.toStdString());
    }
    else if(keyType == KeyType::KEYDET)
    {
        KeyboardUtilities::PARSpecialKey(keySequLetter.toStdString());
    }
    else if(keyType == KeyType::SPECKEY)
    {
        if(specialKeyIndex == 1)
            KeyboardUtilities::PARSpecialKey("Tab");
        else if(specialKeyIndex == 1)
            KeyboardUtilities::PARSpecialKey("Print");
    }

    if(ctrl)
        KeyboardUtilities::ReleaseSpecialKey("Ctrl");
    if(shift)
        KeyboardUtilities::ReleaseSpecialKey("Shift");
    if(alt)
        KeyboardUtilities::ReleaseSpecialKey("Alt");
}

void Commands::WriteText(QStringList cmd)
{
    if(cmd[WriteTextCmdWidget::TypeIdx].toInt() == 0)
        KeyboardUtilities::WriteText(cmd[WriteTextCmdWidget::TextIdx].toStdString());
    else
    {
        QString possibleChars = cmd[WriteTextCmdWidget::CharsIdx];
        int high = possibleChars.count();

        QString text = "";
        for(int i = 0, total = cmd[WriteTextCmdWidget::AmountIdx].toInt(); i < total; ++i)
        {
            int randIndex = qrand() % high;
            text.append(possibleChars.at(randIndex));
        }
        KeyboardUtilities::WriteText(text.toStdString());
    }
}

void Commands::RunExe(QStringList cmd)
{
    WinExec(cmd[RunExeCmdWidget::PathIdx].toUtf8(), SW_NORMAL);
}
