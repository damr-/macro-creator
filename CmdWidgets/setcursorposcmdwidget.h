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
        void ToggleLocked();

        int GetX();
        int GetY();
        void SetCmdSettings(int x, int y);

        static const int XIdx = 3;
        static const int YIdx = 4;

    protected:
        bool eventFilter(QObject *object, QEvent *event);

    signals:
        void showPosHint(bool, int, int);

    private:
        Ui::SetCursorPosCmdWidget *ui;
};

#endif // SETCURSORPOSCMDWIDGET_H
