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

    connect(ui->waitTime, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->waitTimeType, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCommandChangedSignal()));
}

DelayCmdWidget::~DelayCmdWidget()
{
    delete ui;
}

void DelayCmdWidget::SetCmdSettings(int amount, int type)
{
    ui->waitTime->setValue(amount);
    ui->waitTimeType->setCurrentIndex(type);
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

int DelayCmdWidget::GetCmdStrLen()
{
    return 3;
}

int DelayCmdWidget::GetAmount()
{
    return ui->waitTime->value();
}

int DelayCmdWidget::GetTimeScale()
{
    return ui->waitTimeType->currentIndex();
}
