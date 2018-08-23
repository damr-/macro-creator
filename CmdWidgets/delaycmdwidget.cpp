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

    connect(ui->delayAmount, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->delayTimeType, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCmdChangedSignal()));
}

DelayCmdWidget::~DelayCmdWidget()
{
    delete ui;
}

void DelayCmdWidget::CopyTo(CmdWidget *other)
{
    DelayCmdWidget *widget = qobject_cast<DelayCmdWidget*>(other);
    widget->SetCmdSettings(GetAmount(), GetTimeScale());
    CmdWidget::CopyTo(widget);
}

QString DelayCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + QString::number(GetAmount()) + "|" + QString::number(GetTimeScale());
}

void DelayCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->delayAmount->setEnabled(!isLocked);
    ui->delayTimeType->setEnabled(!isLocked);
}

int DelayCmdWidget::GetAmount()
{
    return ui->delayAmount->value();
}

int DelayCmdWidget::GetTimeScale()
{
    return ui->delayTimeType->currentIndex();
}

void DelayCmdWidget::SetCmdSettings(int amount, int timeScale)
{
    ui->delayAmount->setValue(amount);
    ui->delayTimeType->setCurrentIndex(timeScale);
}
