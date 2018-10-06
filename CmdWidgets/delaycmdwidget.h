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
        ~DelayCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void SetSettings(QStringList settings) override;

        int GetAmount();
        int GetTimeScale();
        void SetCmdSettings(int amount, int timeScale);

        static const int AmountIdx = ChildIdxStart;
        static const int ScaleIdx = ChildIdxStart + 1;

    private:
        Ui::DelayCmdWidget *ui;
};

#endif // WAITCMDWIDGET_H
