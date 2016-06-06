#ifndef WAITCOMMANDWIDGET_H
#define WAITCOMMANDWIDGET_H

#include "commandwidget.h"

namespace Ui {
class WaitCommandWidget;
}

class WaitCommandWidget : public CommandWidget
{
    Q_OBJECT

    public:
        explicit WaitCommandWidget(QWidget *parent = 0);
        ~WaitCommandWidget();
        int getAmount();
        int getWaitType();
        void SetWaitSettings(int getAmount, int getWaitType);
        void CopyTo(CommandWidget *other);
        QString GetCommandString();

    private:
        Ui::WaitCommandWidget *ui;
};

#endif // WAITCOMMANDWIDGET_H
