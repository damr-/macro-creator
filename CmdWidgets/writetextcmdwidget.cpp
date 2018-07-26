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

    connect(ui->textLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->randomTextCheckBox, SIGNAL(toggled(bool)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->randomAmountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(emitCommandChangedSignal()));
    connect(ui->possibleCharsLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emitCommandChangedSignal()));

    connect(ui->randomTextCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateVisibility()));
    connect(ui->randomAmountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateGrammar()));

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
    qobject_cast<WriteTextCmdWidget*>(other)->SetCmdSettings(GetIsRandom(), GetPossibleChars(), GetRandomAmount(), GetText());
}

QString WriteTextCmdWidget::GetCmdString()
{
    return QString::number(int(cmdType)) + "|" + QString::number(GetIsRandom()) + "|" + GetPossibleChars() + "|" + QString::number(GetRandomAmount()) + "|" +  GetText();
}

int WriteTextCmdWidget::GetCmdStrLen()
{
    return 5;
}

bool WriteTextCmdWidget::IsValidCmd()
{
    return (!GetIsRandom() && !GetText().isEmpty()) || (GetIsRandom() && !GetPossibleChars().isEmpty());
}

bool WriteTextCmdWidget::GetIsRandom()
{
    return ui->randomTextCheckBox->isChecked();
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
    ui->randomTextCheckBox->setChecked(isRandom);
    ui->possibleCharsLineEdit->setText(possibleChars);
    ui->randomAmountSpinBox->setValue(randomAmount);
    ui->textLineEdit->setText(text);
}

void WriteTextCmdWidget::updateVisibility()
{
    ui->textLineEdit->setVisible(!GetIsRandom());
    ui->possibleCharsLineEdit->setVisible(GetIsRandom());
    ui->randomAmountSpinBox->setVisible(GetIsRandom());
    ui->usingTextLabel->setVisible(GetIsRandom());
    ui->toolButton->setVisible(GetIsRandom());

    QRect rect = ui->randomTextCheckBox->geometry();
    ui->randomTextCheckBox->setGeometry((GetIsRandom() ? randomCBRandomX : randomCBDefaultX), rect.y(), rect.width(), rect.height());
}

void WriteTextCmdWidget::updateGrammar()
{
    QString add = ui->randomAmountSpinBox->value() > 1 ? "s" : "";
    ui->usingTextLabel->setText("character" + add + " using");
}

void WriteTextCmdWidget::applyPreset(int presetIndex)
{
    ui->possibleCharsLineEdit->setText(Presets.at(presetIndex));
    ui->toolButton->menu()->close();
}
