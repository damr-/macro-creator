#include "writetextcmdwidget.h"
#include "ui_writetextcmdwidget.h"

#include <QRegExpValidator>
#include <QWidgetAction>
#include <QPushButton>
#include <QMenu>
#include <QSignalMapper>

WriteTextCmdWidget::WriteTextCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::WriteTextCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::WRITETEXT;

    connect(ui->textLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->textTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->randomAmountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->possibleCharsLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emitCmdChangedSignal()));
    connect(ui->pasteCheckBox, SIGNAL(toggled(bool)), this, SLOT(emitCmdChangedSignal()));

    connect(ui->textTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVisibility()));

    connect(ui->pasteCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateAllowedChars()));

    //
    QMenu *menu = new QMenu("Menu");
    QWidgetAction *a1 = new QWidgetAction(this);
    QWidgetAction *a2 = new QWidgetAction(this);
    QWidgetAction *a3 = new QWidgetAction(this);
    QWidgetAction *a4 = new QWidgetAction(this);

    QPushButton *b1 = new QPushButton("alphabet", menu);
    QPushButton *b2 = new QPushButton("alphabet + numbers", menu);
    QPushButton *b3 = new QPushButton("alphabet + punctuation", menu);
    QPushButton *b4 = new QPushButton("alphabet + numbers + punctuation", menu);
    b1->setFlat(true);
    b2->setFlat(true);
    b3->setFlat(true);
    b4->setFlat(true);

    QSignalMapper *addActionsMapper = new QSignalMapper(this);
    connect(b1, SIGNAL(clicked(bool)), addActionsMapper, SLOT(map()));
    connect(b2, SIGNAL(clicked(bool)), addActionsMapper, SLOT(map()));
    connect(b3, SIGNAL(clicked(bool)), addActionsMapper, SLOT(map()));
    connect(b4, SIGNAL(clicked(bool)), addActionsMapper, SLOT(map()));
    addActionsMapper->setMapping(b1, 0);
    addActionsMapper->setMapping(b2, 1);
    addActionsMapper->setMapping(b3, 2);
    addActionsMapper->setMapping(b4, 3);
    connect(addActionsMapper, SIGNAL(mapped(int)), this, SLOT(applyPreset(int)));

    a1->setDefaultWidget(b1);
    a2->setDefaultWidget(b2);
    a3->setDefaultWidget(b3);
    a4->setDefaultWidget(b4);

    menu->addAction(a1);
    menu->addAction(a2);
    menu->addAction(a3);
    menu->addAction(a4);
    ui->toolButton->setMenu(menu);
    //

    ui->textLineEdit->setValidator(new QRegExpValidator(WriteRegExp, this));
    ui->possibleCharsLineEdit->setValidator(new QRegExpValidator(WriteRegExp, this));

    updateVisibility();

    WheelEventWidgets = QList<QWidget*>{ui->textTypeComboBox, ui->randomAmountSpinBox};
    InstallWheelEventFilters();
}

WriteTextCmdWidget::~WriteTextCmdWidget()
{
    delete ui;
}

void WriteTextCmdWidget::CopyTo(CmdWidget *other)
{
    WriteTextCmdWidget *widget = qobject_cast<WriteTextCmdWidget*>(other);
    widget->SetCmdSettings(GetIsRandom(), GetPossibleCharsHex(), GetRandomAmount(), GetTextHex(), GetUsePaste());
    CmdWidget::CopyTo(widget);
}

QString WriteTextCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" +
            QString::number(GetIsRandom()) + "|" +
            GetPossibleCharsHex() + "|" +
            QString::number(GetRandomAmount()) + "|" +
            GetTextHex() + "|" +
            QString::number(GetUsePaste());
}

void WriteTextCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->textTypeComboBox->setEnabled(!isLocked);
    ui->possibleCharsLineEdit->setEnabled(!isLocked);
    ui->randomAmountSpinBox->setEnabled(!isLocked);
    ui->textLineEdit->setEnabled(!isLocked);
    ui->toolButton->setEnabled(!isLocked);
    ui->pasteCheckBox->setEnabled(!isLocked);
}

void WriteTextCmdWidget::SetSettings(QStringList settings)
{
    SetCmdSettings(settings[TypeIdx].toInt(),
                   settings[CharsIdx],
                   settings[AmountIdx].toInt(),
                   settings[TextIdx],
                   settings[PasteIdx].toInt());
}

bool WriteTextCmdWidget::IsValidCmd()
{
    return (!GetIsRandom() && !GetTextHex().isEmpty()) || (GetIsRandom() && !GetPossibleCharsHex().isEmpty());
}

int WriteTextCmdWidget::GetIsRandom()
{
    return ui->textTypeComboBox->currentIndex() == WriteTextType::Random;
}

QString WriteTextCmdWidget::GetPossibleCharsHex()
{
    return ui->possibleCharsLineEdit->text().toUtf8().toHex();
}

int WriteTextCmdWidget::GetRandomAmount()
{
    return ui->randomAmountSpinBox->value();
}

QString WriteTextCmdWidget::GetTextHex()
{
    return ui->textLineEdit->text().toUtf8().toHex();
}

bool WriteTextCmdWidget::GetUsePaste()
{
    return ui->pasteCheckBox->isChecked();
}

void WriteTextCmdWidget::SetCmdSettings(bool isRandom, QString possibleChars, int randomAmount, QString text, bool usePaste)
{
    ui->textTypeComboBox->setCurrentIndex(isRandom);
    ui->possibleCharsLineEdit->setText(CmdWidget::FromHex(possibleChars));
    ui->randomAmountSpinBox->setValue(randomAmount);
    ui->textLineEdit->setText(CmdWidget::FromHex(text));
    ui->pasteCheckBox->setChecked(usePaste);
}

void WriteTextCmdWidget::updateVisibility()
{
    if(GetIsRandom())
        ui->textLineEdit->lower();
    else
        ui->textLineEdit->raise();

    ui->textLineEdit->setVisible(!GetIsRandom());

    ui->possibleCharsLineEdit->setVisible(GetIsRandom());
    ui->randomAmountSpinBox->setVisible(GetIsRandom());
    ui->randomL2->setVisible(GetIsRandom());
    ui->toolButton->setVisible(GetIsRandom());
    ui->textLabel->setText(QString("text") + (GetIsRandom() ? " of length" : ":"));

    QRect rect = ui->pasteCheckBox->geometry();
    ui->pasteCheckBox->setGeometry(GetIsRandom() ? 490 : 400, rect.y(), rect.width(), rect.height());
}

void WriteTextCmdWidget::updateAllowedChars()
{
    if(GetIsRandom())
        cleanseLineEdit(ui->possibleCharsLineEdit);
    else
        cleanseLineEdit(ui->textLineEdit);
}

void WriteTextCmdWidget::applyPreset(int presetIndex)
{
    ui->possibleCharsLineEdit->setText(Presets.at(presetIndex));
    ui->toolButton->menu()->close();
}

void WriteTextCmdWidget::cleanseLineEdit(QLineEdit *lineEdit)
{
    if(GetUsePaste())
    {
        lineEdit->setValidator(nullptr);
        return;
    }

    QString text = cleanseText(lineEdit->text());
    lineEdit->setText(text);
    lineEdit->setValidator(new QRegExpValidator(WriteRegExp, this));
}

QString WriteTextCmdWidget::cleanseText(QString text)
{
    QString cleansedText = text;

    //Remove all characters from the text which are not allowed when simulating keystrokes
    int pos = 0;
    while((pos = NotWriteRegExp.indexIn(cleansedText)) != -1)
        cleansedText = cleansedText.remove(pos, 1);

    return cleansedText;
}
