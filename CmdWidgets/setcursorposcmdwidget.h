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
        QString GetCmdString();
        int GetCmdStrLen();

        int GetX();
        int GetY();
        void SetCmdSettings(int x, int y);

        static const int XIdx = 1;
        static const int YIdx = 2;

    private:
        Ui::SetCursorPosCmdWidget *ui;
};

#endif // SETCURSORPOSCMDWIDGET_H
