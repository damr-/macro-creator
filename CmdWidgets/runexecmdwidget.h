#ifndef RUNEXECMDWIDGET_H
#define RUNEXECMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class RunExeCmdWidget;
}

class RunExeCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit RunExeCmdWidget(QWidget *parent = nullptr);
        ~RunExeCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void ToggleLocked() override;
        void SetSettings(QStringList settings) override;

        bool IsValidCmd() override;

        QString GetFilePathHex();
        void SetCmdSettings(QString filePath);

        static const int PathIdx = 3;

    private slots:
        void chooseExe();

    private:
        Ui::RunExeCmdWidget *ui;
        QString currentFilePath = QString("");
};

#endif // RUNEXECMDWIDGET_H
