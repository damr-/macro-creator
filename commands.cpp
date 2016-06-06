#include "commands.h"

#include <Windows.h>
#include <QProcess>

#include "clickcommandwidget.h"
#include "keyboardutilities.h"
#include "setcursorposcommandwidget.h"
#include "waitcommandwidget.h"

#define SCROLLUP 120
#define SCROLLDOWN -120

Commands::Commands()
{

}

CommandWidget* Commands::GetNewCommandWidget(int commandIndex)
{
    switch(commandIndex){
        case 0:
            return new ClickCommandWidget();
        case 1:
            return new SetCursorPosCommandWidget();
        case 2:
            return new WaitCommandWidget();
        default:
            return new CommandWidget();
    }
}

void Commands::ExecuteCommand(QString command)
{
    QStringList commandList = command.split("|");

    if( commandList[0] == "mov" )
    {
        SetCursorPos( commandList[1].toInt(), commandList[2].toInt() );
        Sleep(50);
        if(commandList[3] == "1")
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(50);
        }
    }
    else if( commandList[0] == "clc" )
    {
        if( commandList[1] == "0" )
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(50);
        }
        else if( commandList[1] == "1" )
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(50);
        }
        else if( commandList[1] == "2" )
        {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            Sleep(50);
        }
        else if( commandList[1] == "3" )
        {
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
            Sleep(50);
        }
    }
    else if( commandList[0] == "drg" )
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        Sleep(10);
        SetCursorPos(commandList[1].toInt(), commandList[2].toInt());
        Sleep(10);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        Sleep(50);
    }
    else if( commandList[0] == "scr" )
    {
        int direction = commandList[2] == "0" ? SCROLLUP : SCROLLDOWN;
        for(int i = 0; i<=commandList[1].toInt(); ++i)
        {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, direction, 0);
            Sleep(50);
        }
    }
    else if( commandList[0] == "txt" )
    {
        if( commandList[1] == "0" )
        {
            keybd_event(VK_SHIFT, 0xAA, 0, 0);
            Sleep(50);
        }

        KeyboardUtilities::writeText(commandList[2].toStdString());

        if( commandList[1] == "0" )
        {
            keybd_event(VK_SHIFT, 0xAA, KEYEVENTF_KEYUP, 0);
            Sleep(50);
        }
    }
    else if( commandList[0] == "slp" )
    {
        int multi = commandList[2] == "0" ? 1000 : 1;
        Sleep(commandList[1].toInt() * multi);
    }
    else if( commandList[0] == "end" )
    {
        WinExec("taskkill /F /IM " + commandList[1].toUtf8(), SW_HIDE);
        Sleep(50);
    }
    else if( commandList[0] == "exe" )
    {
        WinExec(commandList[1].toUtf8(), SW_NORMAL);
        Sleep(50);
    }
    else if( commandList[0] == "sct" )
    {
        for(int i = 0; i < commandList[6].toInt(); ++i)
        {
            if(commandList[1] == "1")
                keybd_event(VK_CONTROL, 0x9D, 0, 0);
            if(commandList[2] == "1")
                keybd_event(VK_MENU, 0xB8, 0, 0);
            if(commandList[3] == "1")
                keybd_event(VK_SHIFT, 0xAA, 0, 0);

            if(commandList[4] == "1")
                KeyboardUtilities::writeText(commandList[5].toStdString());
            else
                KeyboardUtilities::pressVK(commandList[5].toStdString());

            if(commandList[1] == "1")
                keybd_event(VK_CONTROL, 0x9D, KEYEVENTF_KEYUP, 0);
            if(commandList[2] == "1")
                keybd_event(VK_MENU, 0xB8, KEYEVENTF_KEYUP, 0);
            if(commandList[3] == "1")
                keybd_event(VK_SHIFT, 0xAA, KEYEVENTF_KEYUP, 0);
            Sleep(50);
        }
    }
}
