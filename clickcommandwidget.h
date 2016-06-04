#ifndef CLICKCOMMANDWIDGET_H
#define CLICKCOMMANDWIDGET_H

#include "commandwidget.h"

namespace Ui {
class ClickCommandWidget;
}

class ClickCommandWidget : public CommandWidget
{
		Q_OBJECT

	public:
		explicit ClickCommandWidget(QWidget *parent = 0);
		~ClickCommandWidget();

	private:
		Ui::ClickCommandWidget *ui;
};

#endif // CLICKCOMMANDWIDGET_H
