#ifndef DRAGCMDWIDGET_H
#define DRAGCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class DragCmdWidget;
}

class DragCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit DragCmdWidget(QWidget *parent = nullptr);
        ~DragCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();

        int GetX();
        int GetY();
        void SetCoordinates(int x, int y);

    private:
        Ui::DragCmdWidget *ui;
};

#endif // DRAGCMDWIDGET_H
