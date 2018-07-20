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
    HITKEY = 5,
    WRITETEXT = 6,
    RUNEXE = 7
};

class Commands
{
public:
    static void ExecuteCommand(QString command);
    const static QStringList commandNames()
    {
        return {
            "Delay"
            "Click",
            "Set cursor position",
            "Drag",
            "Scroll",
            "Write text",
            "Hit key",
            "Open .exe",
            "End process"
        };
    }
    static int getCommandTypeIndex(CmdType commandType) { return (int)commandType; }
};

#endif // COMMANDS_H
