#include "commandwidget.h"
#include "ui_commandwidget.h"

#include "clickcommandwidget.h"
#include "keyboardutilities.h"
#include "setcursorposcommandwidget.h"
#include "waitcommandwidget.h"
#include "dragcommandwidget.h"

CommandWidget* CommandWidget::GetNewCommandWidget(int commandIndex)
{
    switch(commandIndex){
        case -1:
            return new WaitCommandWidget();
        case 0:
            return new ClickCommandWidget();
        case 1:
            return new SetCursorPosCommandWidget();
        case 2:
            return new DragCommandWidget();
        default:
            return new CommandWidget();
    }
}

CommandWidget::CommandWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CommandWidget)
{
    ui->setupUi(this);
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
