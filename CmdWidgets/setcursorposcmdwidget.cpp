#include "setcursorposcmdwidget.h"
#include "ui_setcursorposcmdwidget.h"

#include "commands.h"

SetCursorPosCmdWidget::SetCursorPosCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::SetCursorPosCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::CURPOS;

    connect(ui->xCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->yCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));

    ui->coordGroupBox->installEventFilter(this);
}

SetCursorPosCmdWidget::~SetCursorPosCmdWidget()
{
    delete ui;
}

void SetCursorPosCmdWidget::CopyTo(CmdWidget *other)
{
    SetCursorPosCmdWidget *widget = qobject_cast<SetCursorPosCmdWidget*>(other);
    widget->SetCmdSettings(GetX(), GetY());
    CmdWidget::CopyTo(widget);
}

QString SetCursorPosCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + QString::number(GetX()) + "|" + QString::number(GetY());
}

void SetCursorPosCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->xCoord->setEnabled(!isLocked);
    ui->yCoord->setEnabled(!isLocked);
}

int SetCursorPosCmdWidget::GetX()
{
    return ui->xCoord->value();
}

int SetCursorPosCmdWidget::GetY()
{
    return ui->yCoord->value();
}

void SetCursorPosCmdWidget::SetCmdSettings(int x, int y)
{
    ui->xCoord->setValue(x);
    ui->yCoord->setValue(y);
}

bool SetCursorPosCmdWidget::eventFilter(QObject *object, QEvent *event)
{
    if(object != ui->coordGroupBox)
        return false;

    if (event->type() == QEvent::Enter)
        emit showPosHint(true, GetX(), GetY());
    if (event->type() == QEvent::Leave)
        emit showPosHint(false, GetX(), GetY());
    return false;
}
