#include "setcursorposcmdwidget.h"
#include "ui_setcursorposcmdwidget.h"

#include "commands.h"

SetCursorPosCmdWidget::SetCursorPosCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::SetCursorPosCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::SETCURSORPOS;

    connect(ui->xCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->yCoord, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->clickCheckBox, SIGNAL(toggled(bool)), this, SLOT(emitCmdChangedSignal()));

    ui->coordGroupBox->installEventFilter(this);

    WheelEventWidgets = QList<QWidget*>{ui->xCoord, ui->yCoord};
    InstallWheelEventFilters();
}

SetCursorPosCmdWidget::~SetCursorPosCmdWidget()
{
    delete ui;
}

void SetCursorPosCmdWidget::CopyTo(CmdWidget *other)
{
    SetCursorPosCmdWidget *widget = qobject_cast<SetCursorPosCmdWidget*>(other);
    widget->SetCmdSettings(GetX(), GetY(), GetAddClick());
    CmdWidget::CopyTo(widget);
}

QString SetCursorPosCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + QString::number(GetX()) + "|" + QString::number(GetY()) + "|" + QString::number(GetAddClick());
}

void SetCursorPosCmdWidget::SetSettings(QStringList settings)
{
    SetCmdSettings(settings[XIdx].toInt(),
                   settings[YIdx].toInt(),
                   settings[ClickIdx].toInt());
}

int SetCursorPosCmdWidget::GetX()
{
    return ui->xCoord->value();
}

int SetCursorPosCmdWidget::GetY()
{
    return ui->yCoord->value();
}

bool SetCursorPosCmdWidget::GetAddClick()
{
    return ui->clickCheckBox->isChecked();
}

void SetCursorPosCmdWidget::SetCmdSettings(int x, int y, bool addClick)
{
    ui->xCoord->setValue(x);
    ui->yCoord->setValue(y);
    ui->clickCheckBox->setChecked(addClick);
}

bool SetCursorPosCmdWidget::eventFilter(QObject *object, QEvent *event)
{
    if(CmdWidget::eventFilter(object, event))
        return true;

    if(object != ui->coordGroupBox)
        return false;

    if (event->type() == QEvent::Enter)
        emit showPosHint(true, GetX(), GetY());
    if (event->type() == QEvent::Leave)
        emit showPosHint(false, GetX(), GetY());
    return false;
}
