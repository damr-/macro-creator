#include "gotocmdwidget.h"
#include "ui_gotocmdwidget.h"

#include <QDebug>

GotoCmdWidget::GotoCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::GotoCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::GOTO;

    connect(ui->amountBox, SIGNAL(valueChanged(int)), this, SLOT(amountChanged(int)));

    connect(ui->rowBox, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->amountBox, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
}

GotoCmdWidget::~GotoCmdWidget()
{
    delete ui;
}

void GotoCmdWidget::CopyTo(CmdWidget *other)
{
    GotoCmdWidget *widget = qobject_cast<GotoCmdWidget*>(other);
    widget->SetCmdSettings(GetTargetRow(), GetAmount());
    CmdWidget::CopyTo(widget);
}

QString GotoCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + QString::number(GetTargetRow()) + "|" + QString::number(int(GetAmount()));
}

void GotoCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->rowBox->setEnabled(!isLocked);
    ui->amountBox->setEnabled(!isLocked);
}

void GotoCmdWidget::SetSettings(QStringList settings)
{
    SetCmdSettings(settings[TargetRowIdx].toInt(), settings[AmountIdx].toInt());
}

bool GotoCmdWidget::IsValidCmd()
{
    return GetAmount() != 0 && GetTargetRow() < GetRowNumber();
}

int GotoCmdWidget::GetTargetRow()
{
    return ui->rowBox->value();
}

int GotoCmdWidget::GetAmount()
{
    return ui->amountBox->value();
}

void GotoCmdWidget::SetCmdSettings(int targetRow, int amount)
{
    ui->rowBox->setValue(targetRow);
    ui->amountBox->setValue(amount);
}

void GotoCmdWidget::ValidateRowNumber(int cmdListRowCount)
{    
    int newTargetRow = GetTargetRow();

    if(newTargetRow > cmdListRowCount)
        newTargetRow = cmdListRowCount;
    if(newTargetRow > GetRowNumber())
        newTargetRow = GetRowNumber();

    ui->rowBox->setValue(newTargetRow);
}

void GotoCmdWidget::amountChanged(int amount)
{
    ui->timesLabel->setText(amount == 1 ? "time" : "times");
}
