#include "setcursorposcmdwidget.h"
#include "ui_setcursorposcmdwidget.h"

#include "commands.h"

SetCursorPosCmdWidget::SetCursorPosCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::SetCursorPosCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::CURPOS;

    connect(ui->xCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->yCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
}

SetCursorPosCmdWidget::~SetCursorPosCmdWidget()
{
    delete ui;
}

void SetCursorPosCmdWidget::CopyTo(CmdWidget *other)
{
    qobject_cast<SetCursorPosCmdWidget*>(other)->SetCoordinates(GetX(), GetY());
}

QString SetCursorPosCmdWidget::GetCmdString()
{
    return QString::number(int(CmdType::CURPOS)) +  "|" + QString::number(GetX()) + "|" + QString::number(GetY());
}

int SetCursorPosCmdWidget::GetX()
{
    return ui->xCoord->value();
}

int SetCursorPosCmdWidget::GetY()
{
    return ui->yCoord->value();
}

void SetCursorPosCmdWidget::SetCoordinates(int x, int y)
{
    ui->xCoord->setValue(x);
    ui->yCoord->setValue(y);
}
