#include "defaultdelaywidget.h"
#include "ui_defaultdelaywidget.h"

#include <QDebug>

DefaultDelayWidget::DefaultDelayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefaultDelayWidget)
{
    ui->setupUi(this);

    connect(ui->checkBox, SIGNAL(toggled(bool)), this, SLOT(Toggle()));
    connect(ui->amountBox, SIGNAL(valueChanged(int)), this, SLOT(EmitSettingsChanged()));
    connect(ui->timeScaleBox, SIGNAL(currentIndexChanged(int)), this, SLOT(EmitSettingsChanged()));
}

DefaultDelayWidget::~DefaultDelayWidget()
{
    delete ui;
}

DefaultDelaySettings *DefaultDelayWidget::GetSettings()
{
    DefaultDelaySettings *settings = new DefaultDelaySettings();
    settings->enabled = ui->checkBox->isChecked();
    settings->amount = ui->amountBox->value();
    settings->timeScale = ui->timeScaleBox->currentIndex();
    return settings;
}

void DefaultDelayWidget::SetSettings(DefaultDelaySettings *settings)
{
    ui->checkBox->setChecked(settings->enabled);
    ui->amountBox->setValue(settings->amount);
    ui->timeScaleBox->setCurrentIndex(settings->timeScale);
}

void DefaultDelayWidget::Reset()
{
    DefaultDelaySettings *settings = new DefaultDelaySettings();
    settings->enabled = false;
    settings->amount = 250;
    settings->timeScale = 1;
    SetSettings(settings);
}

void DefaultDelayWidget::Toggle()
{
    bool enabled = !ui->amountBox->isEnabled();
    ui->amountBox->setEnabled(enabled);
    ui->timeScaleBox->setEnabled(enabled);
    EmitSettingsChanged();
}

void DefaultDelayWidget::EmitSettingsChanged()
{
    emit SettingsChanged();
}
