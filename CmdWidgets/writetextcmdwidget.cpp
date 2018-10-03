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

    connect(ui->textTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVisibility()));

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

    ui->textLineEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9,. ]+"), this));

    updateVisibility();
}

WriteTextCmdWidget::~WriteTextCmdWidget()
{
    delete ui;
}

void WriteTextCmdWidget::CopyTo(CmdWidget *other)
{
    WriteTextCmdWidget *widget = qobject_cast<WriteTextCmdWidget*>(other);
    widget->SetCmdSettings(GetIsRandom(), GetPossibleChars(), GetRandomAmount(), GetText());
    CmdWidget::CopyTo(widget);
}

QString WriteTextCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + QString::number(GetIsRandom()) + "|" + GetPossibleChars() + "|" + QString::number(GetRandomAmount()) + "|" +  GetText();
}

void WriteTextCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->textTypeComboBox->setEnabled(!isLocked);
    ui->possibleCharsLineEdit->setEnabled(!isLocked);
    ui->randomAmountSpinBox->setEnabled(!isLocked);
    ui->textLineEdit->setEnabled(!isLocked);
    ui->toolButton->setEnabled(!isLocked);
}

void WriteTextCmdWidget::SetSettings(QStringList settings)
{
    SetCmdSettings(settings[TypeIdx].toInt(), settings[CharsIdx], settings[AmountIdx].toInt(), settings[TextIdx]);
}

bool WriteTextCmdWidget::IsValidCmd()
{
    return (!GetIsRandom() && !GetText().isEmpty()) || (GetIsRandom() && !GetPossibleChars().isEmpty());
}

int WriteTextCmdWidget::GetIsRandom()
{
    return ui->textTypeComboBox->currentIndex() == WriteTextType::Random;
}

QString WriteTextCmdWidget::GetPossibleChars()
{
    return ui->possibleCharsLineEdit->text();
}

int WriteTextCmdWidget::GetRandomAmount()
{
    return ui->randomAmountSpinBox->value();
}

QString WriteTextCmdWidget::GetText()
{
    return ui->textLineEdit->text();
}

void WriteTextCmdWidget::SetCmdSettings(bool isRandom, QString possibleChars, int randomAmount, QString text)
{
    ui->textTypeComboBox->setCurrentIndex(isRandom);
    ui->possibleCharsLineEdit->setText(possibleChars);
    ui->randomAmountSpinBox->setValue(randomAmount);
    ui->textLineEdit->setText(text);
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
}

void WriteTextCmdWidget::applyPreset(int presetIndex)
{
    ui->possibleCharsLineEdit->setText(Presets.at(presetIndex));
    ui->toolButton->menu()->close();
}
