#ifndef DRAGCOMMANDWIDGET_H
#define DRAGCOMMANDWIDGET_H

#include "commandwidget.h"

namespace Ui {
class DragCommandWidget;
}

class DragCommandWidget : public CommandWidget
{
    Q_OBJECT

    public:
        explicit DragCommandWidget(QWidget *parent = 0);
        ~DragCommandWidget();

        void CopyTo(CommandWidget *other);
        QString GetCommandString();

        int GetX();
        int GetY();
        void SetCoordinates(int x, int y);

    private:
        Ui::DragCommandWidget *ui;
};

#endif // DRAGCOMMANDWIDGET_H
