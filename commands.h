#ifndef COMMANDS_H
#define COMMANDS_H

#include <QString>
#include <QStringList>

enum CmdType
{
    DELAY = 0,
    CLICK = 1,
    CURPOS = 2,
    DRAG = 3,
    SCROLL = 4,
    PRESSKEY = 5,
    WRITETEXT = 6,
    RUNEXE = 7
};

class Commands
{
public:
    static void ExecuteCommand(QString command);

private:
    static void Click(QStringList cmd);
    static void Delay(QStringList cmd);
    static void CursorPos(QStringList cmd);
    static void Drag(QStringList cmd);
    static void Scroll(QStringList cmd);
    static void PressKey(QStringList cmd);
    static void WriteText(QStringList cmd);
    static void RunExe(QStringList cmd);
};

#endif // COMMANDS_H
