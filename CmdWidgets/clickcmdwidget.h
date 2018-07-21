#ifndef CLICKCMDWIDGET_H
#define CLICKCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class ClickCmdWidget;
}

static const QList<QString> ClickTypeNames {
    "Left Mouse Button",
    "Double Left Mouse Button",
    "Right Mouse Button",
    "Middle Mouse Button"
};

enum ClickType
{
    Left,
    DoubleLeft,
    Right,
    Middle
};

class ClickCmdWidget : public CmdWidget
{
    Q_OBJECT

	public:
        explicit ClickCmdWidget(QWidget *parent = nullptr);
        ~ClickCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();

        int GetClickAmount();
        ClickType GetClickType();
        void SetClickSettings(int amount, ClickType clickType);

    private slots:
        void clickAmountChanged(int amount);

	private:
        Ui::ClickCmdWidget *ui;
};

#endif // CLICKCMDWIDGET_H
