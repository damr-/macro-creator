#ifndef SETCURSORPOSCOMMANDWIDGET_H
#define SETCURSORPOSCOMMANDWIDGET_H

#include "commandwidget.h"

namespace Ui {
class SetCursorPosCommandWidget;
}

class SetCursorPosCommandWidget : public CommandWidget
{
    Q_OBJECT

    public:
        explicit SetCursorPosCommandWidget(QWidget *parent = 0);
        ~SetCursorPosCommandWidget();
        int GetX();
        int GetY();
        void SetCoordinates(int x, int y);
        void CopyTo(CommandWidget *other);
        QString GetCommandString();

    private:
        Ui::SetCursorPosCommandWidget *ui;
};

#endif // SETCURSORPOSCOMMANDWIDGET_H
