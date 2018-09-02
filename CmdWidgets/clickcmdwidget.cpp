#include "clickcmdwidget.h"
#include "ui_clickcmdwidget.h"

#include "commands.h"

ClickCmdWidget::ClickCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::ClickCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::CLICK;

    for(int i = ClickType::Left; i <= ClickType::Middle; i++)
        ui->clickTypeBox->addItem(ClickTypeNames[i]);

    connect(ui->clickAmountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(clickAmountChanged(int)));

    connect(ui->clickAmountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->clickTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCmdChangedSignal()));
}

ClickCmdWidget::~ClickCmdWidget()
{
    delete ui;
}

void ClickCmdWidget::CopyTo(CmdWidget *other)
{
    ClickCmdWidget *widget = qobject_cast<ClickCmdWidget*>(other);
    widget->SetCmdSettings(GetClickAmount(), GetClickType());
    CmdWidget::CopyTo(widget);
}

QString ClickCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + QString::number(GetClickAmount()) + "|" + QString::number(int(GetClickType()));
}

void ClickCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->clickAmountSpinBox->setEnabled(!isLocked);
    ui->clickTypeBox->setEnabled(!isLocked);
}

void ClickCmdWidget::SetSettings(QStringList settings)
{
    SetCmdSettings(settings[AmountIdx].toInt(), static_cast<ClickType>(settings[TypeIdx].toInt()));
}

int ClickCmdWidget::GetClickAmount()
{
    return ui->clickAmountSpinBox->value();
}

ClickType ClickCmdWidget::GetClickType()
{
    return ClickType(ui->clickTypeBox->currentIndex());
}

void ClickCmdWidget::SetCmdSettings(int amount, ClickType clickType)
{
    ui->clickAmountSpinBox->setValue(amount);
    ui->clickTypeBox->setCurrentIndex(int(clickType));
}

void ClickCmdWidget::clickAmountChanged(int amount)
{
    ui->clickLabel->setText(amount > 1 ? "clicks" : "click");
}
