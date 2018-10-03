#include "presskeycmdwidget.h"
#include "ui_presskeycmdwidget.h"

PressKeyCmdWidget::PressKeyCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::PressKeyCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::PRESSKEY;

    connect(ui->keyTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVisibility()));

    connect(ui->modifierListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->keyTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->keySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->specialKeyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCmdChangedSignal()));

    connect(ui->keySequenceEdit, SIGNAL(editingFinished()), this, SLOT(truncateKeySequence()));
    connect(ui->keySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(truncateKeySequence()));
    connect(ui->keySequenceClearButton, SIGNAL(clicked(bool)), this, SLOT(clearKeySequence()));

    updateVisibility();
}

PressKeyCmdWidget::~PressKeyCmdWidget()
{
    delete ui;
}

void PressKeyCmdWidget::CopyTo(CmdWidget *other)
{
    PressKeyCmdWidget *widget = qobject_cast<PressKeyCmdWidget*>(other);
    widget->SetCmdSettings(GetModifiers(), GetKeyType(), GetKeySequenceLetter(), GetSpecialKeyIndex());
    CmdWidget::CopyTo(widget);
}

QString PressKeyCmdWidget::GetCmdString()
{
    Modifiers mod = GetModifiers();

    return CmdWidget::GetCmdString() + "|" +
            QString::number(mod.CTRL) + "|" +
            QString::number(mod.SHIFT) + "|" +
            QString::number(mod.ALT) + "|" +
            QString::number(GetKeyType()) + "|" +
            GetKeySequenceLetter() + "|" +
            QString::number(GetSpecialKeyIndex());
}

void PressKeyCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->modifierListWidget->setEnabled(!isLocked);
    ui->keyTypeComboBox->setEnabled(!isLocked);
    ui->keySequenceEdit->setEnabled(!isLocked);
    ui->keySequenceClearButton->setEnabled(!isLocked);
    ui->specialKeyComboBox->setEnabled(!isLocked);
}

void PressKeyCmdWidget::SetSettings(QStringList settings)
{
    PressKeyCmdWidget::Modifiers mod =
            PressKeyCmdWidget::Modifiers(settings[ModCTRLIdx].toInt(), settings[ModSHIFTIdx].toInt(), settings[ModALTIdx].toInt());

    SetCmdSettings(mod,
                   KeyType(settings[KeyTypeIdx].toInt()),
                   settings[SeqLetterIdx],
                   settings[SpcKeyIndexIdx].toInt());
}

bool PressKeyCmdWidget::IsValidCmd()
{
    KeyType keyType = GetKeyType();

    if(keyType == KeyType::KEY)
        return !ui->keySequenceEdit->keySequence().isEmpty();
    else if(keyType == KeyType::SPECIAL)
        return ui->specialKeyComboBox->currentIndex() > 0;
    return true;
}

PressKeyCmdWidget::Modifiers PressKeyCmdWidget::GetModifiers()
{
    Modifiers mod = Modifiers();

    QListWidgetItem *item = ui->modifierListWidget->item(Modifiers::CTRLIdx);
    mod.CTRL = (item->checkState() == Qt::CheckState::Checked);
    item = ui->modifierListWidget->item(Modifiers::SHIFTIdx);
    mod.SHIFT = (item->checkState() == Qt::CheckState::Checked);
    item = ui->modifierListWidget->item(Modifiers::ALTIdx);
    mod.ALT = (item->checkState() == Qt::CheckState::Checked);

    return mod;
}

KeyType PressKeyCmdWidget::GetKeyType()
{
    return KeyType(ui->keyTypeComboBox->currentIndex());
}

QString PressKeyCmdWidget::GetKeySequenceLetter()
{
    return ui->keySequenceEdit->keySequence().toString();
}

int PressKeyCmdWidget::GetSpecialKeyIndex()
{
    return ui->specialKeyComboBox->currentIndex();
}

void PressKeyCmdWidget::SetCmdSettings(Modifiers modifiers, KeyType keyType, QString keySequenceLetter, int specialKeyIndex)
{
    if(modifiers.CTRL)
        ui->modifierListWidget->item(0)->setCheckState(Qt::CheckState::Checked);
    if(modifiers.SHIFT)
        ui->modifierListWidget->item(1)->setCheckState(Qt::CheckState::Checked);
    if(modifiers.ALT)
        ui->modifierListWidget->item(2)->setCheckState(Qt::CheckState::Checked);

    ui->keyTypeComboBox->setCurrentIndex(int(keyType));
    ui->keySequenceEdit->setKeySequence(QKeySequence::fromString(keySequenceLetter));
    ui->specialKeyComboBox->setCurrentIndex(specialKeyIndex);
}

void PressKeyCmdWidget::truncateKeySequence()
{
    QKeySequence sequence(ui->keySequenceEdit->keySequence()[0]);

    QString seq = sequence.toString();

    if(seq == "+")
        return;

    QString letter = seq.split('+').last();

    if(letter == "|" || letter == "F7")
    {
        ui->keySequenceEdit->clear();
        return;
    }
    ui->keySequenceEdit->setKeySequence(QKeySequence::fromString(letter));
}

void PressKeyCmdWidget::clearKeySequence()
{
    ui->keySequenceEdit->clear();
}

void PressKeyCmdWidget::updateVisibility()
{
    int keyTypeIndex = ui->keyTypeComboBox->currentIndex();

    if(keyTypeIndex == KeyType::KEY)
    {
        ui->keySequenceEdit->raise();
        ui->keySequenceClearButton->raise();
    }
    ui->keySequenceEdit->setVisible(keyTypeIndex == KeyType::KEY);
    ui->keySequenceClearButton->setVisible(keyTypeIndex == KeyType::KEY);

    if(keyTypeIndex == KeyType::SPECIAL)
        ui->specialKeyComboBox->raise();
    ui->specialKeyComboBox->setVisible(keyTypeIndex == KeyType::SPECIAL);
}
