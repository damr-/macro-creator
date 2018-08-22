#include "presskeycmdwidget.h"
#include "ui_presskeycmdwidget.h"

PressKeyCmdWidget::PressKeyCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::PressKeyCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::PRESSKEY;

    //ui->letterLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-z0-9]"), this));

    connect(ui->keyTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVisibility()));

    connect(ui->modifierListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->keyTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->letterLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emitCmdChangedSignal()));
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
    qobject_cast<PressKeyCmdWidget*>(other)->SetCmdSettings(GetModifiers(), GetKeyType(), GetLetter(), GetKeySequenceLetter(), GetSpecialKeyIndex());
}

QString PressKeyCmdWidget::GetCmdString()
{
    Modifiers mod = GetModifiers();

    return QString::number(int(cmdType)) + "|" +
            QString::number(mod.CTRL) + "|" +
            QString::number(mod.SHIFT) + "|" +
            QString::number(mod.ALT) + "|" +
            QString::number(GetKeyType()) + "|" +
            GetLetter() + "|" +
            GetKeySequenceLetter() + "|" +
            QString::number(GetSpecialKeyIndex());
}

bool PressKeyCmdWidget::IsValidCmd()
{
    KeyType keyType = GetKeyType();

    if(keyType == KeyType::LETTER)
        return !ui->letterLineEdit->text().isEmpty();
    else if(keyType == KeyType::KEYDET)
        return !ui->keySequenceEdit->keySequence().isEmpty();
    else if(keyType == KeyType::SPECKEY)
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

QString PressKeyCmdWidget::GetLetter()
{
    return ui->letterLineEdit->text();
}

QString PressKeyCmdWidget::GetKeySequenceLetter()
{
    return ui->keySequenceEdit->keySequence().toString();
}

int PressKeyCmdWidget::GetSpecialKeyIndex()
{
    return ui->specialKeyComboBox->currentIndex();
}

void PressKeyCmdWidget::SetCmdSettings(Modifiers modifiers, KeyType keyType, QString letter, QString keySequenceLetter, int specialKeyIndex)
{
    if(modifiers.CTRL)
        ui->modifierListWidget->item(0)->setCheckState(Qt::CheckState::Checked);
    if(modifiers.SHIFT)
        ui->modifierListWidget->item(1)->setCheckState(Qt::CheckState::Checked);
    if(modifiers.ALT)
        ui->modifierListWidget->item(2)->setCheckState(Qt::CheckState::Checked);

    ui->keyTypeComboBox->setCurrentIndex(int(keyType));
    ui->letterLineEdit->setText(letter);
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

    if(keyTypeIndex == KeyType::LETTER)
        ui->letterLineEdit->raise();
    ui->letterLineEdit->setVisible(keyTypeIndex == KeyType::LETTER);


    if(keyTypeIndex == KeyType::KEYDET)
    {
        ui->keySequenceEdit->raise();
        ui->keySequenceClearButton->raise();
    }
    ui->keySequenceEdit->setVisible(keyTypeIndex == KeyType::KEYDET);
    ui->keySequenceClearButton->setVisible(keyTypeIndex == KeyType::KEYDET);

    if(keyTypeIndex == KeyType::SPECKEY)
        ui->specialKeyComboBox->raise();
    ui->specialKeyComboBox->setVisible(keyTypeIndex == KeyType::SPECKEY);
}
