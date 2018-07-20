#include "hitkeycmdwidget.h"
#include "ui_hitkeycmdwidget.h"

HitKeyCmdWidget::HitKeyCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::HitKeyCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::HITKEY;

    connect(ui->keySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(emitCommandChangedSignal()));
}

HitKeyCmdWidget::~HitKeyCmdWidget()
{
    delete ui;
}

void HitKeyCmdWidget::CopyTo(CmdWidget *other)
{
    qobject_cast<HitKeyCmdWidget*>(other)->SetKeySequence(GetKeySequence());
}

QString HitKeyCmdWidget::GetCmdSafeString()
{
    return QString("HitKeyCmdWidget safestring");
}

bool HitKeyCmdWidget::IsValidCmd()
{
    return !ui->keySequenceEdit->keySequence().isEmpty();
}

void HitKeyCmdWidget::SetKeySequence(QKeySequence keySequence)
{
    ui->keySequenceEdit->setKeySequence(keySequence);
}

QKeySequence HitKeyCmdWidget::GetKeySequence()
{
    return ui->keySequenceEdit->keySequence();
}
