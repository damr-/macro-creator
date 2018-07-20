#include "cmdwidget.h"
#include "ui_cmdwidget.h"

#include "clickcmdwidget.h"
#include "keyboardutilities.h"
#include "setcursorposcmdwidget.h"
#include "delaycmdwidget.h"
#include "dragcmdwidget.h"
#include "scrollcmdwidget.h"
#include "writetextcmdwidget.h"
#include "hitkeycmdwidget.h"

CmdWidget* CmdWidget::GetNewCommandWidget(CmdType cmdType)
{
    switch(cmdType){
        case CmdType::DELAY:
            return new DelayCmdWidget();
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
        case CmdType::HITKEY:
            return new HitKeyCmdWidget();
        case CmdType::OPENEXE: //TODO
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

}

QString CmdWidget::GetCmdSafeString()
{
    return "";
}

void CmdWidget::emitCommandChangedSignal()
{
    emit commandChanged();
}
