#include "commands.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QProcess>
#include <QRegularExpression>

#include "keyboardutilities.h"
#include "CmdWidgets/clickcmdwidget.h"
#include "CmdWidgets/delaycmdwidget.h"
#include "CmdWidgets/dragcmdwidget.h"
#include "CmdWidgets/presskeycmdwidget.h"
#include "CmdWidgets/runexecmdwidget.h"
#include "CmdWidgets/scrollcmdwidget.h"
#include "CmdWidgets/setcursorposcmdwidget.h"
#include "CmdWidgets/writetextcmdwidget.h"
#include "CmdWidgets/applyregexcmdwidget.h"

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
    case CmdType::SETCURSORPOS:
        CursorPos(cmdParts); break;
    case CmdType::CLICK:
        Click(cmdParts); break;
    case CmdType::PRESSKEY:
        PressKey(cmdParts); break;
    case CmdType::WRITETEXT:
        WriteText(cmdParts); break;
    case CmdType::DRAG:
        Drag(cmdParts); break;
    case CmdType::SCROLL:
        Scroll(cmdParts); break;
    case CmdType::RUNEXE:
        RunExe(cmdParts); break;
    case CmdType::REGEX:
        ApplyRegex(cmdParts); break;
    }
}

void Commands::Delay(QStringList cmd)
{
    DWORD amount = DWORD(cmd[DelayCmdWidget::AmountIdx].toInt());
    DWORD timeScale = cmd[DelayCmdWidget::ScaleIdx] == "0" ? 1000 : 1;
    DWORD timeMS = amount * timeScale;

    DWORD partLengthMS = 1000;

    DWORD parts = timeMS / partLengthMS;
    DWORD rest = timeMS % partLengthMS;

    for (DWORD i = 0; i < parts; i++) {
        Sleep(partLengthMS);
        qApp->processEvents();
    }
    Sleep(rest);
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

        if(i % 50 == 0)
            qApp->processEvents();

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

        if(i % 50 == 0)
            qApp->processEvents();

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
    QString specialKey = cmd[PressKeyCmdWidget::SpcKeyIdx];

    if(ctrl)
        KeyboardUtilities::PressKey("Ctrl");
    if(shift)
        KeyboardUtilities::PressKey("Shift");
    if(alt)
        KeyboardUtilities::PressKey("Alt");

    Sleep(INTERNAL_DELAY);

    if(keyType == KeyType::KEY)
        KeyboardUtilities::PressAndReleaseKey(keySequLetter.toStdString());
    else if(keyType == KeyType::SPECIAL)
        KeyboardUtilities::PressAndReleaseKey(specialKey.toStdString());

    Sleep(INTERNAL_DELAY);

    if(ctrl)
        KeyboardUtilities::ReleaseKey("Ctrl");
    if(shift)
        KeyboardUtilities::ReleaseKey("Shift");
    if(alt)
        KeyboardUtilities::ReleaseKey("Alt");
}

void Commands::WriteText(QStringList cmd)
{
    QString text = "";
    if(cmd[WriteTextCmdWidget::TypeIdx].toInt() == WriteTextType::Given)
    {
        text = CmdWidget::FromHex(cmd[WriteTextCmdWidget::TextIdx]);
    }
    else
    {
        QString possibleChars = CmdWidget::FromHex(cmd[WriteTextCmdWidget::CharsIdx]);
        int high = possibleChars.count();

        for(int i = 0, total = cmd[WriteTextCmdWidget::AmountIdx].toInt(); i < total; ++i)
        {
            int randIndex = qrand() % high;
            text.append(possibleChars.at(randIndex));
        }
    }

    if(cmd[WriteTextCmdWidget::PasteIdx].toInt())
    {
        QClipboard *clipboard = QApplication::clipboard();

        for(int i = 0, total = text.length(); i < total; ++i)
        {
            clipboard->setText(QString(text[i]));

            //Hit Ctrl+V to paste the character from the clipboard
            ExecuteCmd(PressKeyCmdWidget::GetPasteCmd());

            qApp->processEvents();
            Sleep(INTERNAL_DELAY);
        }
    }
    else
    {
        int partLength = 20;
        int parts = text.length() / partLength;
        int rest = text.length() % partLength;

        for (int i = 0; i < parts; i++)
        {
            KeyboardUtilities::WriteText(text.mid(i * partLength, partLength).toStdString());
            qApp->processEvents();
        }
        KeyboardUtilities::WriteText(text.right(rest).toStdString());
    }
}

void Commands::RunExe(QStringList cmd)
{
    QString path = CmdWidget::FromHex(cmd[RunExeCmdWidget::PathIdx]);
    WinExec(path.toUtf8(), SW_NORMAL);
}

void Commands::ApplyRegex(QStringList cmd)
{
    QString regex = CmdWidget::FromHex(cmd[ApplyRegexCmdWidget::RegexIdx]);
    QRegularExpression re(regex);
    QClipboard *clipboard = QApplication::clipboard();

    QString text = "";
    int counter = 0;

    //Sometimes Ctrl+C doesn't register, so we try until it does.
    //The counter is for the rare case that the clipboard is empty and nothing is actually selected.
    while(text == "" && counter++ < 100)
    {
        ExecuteCmd(PressKeyCmdWidget::GetCopyCmd());
        qApp->processEvents();
        text = clipboard->text();
    }

    QRegularExpressionMatch match = re.match(text);
    if(!match.hasMatch())
    {
        qDebug() << "No Regex match for '" + text + "'";
        return;
    }

    QString result = match.captured(0);
    clipboard->clear();
    clipboard->setText(result);

    //Hit Ctrl+V to paste the result, if the user chose to
    if(cmd[ApplyRegexCmdWidget::PasteIdx].toInt())
        ExecuteCmd(PressKeyCmdWidget::GetPasteCmd());
}
