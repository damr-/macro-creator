#include "defaultdelaywidget.h"
#include "ui_defaultdelaywidget.h"

DefaultDelayWidget::DefaultDelayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefaultDelayWidget)
{
    ui->setupUi(this);

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
    settings->amount = ui->amountBox->value();
    settings->timeScale = ui->timeScaleBox->currentIndex();
    return settings;
}

void DefaultDelayWidget::SetSettings(DefaultDelaySettings *settings)
{
    ui->amountBox->setValue(settings->amount);
    ui->timeScaleBox->setCurrentIndex(settings->timeScale);
}

void DefaultDelayWidget::Reset()
{
    DefaultDelaySettings *settings = new DefaultDelaySettings();
    settings->amount = 250;
    settings->timeScale = 1;
    SetSettings(settings);
}

void DefaultDelayWidget::EmitSettingsChanged()
{
    emit SettingsChanged();
}
