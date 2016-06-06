#ifndef COMMANDWIDGET_H
#define COMMANDWIDGET_H

#include <QWidget>

namespace Ui {
class CommandWidget;
}

class CommandWidget : public QWidget
{
    Q_OBJECT

	public:
        explicit CommandWidget(QWidget *parent = 0);
		~CommandWidget();

        static int index;

        virtual void CopyTo(CommandWidget *other);
        virtual QString GetCommandString();

    protected slots:
        void emitCommandChangedSignal();

    signals:
        void commandChanged();

	private:
        Ui::CommandWidget *ui;
};

#endif // COMMANDWIDGET_H
