#ifndef WRITETEXTCMDWIDGET_H
#define WRITETEXTCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class WriteTextCmdWidget;
}

class WriteTextCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit WriteTextCmdWidget(QWidget *parent = nullptr);
        ~WriteTextCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdSafeString();

    private:
        Ui::WriteTextCmdWidget *ui;
};

#endif // WRITETEXTCMDWIDGET_H
