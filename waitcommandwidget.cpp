#include "waitcommandwidget.h"
#include "ui_waitcommandwidget.h"

WaitCommandWidget::WaitCommandWidget(QWidget *parent) :
    CommandWidget(parent),
    ui(new Ui::WaitCommandWidget)
{
    ui->setupUi(this);
    SetWaitSettings(qrand() % (100 + 1), qrand() % (2));
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
    widget->SetWaitSettings(amount(), type());
}

int WaitCommandWidget::amount()
{
    return ui->waitTime->value();
}

int WaitCommandWidget::type()
{
    return ui->waitTimeType->currentIndex();
}
