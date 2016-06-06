#include "commandwidget.h"
#include "ui_commandwidget.h"

#include <QDebug>

int CommandWidget::index = 0;

CommandWidget::CommandWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CommandWidget)
{
    ui->setupUi(this);
    this->index++;
}

CommandWidget::~CommandWidget()
{
    delete ui;
}

void CommandWidget::CopyTo(CommandWidget *other)
{
    other = NULL;
}

QString CommandWidget::GetCommandString()
{
    return "";
}

void CommandWidget::emitCommandChangedSignal()
{
    emit commandChanged();
}
