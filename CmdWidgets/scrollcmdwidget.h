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
        ~ScrollCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void SetSettings(QStringList settings) override;

        int GetAmount();
        int GetDirection();
        void SetCmdSettings(int amount, int direction);

        static const int AmountIdx = ChildIdxStart;
        static const int DirIdx = ChildIdxStart + 1;

    private:
        Ui::ScrollCmdWidget *ui;

    private slots:
        void scrollAmountChanged(int newTimes);
};

#endif // SCROLLCMDWIDGET_H
