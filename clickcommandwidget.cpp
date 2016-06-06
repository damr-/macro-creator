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
    SetClickAmount(qrand() % ((max + 1) - min) + min);

    connect(ui->clickAmountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(clickAmountChanged(int)));

    for(int i = ClickType::Left; i <= ClickType::Middle; i++)
    {
        ui->clickTypeBox->addItem(ClickTypeNames[i]);
    }
    clickAmountChanged(ui->clickAmountSpinBox->value());

    connect(ui->clickAmountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->clickTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCommandChangedSignal()));
}

ClickCommandWidget::~ClickCommandWidget()
{
    delete ui;
}

void ClickCommandWidget::SetClickAmount(int amount)
{
    ui->clickAmountSpinBox->setValue(amount);
}

void ClickCommandWidget::clickAmountChanged(int amount)
{
    ui->clickLabel->setText(amount > 1 ? "clicks" : "click");
}

void ClickCommandWidget::CopyTo(CommandWidget *other)
{
    ClickCommandWidget* widget = (ClickCommandWidget*)other;
    widget->SetClickAmount(clickAmount());
}

QString ClickCommandWidget::GetCommandString()
{
    return "clc|" + QString::number(clickAmount()) + "|" + ui->clickTypeBox->currentIndex();
}

int ClickCommandWidget::clickAmount()
{
    return ui->clickAmountSpinBox->value();
}

ClickType ClickCommandWidget::clickType()
{
    return (ClickType)ui->clickTypeBox->currentIndex();
}
