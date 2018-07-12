#include "waitcmdwidget.h"
#include "ui_waitcmdwidget.h"

#include "commands.h"

WaitCmdWidget::WaitCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::WaitCmdWidget)
{
    ui->setupUi(this);
    SetWaitSettings(1, 0);

    connect(ui->waitTime, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->waitTimeType, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCommandChangedSignal()));

    commandType = CommandType::Delay;
}

WaitCmdWidget::~WaitCmdWidget()
{
    delete ui;
}

void WaitCmdWidget::SetWaitSettings(int amount, int type)
{
    ui->waitTime->setValue(amount);
    ui->waitTimeType->setCurrentIndex(type);
}

void WaitCmdWidget::CopyTo(CmdWidget *other)
{
    WaitCmdWidget *widget = (WaitCmdWidget*) other;
    widget->SetWaitSettings(getAmount(), getWaitType());
}

QString WaitCmdWidget::GetCommandString()
{
    return QString::number((int)CommandType::Delay) + "|" + QString::number(getAmount()) + "|" + QString::number(getWaitType());
}

int WaitCmdWidget::getAmount()
{
    return ui->waitTime->value();
}

int WaitCmdWidget::getWaitType()
{
    return ui->waitTimeType->currentIndex();
}
