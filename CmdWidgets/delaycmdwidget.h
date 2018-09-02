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
        void ToggleLocked();
        void SetSettings(QStringList settings);

        int GetAmount();
        int GetTimeScale();
        void SetCmdSettings(int amount, int timeScale);

        static const int AmountIdx = 3;
        static const int ScaleIdx = 4;

    private:
        Ui::DelayCmdWidget *ui;
};

#endif // WAITCMDWIDGET_H
