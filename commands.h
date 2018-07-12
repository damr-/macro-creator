#ifndef COMMANDS_H
#define COMMANDS_H

#include <QString>
#include <QStringList>

enum CommandType
{
    Delay,
    Click,
    SetCursorPosition,
    Drag,
    Scroll,
    WriteText,
    HitKey,
    OpenExe,
    EndProcess
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
            "Cursor position",
            "Drag",
            "Scroll",
            "Write text",
            "Hit key",
            "Open .exe",
            "End process"
        };
    }
    static int getCommandTypeIndex(CommandType commandType) { return (int)commandType; }

};

#endif // COMMANDS_H
