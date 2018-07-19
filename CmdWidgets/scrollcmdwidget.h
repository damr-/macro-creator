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
        explicit ScrollCmdWidget(QWidget *parent = 0);
        ~ScrollCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdSafeString();

    private:
        Ui::ScrollCmdWidget *ui;

    private slots:
        void scrollAmountChanged(int newTimes);
};

#endif // SCROLLCMDWIDGET_H
