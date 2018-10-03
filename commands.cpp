#include "commands.h"

#include <QProcess>

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
        case CmdType::GOTO: break;
        case CmdType::CLICK:
            Click(cmdParts); break;
        case CmdType::SETCURSORPOS:
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
                Click(MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
                break;
            case(ClickType::Right):
                Click(MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
                break;
            case(ClickType::Middle):
                Click(MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);
                break;
            case(ClickType::DoubleLeft):
                Click(MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
                Sleep(INTERNAL_DELAY);
                Click(MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
                break;
        }
        Sleep(INTERNAL_DELAY);
    }
}

void Commands::Click(DWORD buttonDown, DWORD buttonUp)
{
    mouse_event(buttonDown, 0, 0, 0, 0);
    Sleep(INTERNAL_DELAY);
    mouse_event(buttonUp, 0, 0, 0, 0);
}

void Commands::CursorPos(QStringList cmd)
{
    SetCursorPos(cmd[SetCursorPosCmdWidget::XIdx].toInt(), cmd[SetCursorPosCmdWidget::YIdx].toInt());
    Sleep(INTERNAL_DELAY);
    if(cmd[SetCursorPosCmdWidget::ClickIdx].toInt())
        Click(MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
}

void Commands::Drag(QStringList cmd)
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(INTERNAL_DELAY);
    SetCursorPos(cmd[DragCmdWidget::XIdx].toInt(), cmd[DragCmdWidget::YIdx].toInt());
    Sleep(INTERNAL_DELAY);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Commands::Scroll(QStringList cmd)
{
    DWORD direction = (cmd[ScrollCmdWidget::DirIdx] == "0" ? DWORD(SCROLLUP) : DWORD(SCROLLDOWN));
    for(int i = 0, amount = cmd[ScrollCmdWidget::AmountIdx].toInt(); i <= amount; ++i)
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, direction, 0);
        Sleep(INTERNAL_DELAY);
    }
}

void Commands::PressKey(QStringList cmd)
{
    bool ctrl = cmd[PressKeyCmdWidget::ModCTRLIdx].toInt();
    bool shift = cmd[PressKeyCmdWidget::ModSHIFTIdx].toInt();
    bool alt = cmd[PressKeyCmdWidget::ModALTIdx].toInt();
    KeyType keyType = KeyType(cmd[PressKeyCmdWidget::KeyTypeIdx].toInt());
    QString keySequLetter = cmd[PressKeyCmdWidget::SeqLetterIdx];
    int specialKeyIndex = cmd[PressKeyCmdWidget::SpcKeyIndexIdx].toInt();

    if(ctrl)
        KeyboardUtilities::PressSpecialKey("Ctrl");
    if(shift)
        KeyboardUtilities::PressSpecialKey("Shift");
    if(alt)
        KeyboardUtilities::PressSpecialKey("Alt");

    Sleep(INTERNAL_DELAY);

    if(keyType == KeyType::KEY)
    {
        KeyboardUtilities::PARSpecialKey(keySequLetter.toStdString());
    }
    else if(keyType == KeyType::SPECIAL)
    {
        QString key = (specialKeyIndex == SpecialKeyType::Tab) ? "Tab" : "Print";
        KeyboardUtilities::PARSpecialKey(key.toStdString());
    }

    Sleep(INTERNAL_DELAY);

    if(ctrl)
        KeyboardUtilities::ReleaseSpecialKey("Ctrl");
    if(shift)
        KeyboardUtilities::ReleaseSpecialKey("Shift");
    if(alt)
        KeyboardUtilities::ReleaseSpecialKey("Alt");
}

void Commands::WriteText(QStringList cmd)
{
    if(cmd[WriteTextCmdWidget::TypeIdx].toInt() == WriteTextType::Given)
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
