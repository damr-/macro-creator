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
        void SetSettings(QStringList settings) override;

        bool IsValidCmd() override;

        QByteArray GetRegexHex();
        bool GetUsePaste();
        void SetCmdSettings(QString regex, bool usePaste);

        static const int RegexIdx = ChildIdxStart;
        static const int PasteIdx = ChildIdxStart + 1;

    private:
        Ui::ApplyRegexCmdWidget *ui;
};

#endif // APPLYREGEXCMDWIDGET_H
