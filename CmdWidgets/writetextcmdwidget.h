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
        QString GetCmdString();
        bool IsValidCmd();

        QString GetText();
        void SetText(QString text);

        static const int TextIdx = 1;

    private:
        Ui::WriteTextCmdWidget *ui;
};

#endif // WRITETEXTCMDWIDGET_H
