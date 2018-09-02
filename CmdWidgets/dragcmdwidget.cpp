#include "dragcmdwidget.h"
#include "ui_dragcmdwidget.h"

#include "commands.h"

DragCmdWidget::DragCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::DragCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::DRAG;

    connect(ui->xCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->yCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));

    ui->coordGroupBox->installEventFilter(this);
}

DragCmdWidget::~DragCmdWidget()
{
    delete ui;
}

void DragCmdWidget::CopyTo(CmdWidget *other)
{
    DragCmdWidget *widget = qobject_cast<DragCmdWidget*>(other);
    widget->SetCmdSettings(GetX(), GetY());
    CmdWidget::CopyTo(widget);
}

QString DragCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + QString::number(GetX()) + "|" + QString::number(GetY());
}

void DragCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->xCoord->setEnabled(!isLocked);
    ui->yCoord->setEnabled(!isLocked);
}

void DragCmdWidget::SetSettings(QStringList settings)
{
    SetCmdSettings(settings[XIdx].toInt(), settings[YIdx].toInt());
}

int DragCmdWidget::GetX()
{
    return ui->xCoord->value();
}

int DragCmdWidget::GetY()
{
    return ui->yCoord->value();
}

void DragCmdWidget::SetCmdSettings(int x, int y)
{
    ui->xCoord->setValue(x);
    ui->yCoord->setValue(y);
}

bool DragCmdWidget::eventFilter(QObject *object, QEvent *event)
{
    if(object != ui->coordGroupBox)
        return false;

    if (event->type() == QEvent::Enter)
        emit showPosHint(true, GetX(), GetY());
    if (event->type() == QEvent::Leave)
        emit showPosHint(false, GetX(), GetY());
    return false;
}
