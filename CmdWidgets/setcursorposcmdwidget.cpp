#include "setcursorposcmdwidget.h"
#include "ui_setcursorposcmdwidget.h"

#include "commands.h"

SetCursorPosCmdWidget::SetCursorPosCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::SetCursorPosCmdWidget)
{
    ui->setupUi(this);
    int x = qrand() % (1920 + 1);
    int y = qrand() % (1920 + 1);
    SetCoordinates(x, y);    

    connect(ui->xCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->yCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));

    commandType = CmdType::CURPOS;
}

SetCursorPosCmdWidget::~SetCursorPosCmdWidget()
{
    delete ui;
}

void SetCursorPosCmdWidget::SetCoordinates(int x, int y)
{
    ui->xCoord->setValue(x);
    ui->yCoord->setValue(y);
}

void SetCursorPosCmdWidget::CopyTo(CmdWidget *other)
{
    SetCursorPosCmdWidget *widget = (SetCursorPosCmdWidget*)other;
    widget->SetCoordinates(GetX(), GetY());
}

QString SetCursorPosCmdWidget::GetCmdSafeString()
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
