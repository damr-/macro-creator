#include "clickcommandwidget.h"
#include "ui_clickcommandwidget.h"

ClickCommandWidget::ClickCommandWidget(QWidget *parent) :
	CommandWidget(parent),
	ui(new Ui::ClickCommandWidget)
{
	ui->setupUi(this);

	int max = ui->clickAmountSpinBox->maximum();
	int min = ui->clickAmountSpinBox->minimum();
    SetClickAmount(qrand() % ((max + 1) - min) + min);
}

ClickCommandWidget::~ClickCommandWidget()
{
    delete ui;
}

void ClickCommandWidget::SetClickAmount(int amount)
{
    ui->clickAmountSpinBox->setValue(amount);
}

void ClickCommandWidget::CopyTo(CommandWidget *other)
{
    ClickCommandWidget* widget = (ClickCommandWidget*)other;
    widget->SetClickAmount(clickAmount());
}

int ClickCommandWidget::clickAmount()
{
    return ui->clickAmountSpinBox->value();
}
