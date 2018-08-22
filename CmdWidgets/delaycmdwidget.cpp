#include "delaycmdwidget.h"
#include "ui_delaycmdwidget.h"

#include "commands.h"

DelayCmdWidget::DelayCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::DelayCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::DELAY;

    SetCmdSettings(1, 0);

    connect(ui->waitTime, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->waitTimeType, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCmdChangedSignal()));
}

DelayCmdWidget::~DelayCmdWidget()
{
    delete ui;
}

void DelayCmdWidget::CopyTo(CmdWidget *other)
{
    DelayCmdWidget *widget = qobject_cast<DelayCmdWidget*>(other);
    widget->SetCmdSettings(GetAmount(), GetTimeScale());
}

QString DelayCmdWidget::GetCmdString()
{
    return QString::number(int(cmdType)) + "|" + QString::number(GetAmount()) + "|" + QString::number(GetTimeScale());
}

int DelayCmdWidget::GetAmount()
{
    return ui->waitTime->value();
}

int DelayCmdWidget::GetTimeScale()
{
    return ui->waitTimeType->currentIndex();
}

void DelayCmdWidget::SetCmdSettings(int amount, int timeScale)
{
    ui->waitTime->setValue(amount);
    ui->waitTimeType->setCurrentIndex(timeScale);
}
