#ifndef COMMANDS_H
#define COMMANDS_H

#include <QStringList>

enum CmdType
{
    DELAY = 0,
    GOTO = 1,
    CLICK = 2,
    SETCURSORPOS = 3,
    DRAG = 4,
    SCROLL = 5,
    PRESSKEY = 6,
    WRITETEXT = 7,
    RUNEXE = 8
};

enum StateType
{
    LOCKED = 0,
    DISABLED = 1
};

class Commands
{
    public:
        static void ExecuteCmd(QString cmd);

    private:
        static void Click(QStringList cmd);
        static void Delay(QStringList cmd);
        static void CursorPos(QStringList cmd);
        static void Drag(QStringList cmd);
        static void Scroll(QStringList cmd);
        static void PressKey(QStringList cmd);
        static void WriteText(QStringList cmd);
        static void RunExe(QStringList cmd);

        static const int INTERNAL_DELAY = 10;
};

#endif // COMMANDS_H
