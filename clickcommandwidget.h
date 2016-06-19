#ifndef CLICKCOMMANDWIDGET_H
#define CLICKCOMMANDWIDGET_H

#include "commandwidget.h"

namespace Ui {
class ClickCommandWidget;
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

class ClickCommandWidget : public CommandWidget
{
    Q_OBJECT

	public:
		explicit ClickCommandWidget(QWidget *parent = 0);
        ~ClickCommandWidget();

        void CopyTo(CommandWidget *other);
        QString GetCommandString();
        void SetupCommandChangedSignals();

        int clickAmount();
        ClickType clickType();
        void SetClickAmount(int amount);       
        void SetClickType(ClickType clickType);

    private slots:
        void clickAmountChanged(int amount);

	private:
		Ui::ClickCommandWidget *ui;
};

#endif // CLICKCOMMANDWIDGET_H
