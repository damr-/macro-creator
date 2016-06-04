#include "commandwidget.h"
#include "ui_commandwidget.h"

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
