#include "dragcommandwidget.h"
#include "ui_dragcommandwidget.h"

#include "commands.h"

DragCommandWidget::DragCommandWidget(QWidget *parent) :
    CommandWidget(parent),
    ui(new Ui::DragCommandWidget)
{
    ui->setupUi(this);

    connect(ui->xCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->yCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));

    commandType = CommandType::Drag;
}

DragCommandWidget::~DragCommandWidget()
{
    delete ui;
}

void DragCommandWidget::CopyTo(CommandWidget *other)
{
    ((DragCommandWidget*)other)->SetCoordinates(GetX(), GetY());
}

QString DragCommandWidget::GetCommandString()
{
    return QString::number((int)CommandType::Drag) + "|" + QString::number(GetX()) + "|" + QString::number(GetY());
}

int DragCommandWidget::GetX()
{
    return ui->xCoord->value();
}

int DragCommandWidget::GetY()
{
    return ui->yCoord->value();
}

void DragCommandWidget::SetCoordinates(int x, int y)
{
    ui->xCoord->setValue(x);
    ui->yCoord->setValue(y);
}
