#include "cmdwidget.h"
#include "ui_cmdwidget.h"

#include "clickcmdwidget.h"
#include "keyboardutilities.h"
#include "setcursorposcmdwidget.h"
#include "waitcmdwidget.h"
#include "dragcmdwidget.h"

CmdWidget* CmdWidget::GetNewCommandWidget(int commandIndex)
{
    switch(commandIndex){
        case 0:
            return new WaitCmdWidget();
        case 1:
            return new ClickCmdWidget();
        case 2:
            return new SetCursorPosCmdWidget();
        case 3:
            return new DragCmdWidget();
        default:
            return new CmdWidget();
    }
}

CmdWidget::CmdWidget(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::CmdWidget)
{
    ui->setupUi(this);
}

CmdWidget::~CmdWidget()
{
    delete ui;
}

void CmdWidget::CopyTo(CmdWidget *other)
{
    other = NULL;
}

QString CmdWidget::GetCommandString()
{
    return "";
}

void CmdWidget::emitCommandChangedSignal()
{
    emit commandChanged();
}
