#ifndef COMMANDS_H
#define COMMANDS_H

#include <QString>
#include <QStringList>

#include "commandwidget.h"

class Commands
{
public:
    Commands();
    static void ExecuteCommand(QString command);
    static CommandWidget* GetNewCommandWidget(int commandIndex);
    const static QList<QString> commandNames()
    {
        return {
            "Click",
            "Set cursor position",
            "Wait"
        };
    }

};

#endif // COMMANDS_H
