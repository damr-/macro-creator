#ifndef COMMANDWIDGET_H
#define COMMANDWIDGET_H

#include <QWidget>

#include "commands.h"

namespace Ui {
class CommandWidget;
}

class CommandWidget : public QWidget
{
    Q_OBJECT

	public:
        explicit CommandWidget(QWidget *parent = 0);
        ~CommandWidget();

        CommandType commandType;

        virtual void CopyTo(CommandWidget *other);
        virtual QString GetCommandString();
        static CommandWidget* GetNewCommandWidget(int commandIndex);

    protected slots:
        void emitCommandChangedSignal();

    signals:
        void commandChanged();

	private:
        Ui::CommandWidget *ui;
};

#endif // COMMANDWIDGET_H
