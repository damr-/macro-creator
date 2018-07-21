#include "commands.h"

#include <windows.h>
#include <QProcess>

#include "keyboardutilities.h"
#include "CmdWidgets/clickcmdwidget.h"

#define SCROLLUP 120
#define SCROLLDOWN -120

void Commands::ExecuteCommand(QString command)
{
    QStringList cmd = command.split("|");
    CmdType type = static_cast<CmdType>(cmd[0].toInt());

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
    DWORD amount = DWORD(cmd[1].toInt());
    DWORD timeScale = (cmd[2] == "0" ? 1000 : 1);
    Sleep(amount * timeScale);
}

void Commands::Click(QStringList cmd)
{
    ClickType clickType = static_cast<ClickType>(cmd[2].toInt());

    for(int i = 0, amount = cmd[1].toInt(); i < amount; i++)
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
    SetCursorPos(cmd[1].toInt(), cmd[2].toInt());
}

void Commands::Drag(QStringList cmd)
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(10);
    SetCursorPos(cmd[1].toInt(), cmd[2].toInt());
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Commands::Scroll(QStringList cmd)
{
    DWORD direction = (cmd[2] == "0" ? DWORD(SCROLLUP) : DWORD(SCROLLDOWN));
    for(int i = 0, amount = cmd[1].toInt(); i <= amount; i++)
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, direction, 0);
        Sleep(10);
    }
}

void Commands::HitKey(QStringList cmd)
{
//    for(int i = 0; i < cmd[6].toInt(); ++i)
//    {
//        if(cmd[1] == "1")
//            keybd_event(VK_CONTROL, 0x9D, 0, 0);
//        if(cmd[2] == "1")
//            keybd_event(VK_MENU, 0xB8, 0, 0);
//        if(cmd[3] == "1")
//            keybd_event(VK_SHIFT, 0xAA, 0, 0);

//        if(cmd[4] == "1")
//            KeyboardUtilities::writeText(cmd[5].toStdString());
//        else
//            KeyboardUtilities::pressVK(cmd[5].toStdString());

//        if(cmd[1] == "1")
//            keybd_event(VK_CONTROL, 0x9D, KEYEVENTF_KEYUP, 0);
//        if(cmd[2] == "1")
//            keybd_event(VK_MENU, 0xB8, KEYEVENTF_KEYUP, 0);
//        if(cmd[3] == "1")
//            keybd_event(VK_SHIFT, 0xAA, KEYEVENTF_KEYUP, 0);
//    }
    QList<QKeySequence> keySequence = QKeySequence::listFromString(cmd[1]);
    //TODO
    Sleep(50);
}

void Commands::WriteText(QStringList cmd)
{
    KeyboardUtilities::writeText(cmd[1].toStdString());
}

void Commands::RunExe(QStringList cmd)
{
    WinExec(cmd[1].toUtf8(), SW_NORMAL);
}
