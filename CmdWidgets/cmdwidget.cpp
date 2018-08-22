#include "cmdwidget.h"
#include "ui_cmdwidget.h"

#include "clickcmdwidget.h"
#include "gotocmdwidget.h"
#include "keyboardutilities.h"
#include "setcursorposcmdwidget.h"
#include "delaycmdwidget.h"
#include "dragcmdwidget.h"
#include "scrollcmdwidget.h"
#include "writetextcmdwidget.h"
#include "presskeycmdwidget.h"
#include "runexecmdwidget.h"

CmdWidget* CmdWidget::GetNewCmdWidget(CmdType cmdType)
{
    switch(cmdType){
        case CmdType::DELAY:
            return new DelayCmdWidget();
        case CmdType::GOTO:
            return new GotoCmdWidget();
        case CmdType::CLICK:
            return new ClickCmdWidget();
        case CmdType::CURPOS:
            return new SetCursorPosCmdWidget();
        case CmdType::DRAG:
            return new DragCmdWidget();
        case CmdType::SCROLL:
            return new ScrollCmdWidget();
        case CmdType::WRITETEXT:
            return new WriteTextCmdWidget();
        case CmdType::PRESSKEY:
            return new PressKeyCmdWidget();
        default:
            return new DelayCmdWidget();
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

int CmdWidget::GetCmdStringLen()
{
    return GetCmdString().split("|").length();
}

QString CmdWidget::GetCmdString()
{
    return QString("CmdWidget");
}

void CmdWidget::CopyTo(CmdWidget *other)
{
    other->GetCmdString();
}

CmdType CmdWidget::GetCmdType()
{
    return cmdType;
}

void CmdWidget::SetRowNumber(int number)
{
    ui->rowIndexLabel->setText(QString::number(number));
}

int CmdWidget::GetRowNumber()
{
    return ui->rowIndexLabel->text().toInt();
}

bool CmdWidget::IsValidCmd()
{
    return true;
}

void CmdWidget::emitCmdChangedSignal()
{
    emit cmdChanged(this);
}
