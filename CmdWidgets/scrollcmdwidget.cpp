#include "scrollcmdwidget.h"
#include "ui_scrollcmdwidget.h"

ScrollCmdWidget::ScrollCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::ScrollCmdWidget)
{
    ui->setupUi(this);

    connect(ui->amountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(scrollAmountChanged(int)));
}

ScrollCmdWidget::~ScrollCmdWidget()
{
    delete ui;
}

void ScrollCmdWidget::CopyTo(CmdWidget *other)
{
    ScrollCmdWidget *widget = (ScrollCmdWidget*) other;
    //TODO
}

QString ScrollCmdWidget::GetCmdSafeString()
{
    return QString("ScrollCmdWidget");
}

void ScrollCmdWidget::scrollAmountChanged(int newTimes)
{
    QString newText = QString("time").append((newTimes > 1 ? "s" : ""));
    ui->timesLabel->setText(newText);
}
