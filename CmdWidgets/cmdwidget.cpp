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
#include "applyregexcmdwidget.h"

#include <QDebug>

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

void CmdWidget::SetSettings(QStringList settings)
{
    settings.first(); //to remove 'unused parameter' warning
}

QString CmdWidget::GetCmdString()
{
    return QString::number(int(cmdType)) + "|" + QString::number(!isEnabled());
}

bool CmdWidget::IsValidCmd()
{
    return true;
}

void CmdWidget::CopyTo(CmdWidget *other)
{
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

void CmdWidget::SetDisabled(bool isDisabled)
{
    if((isDisabled && this->isEnabled()) || (!isDisabled && !this->isEnabled()))
        ToggleEnabled();
}

CmdWidget* CmdWidget::GetNewCmdWidget(CmdType cmdType)
{
    switch(cmdType){
        case CmdType::DELAY:
            return new DelayCmdWidget();
        case CmdType::GOTO:
            return new GotoCmdWidget();
        case CmdType::CLICK:
            return new ClickCmdWidget();
        case CmdType::SETCURSORPOS:
            return new SetCursorPosCmdWidget();
        case CmdType::DRAG:
            return new DragCmdWidget();
        case CmdType::SCROLL:
            return new ScrollCmdWidget();
        case CmdType::WRITETEXT:
            return new WriteTextCmdWidget();
        case CmdType::PRESSKEY:
            return new PressKeyCmdWidget();
        case CmdType::RUNEXE:
            return new RunExeCmdWidget();
        default:
            return new ApplyRegexCmdWidget();
    }
}

bool CmdWidget::eventFilter(QObject *object, QEvent *event)
{
    QWidget *w = qobject_cast<QWidget*>(object);
    if(w && event->type() == QEvent::Wheel && WheelEventWidgets.contains(w))
        return !w->hasFocus();
    return false;
}

void CmdWidget::InstallWheelEventFilters()
{
    QWidget *w;
    foreach(w, WheelEventWidgets)
        w->installEventFilter(this);
}

void CmdWidget::emitCmdChangedSignal()
{
    emit cmdChanged(this);
}
