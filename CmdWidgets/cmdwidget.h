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
        ~CmdWidget() override;

        virtual void CopyTo(CmdWidget *other) = 0;        
        virtual QString GetCmdString() = 0;
        virtual void SetSettings(QStringList settings) = 0;

        virtual bool IsValidCmd();

        CmdType GetCmdType();
        void SetRowNumber(int number);
        int GetRowNumber();
        int GetCmdStringLen();
        void ToggleEnabled();
        void SetDisabled(bool isDisabled);

        static CmdWidget* GetNewCmdWidget(CmdType cmdType);

        static const int CmdTypeIdx = 0;
        static const int DisabledStateIdx = 1;

        static const int ChildIdxStart = 2;

        static const QString FromHex(QString string) {return QByteArray::fromHex(string.toUtf8()); }

    protected:
        CmdType cmdType;
        QList<QWidget*>WheelEventWidgets = QList<QWidget*>{};
        bool eventFilter(QObject *object, QEvent *event) override;
        void InstallWheelEventFilters();

    protected slots:
        void emitCmdChangedSignal();

    signals:
        void cmdChanged(CmdWidget*);

    private:
        Ui::CmdWidget *ui;
};

#endif // CMDWIDGET_H
