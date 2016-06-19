#include "waitcommandwidget.h"
#include "ui_waitcommandwidget.h"

#include "commands.h"

WaitCommandWidget::WaitCommandWidget(QWidget *parent) :
    CommandWidget(parent),
    ui(new Ui::WaitCommandWidget)
{
    ui->setupUi(this);
    SetWaitSettings(1, 0);


    connect(ui->waitTime, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->waitTimeType, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCommandChangedSignal()));

    commandType = CommandType::Wait;
}

WaitCommandWidget::~WaitCommandWidget()
{
    delete ui;
}

void WaitCommandWidget::SetWaitSettings(int amount, int type)
{
    ui->waitTime->setValue(amount);
    ui->waitTimeType->setCurrentIndex(type);
}

void WaitCommandWidget::CopyTo(CommandWidget *other)
{
    WaitCommandWidget *widget = (WaitCommandWidget*) other;
    widget->SetWaitSettings(getAmount(), getWaitType());
}

QString WaitCommandWidget::GetCommandString()
{
    return QString::number((int)CommandType::Wait) + "|" + QString::number(getAmount()) + "|" + QString::number(getWaitType());
}

int WaitCommandWidget::getAmount()
{
    return ui->waitTime->value();
}

int WaitCommandWidget::getWaitType()
{
    return ui->waitTimeType->currentIndex();
}
