#ifndef WAITCMDWIDGET_H
#define WAITCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class DelayCmdWidget;
}

class DelayCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit DelayCmdWidget(QWidget *parent = 0);
        ~DelayCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdSafeString();

        int GetAmount();
        int GetTimeScale();
        void SetWaitSettings(int GetAmount, int GetTimeScale);

    private:
        Ui::DelayCmdWidget *ui;
};

#endif // WAITCMDWIDGET_H
