#ifndef SETCURSORPOSCMDWIDGET_H
#define SETCURSORPOSCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class SetCursorPosCmdWidget;
}

class SetCursorPosCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit SetCursorPosCmdWidget(QWidget *parent = nullptr);
        ~SetCursorPosCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdSafeString();

        int GetX();
        int GetY();
        void SetCoordinates(int x, int y);

    private:
        Ui::SetCursorPosCmdWidget *ui;
};

#endif // SETCURSORPOSCMDWIDGET_H
