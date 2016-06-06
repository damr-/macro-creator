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
        void CopyTo(CommandWidget *other);
        int GetX();
        int GetY();
        void SetCoordinates(int x, int y);

    private:
        Ui::SetCursorPosCommandWidget *ui;
};

#endif // SETCURSORPOSCOMMANDWIDGET_H
