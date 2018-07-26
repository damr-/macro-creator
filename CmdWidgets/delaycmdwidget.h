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
        explicit DelayCmdWidget(QWidget *parent = nullptr);
        ~DelayCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();
        int GetCmdStrLen();

        int GetAmount();
        int GetTimeScale();
        void SetCmdSettings(int GetAmount, int GetTimeScale);

        static const int AmountIdx = 1;
        static const int ScaleIdx = 2;

    private:
        Ui::DelayCmdWidget *ui;
};

#endif // WAITCMDWIDGET_H
