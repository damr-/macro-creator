#ifndef WAITCMDWIDGET_H
#define WAITCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class WaitCmdWidget;
}

class WaitCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit WaitCmdWidget(QWidget *parent = 0);
        ~WaitCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCommandString();

        int getAmount();
        int getWaitType();
        void SetWaitSettings(int getAmount, int getWaitType);        

    private:
        Ui::WaitCmdWidget *ui;
};

#endif // WAITCMDWIDGET_H
