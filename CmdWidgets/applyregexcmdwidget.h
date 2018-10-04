#ifndef APPLYREGEXCMDWIDGET_H
#define APPLYREGEXCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class ApplyRegexCmdWidget;
}

class ApplyRegexCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit ApplyRegexCmdWidget(QWidget *parent = nullptr);
        ~ApplyRegexCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void ToggleLocked() override;
        void SetSettings(QStringList settings) override;

        bool IsValidCmd() override;

        QByteArray GetRegexHex();
        bool GetUsePaste();
        void SetCmdSettings(QString regex, bool usePaste);

        static const int RegexIdx = 3;
        static const int PasteIdx = 4;

    private:
        Ui::ApplyRegexCmdWidget *ui;
};

#endif // APPLYREGEXCMDWIDGET_H
