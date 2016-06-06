#include "setcursorposcommandwidget.h"
#include "ui_setcursorposcommandwidget.h"

SetCursorPosCommandWidget::SetCursorPosCommandWidget(QWidget *parent) :
    CommandWidget(parent),
    ui(new Ui::SetCursorPosCommandWidget)
{
    ui->setupUi(this);
    int x = qrand() % (1920 + 1);
    int y = qrand() % (1920 + 1);
    SetCoordinates(x, y);
}

SetCursorPosCommandWidget::~SetCursorPosCommandWidget()
{
    delete ui;
}

void SetCursorPosCommandWidget::SetCoordinates(int x, int y)
{
    ui->xCoord->setValue(x);
    ui->yCoord->setValue(y);
}

void SetCursorPosCommandWidget::CopyTo(CommandWidget *other)
{
    SetCursorPosCommandWidget *widget = (SetCursorPosCommandWidget*)other;
    widget->SetCoordinates(GetX(), GetY());
}

int SetCursorPosCommandWidget::GetX()
{
    return ui->xCoord->value();
}

int SetCursorPosCommandWidget::GetY()
{
    return ui->yCoord->value();
}
