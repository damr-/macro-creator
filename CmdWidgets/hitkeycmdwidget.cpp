#include "hitkeycmdwidget.h"
#include "ui_hitkeycmdwidget.h"

HitKeyCmdWidget::HitKeyCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::HitKeyCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::HITKEY;

    connect(ui->keySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->specialKeyCheckBox, SIGNAL(toggled(bool)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->specialKeyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCommandChangedSignal()));

    connect(ui->keySequenceEdit, SIGNAL(editingFinished()), this, SLOT(truncateKeySequence()));
    connect(ui->keySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(truncateKeySequence()));
    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(clearKeySequence()));
    connect(ui->specialKeyCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateVisibility()));
    updateVisibility();
}

HitKeyCmdWidget::~HitKeyCmdWidget()
{
    delete ui;
}

void HitKeyCmdWidget::CopyTo(CmdWidget *other)
{
    qobject_cast<HitKeyCmdWidget*>(other)->SetCmdSettings(GetIsSpecialKey(), GetSpecialKeyIndex(), GetKeySequence());
}

QString HitKeyCmdWidget::GetCmdString()
{
    return QString::number(int(cmdType)) + "|" + QString::number(GetIsSpecialKey()) + "|" + QString::number(GetSpecialKeyIndex()) + "|" + GetKeySequence().toString();
}

int HitKeyCmdWidget::GetCmdStrLen()
{
    return 4;
}

bool HitKeyCmdWidget::IsValidCmd()
{
    if(GetIsSpecialKey())
        return ui->specialKeyComboBox->currentIndex() > 0;
    else
        return !ui->keySequenceEdit->keySequence().isEmpty();
}

void HitKeyCmdWidget::SetCmdSettings(int isSpecialKey, int specialKeyIndex, QKeySequence keySequence)
{
    ui->specialKeyCheckBox->setChecked(isSpecialKey);
    ui->specialKeyComboBox->setCurrentIndex(specialKeyIndex);
    ui->keySequenceEdit->setKeySequence(keySequence);
    updateVisibility();
}

int HitKeyCmdWidget::GetIsSpecialKey()
{
    return ui->specialKeyCheckBox->isChecked();
}

int HitKeyCmdWidget::GetSpecialKeyIndex()
{
    return ui->specialKeyComboBox->currentIndex();
}

QKeySequence HitKeyCmdWidget::GetKeySequence()
{
    return ui->keySequenceEdit->keySequence();
}

void HitKeyCmdWidget::truncateKeySequence()
{
    QKeySequence sequence(ui->keySequenceEdit->keySequence()[0]);
    ui->keySequenceEdit->setKeySequence(sequence);
}

void HitKeyCmdWidget::clearKeySequence()
{
    ui->keySequenceEdit->clear();
}

void HitKeyCmdWidget::updateVisibility()
{
    ui->specialKeyComboBox->setVisible(GetIsSpecialKey());
    ui->keySequenceEdit->setVisible(!GetIsSpecialKey());
    ui->clearButton->setVisible(!GetIsSpecialKey());
}
