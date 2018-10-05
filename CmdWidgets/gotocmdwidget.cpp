#include "gotocmdwidget.h"
#include "gotocmdwidget.h"
#include "ui_gotocmdwidget.h"

#include <QDebug>

GotoCmdWidget::GotoCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::GotoCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::GOTO;

    connect(ui->amountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(amountChanged(int)));

    connect(ui->rowSpinBox, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->amountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));

    WheelEventWidgets = QList<QWidget*>{ui->rowSpinBox, ui->amountSpinBox};
    InstallWheelEventFilters();
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
    ui->rowSpinBox->setEnabled(!isLocked);
    ui->amountSpinBox->setEnabled(!isLocked);
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
    return ui->rowSpinBox->value();
}

int GotoCmdWidget::GetAmount()
{
    return ui->amountSpinBox->value();
}

void GotoCmdWidget::SetCmdSettings(int targetRow, int amount)
{
    ui->rowSpinBox->setValue(targetRow);
    ui->amountSpinBox->setValue(amount);
}

void GotoCmdWidget::ValidateRowNumber(int cmdListRowCount)
{    
    int newTargetRow = GetTargetRow();

    if(newTargetRow > cmdListRowCount)
        newTargetRow = cmdListRowCount;
    if(newTargetRow > GetRowNumber())
        newTargetRow = GetRowNumber();

    ui->rowSpinBox->setValue(newTargetRow);
}

void GotoCmdWidget::amountChanged(int amount)
{
    ui->timesLabel->setText(amount == 1 ? "time" : "times");
}
