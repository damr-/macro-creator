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

        int GetCmdStringLen();

        CmdType GetCmdType();
        void SetRowNumber(int number);
        int GetRowNumber();

        static CmdWidget* GetNewCmdWidget(CmdType cmdType);

        static const int CmdTypeIdx = 0;

    protected:
        CmdType cmdType;

    protected slots:
        void emitCmdChangedSignal();

    signals:
        void cmdChanged(CmdWidget *);

    private:
        Ui::CmdWidget *ui;
};

#endif // CMDWIDGET_H
