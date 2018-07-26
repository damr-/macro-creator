#include "commands.h"

#include <windows.h>
#include <QProcess>
#include <QDebug>

#include "keyboardutilities.h"
#include "CmdWidgets/clickcmdwidget.h"
#include "CmdWidgets/delaycmdwidget.h"
#include "CmdWidgets/dragcmdwidget.h"
#include "CmdWidgets/hitkeycmdwidget.h"
#include "CmdWidgets/runexecmdwidget.h"
#include "CmdWidgets/scrollcmdwidget.h"
#include "CmdWidgets/setcursorposcmdwidget.h"
#include "CmdWidgets/writetextcmdwidget.h"

#define SCROLLUP 120
#define SCROLLDOWN -120

void Commands::ExecuteCommand(QString command)
{
    QStringList cmd = command.split("|");
    CmdType type = static_cast<CmdType>(cmd[CmdWidget::CmdTypeIdx].toInt());

    switch(type)
    {
        case CmdType::DELAY:
            Delay(cmd); break;
        case CmdType::CLICK:
            Click(cmd); break;
        case CmdType::CURPOS:
            CursorPos(cmd); break;
        case CmdType::DRAG:
            Drag(cmd); break;
        case CmdType::SCROLL:
            Scroll(cmd); break;
        case CmdType::HITKEY:
            HitKey(cmd); break;
        case CmdType::WRITETEXT:
            WriteText(cmd); break;
        case CmdType::RUNEXE:
            RunExe(cmd); break;
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

void Commands::HitKey(QStringList cmd)
{
    if(cmd[HitKeyCmdWidget::SpcKeyIdx] == "1")
    {
        switch(cmd[HitKeyCmdWidget::SpcKeyTypeIdx].toInt())
        {
        case 1:
            keybd_event(VK_MENU, 0xB8, 0, 0);
            KeyboardUtilities::hitSpecialKey("Tab");
            keybd_event(VK_MENU, 0xB8, KEYEVENTF_KEYUP, 0);
            break;
        case 2:
            keybd_event(VK_MENU, 0xB8, 0, 0);
            KeyboardUtilities::hitSpecialKey("F4");
            keybd_event(VK_MENU, 0xB8, KEYEVENTF_KEYUP, 0);
            break;
        }
    }
    else
    {
        QKeySequence keySequence = QKeySequence::fromString(cmd[HitKeyCmdWidget::KeySeqIdx]);
        QString s = keySequence.toString();

        QStringList parts = s.split('+');

        QString part;
        foreach(part, parts)
        {
            if(part.length() == 1 && part.at(0).isLetter())
            {
                char letter = part.at(0).toLatin1();
                KeyboardUtilities::hitKey(letter);
            }
            else //It's a special key
            {
                KeyboardUtilities::pressSpecialKey(part.toStdString());
            }
        }
        //Release the special keys
        foreach(part, parts)
        {
            if(part.length() != 1)
                KeyboardUtilities::releaseSpecialKey(part.toStdString());
        }
    }
}

void Commands::WriteText(QStringList cmd)
{
    if(cmd[WriteTextCmdWidget::IsRandomIdx].toInt() == 0)
        KeyboardUtilities::writeText(cmd[WriteTextCmdWidget::TextIdx].toStdString());
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
        KeyboardUtilities::writeText(text.toStdString());
    }
}

void Commands::RunExe(QStringList cmd)
{
    WinExec(cmd[RunExeCmdWidget::PathIdx].toUtf8(), SW_NORMAL);
}
