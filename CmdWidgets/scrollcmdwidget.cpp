#include "scrollcmdwidget.h"
#include "ui_scrollcmdwidget.h"

ScrollCmdWidget::ScrollCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::ScrollCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::SCROLL;

    connect(ui->amountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(scrollAmountChanged(int)));

    connect(ui->amountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->directionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCommandChangedSignal()));
}

ScrollCmdWidget::~ScrollCmdWidget()
{
    delete ui;
}

void ScrollCmdWidget::CopyTo(CmdWidget *other)
{
    qobject_cast<ScrollCmdWidget*>(other)->SetScrollSettings(GetAmount(), GetDirection());
}

QString ScrollCmdWidget::GetCmdSafeString()
{
    return QString("ScrollCmdWidget");
}

int ScrollCmdWidget::GetAmount()
{
    return ui->amountSpinBox->value();
}

int ScrollCmdWidget::GetDirection()
{
    return ui->directionComboBox->currentIndex();
}

void ScrollCmdWidget::SetScrollSettings(int amount, int direction)
{
    ui->amountSpinBox->setValue(amount);
    ui->directionComboBox->setCurrentIndex(direction);
}

void ScrollCmdWidget::scrollAmountChanged(int newTimes)
{
    QString newText = QString("time").append((newTimes > 1 ? "s" : ""));
    ui->timesLabel->setText(newText);
}
