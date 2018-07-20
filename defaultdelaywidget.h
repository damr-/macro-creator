#ifndef DEFAULTDELAYWIDGET_H
#define DEFAULTDELAYWIDGET_H

#include <QWidget>

namespace Ui {
class DefaultDelayWidget;
}

class DefaultDelaySettings
{
public:
    bool enabled;
    int amount;
    int timeScale;
};

class DefaultDelayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DefaultDelayWidget(QWidget *parent = nullptr);
    ~DefaultDelayWidget();

    DefaultDelaySettings GetSettings();
    void SetSettings(DefaultDelaySettings *settings);

    bool IsEnabled();
    int GetAmount();
    int GetTimeScale();

signals:
    void SettingsChanged(DefaultDelaySettings settings);

private slots:
    void Toggle();
    void EmitSettingsChanged();

private:
    Ui::DefaultDelayWidget *ui;
};

#endif // DEFAULTDELAYWIDGET_H
