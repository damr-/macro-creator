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
        ~RunExeCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();
        void ToggleLocked();
        void SetSettings(QStringList settings);

        bool IsValidCmd();

        QString GetFilePath();
        void SetCmdSettings(QString filePath);

        static const int PathIdx = 3;

    private slots:
        void chooseExe();

    private:
        Ui::RunExeCmdWidget *ui;
        QString currentFilePath = QString("");
};

#endif // RUNEXECMDWIDGET_H
