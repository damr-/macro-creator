#ifndef DEFAULTDELAYWIDGET_H
#define DEFAULTDELAYWIDGET_H

#include <QWidget>

namespace Ui {
    class DefaultDelayWidget;
}

class DefaultDelaySettings
{
public:
    int amount;
    int timeScale;

    static const int AmountIdx = 0;
    static const int TimeScaleIdx = 1;
};

class DefaultDelayWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit DefaultDelayWidget(QWidget *parent = nullptr);
        ~DefaultDelayWidget();

        DefaultDelaySettings *GetSettings();
        void SetSettings(DefaultDelaySettings *settings);
        void Reset();

    signals:
        void SettingsChanged();

    private slots:
        void EmitSettingsChanged();

    private:
    Ui::DefaultDelayWidget *ui;
};

#endif // DEFAULTDELAYWIDGET_H
