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
        ~SetCursorPosCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void ToggleLocked() override;
        void SetSettings(QStringList settings) override;

        int GetX();
        int GetY();
        bool GetAddClick();
        void SetCmdSettings(int x, int y, bool addClick);

        static const int XIdx = 3;
        static const int YIdx = 4;
        static const int ClickIdx = 5;

    protected:
        bool eventFilter(QObject *object, QEvent *event) override;

    signals:
        void showPosHint(bool, int, int);

    private:
        Ui::SetCursorPosCmdWidget *ui;
};

#endif // SETCURSORPOSCMDWIDGET_H
