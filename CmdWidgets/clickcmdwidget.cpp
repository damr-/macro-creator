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

    connect(ui->clickAmountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->clickTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCommandChangedSignal()));
}

ClickCmdWidget::~ClickCmdWidget()
{
    delete ui;
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

void ClickCmdWidget::CopyTo(CmdWidget *other)
{
    qobject_cast<ClickCmdWidget*>(other)->SetCmdSettings(GetClickAmount(), GetClickType());
}

int ClickCmdWidget::GetCmdStrLen()
{
    return 3;
}

QString ClickCmdWidget::GetCmdString()
{
    return QString::number(int(cmdType)) + "|" + QString::number(GetClickAmount()) + "|" + QString::number(int(GetClickType()));
}

int ClickCmdWidget::GetClickAmount()
{
    return ui->clickAmountSpinBox->value();
}

ClickType ClickCmdWidget::GetClickType()
{
    return ClickType(ui->clickTypeBox->currentIndex());
}
