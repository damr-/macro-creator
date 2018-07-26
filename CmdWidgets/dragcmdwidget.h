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
        int GetCmdStrLen();

        int GetX();
        int GetY();
        void SetCmdSettings(int x, int y);

        static const int XIdx = 1;
        static const int YIdx = 2;

    private:
        Ui::DragCmdWidget *ui;
};

#endif // DRAGCMDWIDGET_H
