#ifndef COMMANDS_H
#define COMMANDS_H

#include <QString>
#include <QStringList>

enum CommandType
{
    Click,
    SetCursorPosition,
    Wait
};

class Commands
{
public:
    static void ExecuteCommand(QString command);
    const static QStringList commandNames()
    {
        return {
            "Click",
            "Set cursor position"
        };
    }
    static int getCommandTypeIndex(CommandType commandType) { return (int)commandType; }

};

#endif // COMMANDS_H
