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
        ~ClickCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void SetSettings(QStringList settings) override;

        int GetClickAmount();
        ClickType GetClickType();
        void SetCmdSettings(int amount, ClickType clickType);

        static const int AmountIdx = ChildIdxStart;
        static const int TypeIdx = ChildIdxStart + 1;

    private slots:
        void clickAmountChanged(int amount);

	private:
        Ui::ClickCmdWidget *ui;
};

#endif // CLICKCMDWIDGET_H
