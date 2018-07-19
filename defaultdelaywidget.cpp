#include "defaultdelaywidget.h"
#include "ui_defaultdelaywidget.h"

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

DefaultDelaySettings DefaultDelayWidget::GetSettings()
{
    DefaultDelaySettings *settings = new DefaultDelaySettings();
    settings->enabled = ui->checkBox->isChecked();
    settings->amount = ui->amountBox->value();
    settings->timeScale = ui->timeScaleBox->currentIndex();
}

DefaultDelayWidget::SetSettings(DefaultDelaySettings *settings)
{
    ui->checkBox->setChecked(settings->enabled);
    ui->amountBox->setValue(settings->amount);
    ui->timeScaleBox->setCurrentIndex(settings->timeScale);
}

bool DefaultDelayWidget::IsEnabled()
{
    return ui->checkBox->isChecked();
}

int DefaultDelayWidget::GetAmount()
{
    return ui->amountBox->value();
}

int DefaultDelayWidget::GetTimeScale()
{
    return ui->timeScaleBox->currentIndex();
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
    emit SettingsChanged(GetSettings());
}
