#ifndef COMMANDS_H
#define COMMANDS_H

#include <windows.h>

#include <QStringList>

enum CmdType
{
    DELAY = 0,
    GOTO = 1,
    SETCURSORPOS = 2,
    CLICK = 3,
    PRESSKEY = 4,
    WRITETEXT = 5,
    DRAG = 6,
    SCROLL = 7,
    RUNEXE = 8,
    REGEX = 9,
};

class Commands
{
    public:
        static void ExecuteCmd(QString cmd);

    private:
        static void Click(QStringList cmd);
        static void Click(DWORD buttonDown, DWORD buttonUp);
        static void Delay(QStringList cmd);
        static void CursorPos(QStringList cmd);
        static void Drag(QStringList cmd);
        static void Scroll(QStringList cmd);
        static void PressKey(QStringList cmd);
        static void WriteText(QStringList cmd);
        static void RunExe(QStringList cmd);
        static void ApplyRegex(QStringList cmd);

        static const int INTERNAL_DELAY = 25;
};

#endif // COMMANDS_H
