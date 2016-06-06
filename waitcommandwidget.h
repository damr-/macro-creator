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
        void CopyTo(CommandWidget *other);
        int amount();
        int type();
        void SetWaitSettings(int amount, int type);

    private:
        Ui::WaitCommandWidget *ui;
};

#endif // WAITCOMMANDWIDGET_H
