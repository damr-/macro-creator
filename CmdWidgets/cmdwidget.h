#ifndef CMDWIDGET_H
#define CMDWIDGET_H

#include <QWidget>

#include "commands.h"

namespace Ui {
class CmdWidget;
}

class CmdWidget : public QWidget
{
    Q_OBJECT

	public:
        explicit CmdWidget(QWidget *parent = nullptr);
        ~CmdWidget();

        virtual void CopyTo(CmdWidget *other) = 0;
        virtual QString GetCmdString() = 0;
        virtual bool IsValidCmd();

        CmdType GetCmdType();
        void SetRowNumber(int number);

        static CmdWidget* GetNewCommandWidget(CmdType cmdType);

        static const int CmdTypeIdx = 0;

    protected:
        CmdType cmdType;

    protected slots:
        void emitCommandChangedSignal();

    signals:
        void commandChanged();

	private:
        Ui::CmdWidget *ui;
};

#endif // CMDWIDGET_H
