#include "dragcmdwidget.h"
#include "ui_dragcmdwidget.h"

#include "commands.h"

DragCmdWidget::DragCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::DragCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::DRAG;

    connect(ui->xCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->yCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));    
}

DragCmdWidget::~DragCmdWidget()
{
    delete ui;
}

void DragCmdWidget::CopyTo(CmdWidget *other)
{
    qobject_cast<DragCmdWidget*>(other)->SetCoordinates(GetX(), GetY());
}

QString DragCmdWidget::GetCmdSafeString()
{
    return QString::number(int(CmdType::DRAG)) + "|" + QString::number(GetX()) + "|" + QString::number(GetY());
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
