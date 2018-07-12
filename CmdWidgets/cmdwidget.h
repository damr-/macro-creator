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
        explicit CmdWidget(QWidget *parent = 0);
        ~CmdWidget();

        CommandType commandType;

        virtual void CopyTo(CmdWidget *other);
        virtual QString GetCommandString();
        static CmdWidget* GetNewCommandWidget(int commandIndex);

    protected slots:
        void emitCommandChangedSignal();

    signals:
        void commandChanged();

	private:
        Ui::CmdWidget *ui;
};

#endif // CMDWIDGET_H
