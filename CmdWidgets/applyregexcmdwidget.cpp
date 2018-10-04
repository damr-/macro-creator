#include "applyregexcmdwidget.h"
#include "ui_applyregexcmdwidget.h"

#include <QDebug>

ApplyRegexCmdWidget::ApplyRegexCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::ApplyRegexCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::REGEX;

    connect(ui->regexLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->pasteCheckBox, SIGNAL(toggled(bool)), this, SLOT(emitCmdChangedSignal()));
}

ApplyRegexCmdWidget::~ApplyRegexCmdWidget()
{
    delete ui;
}

void ApplyRegexCmdWidget::CopyTo(CmdWidget *other)
{
    ApplyRegexCmdWidget *widget = qobject_cast<ApplyRegexCmdWidget*>(other);
    widget->SetCmdSettings(GetRegexHex(), GetUsePaste());
    CmdWidget::CopyTo(widget);
}

QString ApplyRegexCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + GetRegexHex() + "|" + QString::number(GetUsePaste());
}

void ApplyRegexCmdWidget::ToggleLocked()
{
    ui->regexLineEdit->setEnabled(!isLocked);
    ui->pasteCheckBox->setEnabled(!isLocked);
}

void ApplyRegexCmdWidget::SetSettings(QStringList settings)
{
    SetCmdSettings(settings[ApplyRegexCmdWidget::RegexIdx], settings[ApplyRegexCmdWidget::PasteIdx].toInt());
}

bool ApplyRegexCmdWidget::IsValidCmd()
{
    return !GetRegexHex().isEmpty();
}

QByteArray ApplyRegexCmdWidget::GetRegexHex()
{
    return ui->regexLineEdit->text().toUtf8().toHex();
}

bool ApplyRegexCmdWidget::GetUsePaste()
{
    return ui->pasteCheckBox->isChecked();
}

void ApplyRegexCmdWidget::SetCmdSettings(QString regex, bool usePaste)
{
    ui->regexLineEdit->setText(CmdWidget::FromHex(regex));
    ui->pasteCheckBox->setChecked(usePaste);
}
