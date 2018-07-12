#include "dragcmdwidget.h"
#include "ui_dragcommandwidget.h"

#include "commands.h"

DragCmdWidget::DragCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::DragCmdWidget)
{
    ui->setupUi(this);

    connect(ui->xCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->yCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));

    commandType = CommandType::Drag;
}

DragCmdWidget::~DragCmdWidget()
{
    delete ui;
}

void DragCmdWidget::CopyTo(CmdWidget *other)
{
    ((DragCmdWidget*)other)->SetCoordinates(GetX(), GetY());
}

QString DragCmdWidget::GetCommandString()
{
    return QString::number((int)CommandType::Drag) + "|" + QString::number(GetX()) + "|" + QString::number(GetY());
}

int DragCmdWidget::GetX()
{
    return ui->xCoord->value();
}

int DragCmdWidget::GetY()
{
    return ui->yCoord->value();
}

void DragCmdWidget::SetCoordinates(int x, int y)
{
    ui->xCoord->setValue(x);
    ui->yCoord->setValue(y);
}
