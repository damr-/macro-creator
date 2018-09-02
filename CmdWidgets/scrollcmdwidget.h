#ifndef SCROLLCMDWIDGET_H
#define SCROLLCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class ScrollCmdWidget;
}

class ScrollCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit ScrollCmdWidget(QWidget *parent = nullptr);
        ~ScrollCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();
        void ToggleLocked();
        void SetSettings(QStringList settings);

        int GetAmount();
        int GetDirection();
        void SetCmdSettings(int amount, int direction);

        static const int AmountIdx = 3;
        static const int DirIdx = 4;

    private:
        Ui::ScrollCmdWidget *ui;

    private slots:
        void scrollAmountChanged(int newTimes);
};

#endif // SCROLLCMDWIDGET_H
