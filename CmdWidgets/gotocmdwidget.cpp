#include "gotocmdwidget.h"
#include "ui_gotocmdwidget.h"

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
    qobject_cast<GotoCmdWidget*>(other)->SetCmdSettings(GetTargetRow(), GetAmount());
}

QString GotoCmdWidget::GetCmdString()
{
    return QString::number(int(cmdType)) + "|" + QString::number(GetTargetRow()) + "|" + QString::number(int(GetAmount()));
}

bool GotoCmdWidget::IsValidCmd()
{
    return ui->amountBox->value() != 0;
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
    if(GetTargetRow() > cmdListRowCount)
        ui->rowBox->setValue(cmdListRowCount);
}

void GotoCmdWidget::amountChanged(int amount)
{
    ui->timesLabel->setText(amount == 1 ? "time." : "times.");
}
