#ifndef COMMANDS_H
#define COMMANDS_H

#include <QString>
#include <QList>

#include "clickcommandwidget.h"
#include "setcursorposcommandwidget.h"
#include "waitcommandwidget.h"

class Commands
{
public:
    const static QList<QString> commandNames()
    {
        return {
            "Click",
            "Set cursor position",
            "Wait"
        };
    }

    static CommandWidget* getNewCommandWidget(int commandIndex)
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

};

#endif // COMMANDS_H
