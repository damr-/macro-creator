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

#include <QDebug>

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
    ui->lockIcon->setVisible(false);
    isLocked = false;

    connect(ui->lockIcon, SIGNAL(clicked(bool)), this, SLOT(unlock()));
}

CmdWidget::~CmdWidget()
{
    delete ui;
}

QString CmdWidget::GetCmdString()
{
    return QString::number(int(cmdType)) + "|" + QString::number(isLocked) + "|" + QString::number(!isEnabled());
}

void CmdWidget::ToggleLocked()
{
    isLocked = !isLocked;
    ui->lockIcon->setVisible(isLocked);
    emitCmdChangedSignal();
}

bool CmdWidget::IsValidCmd()
{
    return true;
}

void CmdWidget::CopyTo(CmdWidget *other)
{
    if(isLocked)
        other->ToggleLocked();
    if(!isEnabled())
        other->ToggleEnabled();
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

int CmdWidget::GetCmdStringLen()
{
    return GetCmdString().split("|").length();
}

void CmdWidget::ToggleEnabled()
{
    this->setEnabled(!this->isEnabled());
    emitCmdChangedSignal();
}

void CmdWidget::SetCmdStates(bool isLocked, bool isDisabled)
{
    if((isLocked && !this->isLocked) || (!isLocked && this->isLocked))
        ToggleLocked();
    if((isDisabled && this->isEnabled()) || (!isDisabled && !this->isEnabled()))
        ToggleEnabled();
}

void CmdWidget::emitCmdChangedSignal()
{
    emit cmdChanged(this);
}

void CmdWidget::unlock()
{
    ToggleLocked();
}
