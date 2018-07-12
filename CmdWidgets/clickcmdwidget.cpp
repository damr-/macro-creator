#include "clickcmdwidget.h"
#include "ui_clickcmdwidget.h"

#include "commands.h"

ClickCmdWidget::ClickCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::ClickCmdWidget)
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

    commandType = CommandType::Click;
}

ClickCmdWidget::~ClickCmdWidget()
{
    delete ui;
}

void ClickCmdWidget::SetClickAmount(int amount)
{
    ui->clickAmountSpinBox->setValue(amount);
}

void ClickCmdWidget::SetClickType(ClickType clickType)
{
    ui->clickTypeBox->setCurrentIndex((int)clickType);
}

void ClickCmdWidget::clickAmountChanged(int amount)
{
    ui->clickLabel->setText(amount > 1 ? "clicks" : "click");
}

void ClickCmdWidget::CopyTo(CmdWidget *other)
{
    ClickCmdWidget* widget = (ClickCmdWidget*)other;
    widget->SetClickAmount(clickAmount());
}

QString ClickCmdWidget::GetCommandString()
{
    return QString::number((int)CommandType::Click) + "|" + QString::number(clickAmount()) + "|" + QString::number((int)GetClickType());
}

int ClickCmdWidget::clickAmount()
{
    return ui->clickAmountSpinBox->value();
}

ClickType ClickCmdWidget::GetClickType()
{
    return (ClickType)ui->clickTypeBox->currentIndex();
}
