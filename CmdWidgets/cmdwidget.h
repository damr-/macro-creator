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
        virtual void ToggleLocked() = 0;

        virtual bool IsValidCmd();

        CmdType GetCmdType();
        void SetRowNumber(int number);
        int GetRowNumber();
        int GetCmdStringLen();
        void ToggleEnabled();
        void SetCmdStates(bool isLocked, bool isDisabled);

        static CmdWidget* GetNewCmdWidget(CmdType cmdType);

        static const int CmdTypeIdx = 0;
        static const int LockedStateIdx = 1;
        static const int DisabledStateIdx = 2;

    protected:
        CmdType cmdType;
        bool isLocked;

    protected slots:
        void emitCmdChangedSignal();
        void unlock();

    signals:
        void cmdChanged(CmdWidget*);

    private:
        Ui::CmdWidget *ui;
};

#endif // CMDWIDGET_H
