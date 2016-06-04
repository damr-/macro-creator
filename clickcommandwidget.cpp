#include "clickcommandwidget.h"
#include "ui_clickcommandwidget.h"

#include <QDebug>

ClickCommandWidget::ClickCommandWidget(QWidget *parent) :
	CommandWidget(parent),
	ui(new Ui::ClickCommandWidget)
{
	ui->setupUi(this);

	int max = ui->clickAmountSpinBox->maximum();
	int min = ui->clickAmountSpinBox->minimum();

	int val = qrand() % ((max + 1) - min) + min;

	ui->clickAmountSpinBox->setValue(val);
}

ClickCommandWidget::~ClickCommandWidget()
{
	delete ui;
}
