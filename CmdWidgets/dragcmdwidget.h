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
        ~DragCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void ToggleLocked() override;
        void SetSettings(QStringList settings) override;

        int GetX();
        int GetY();
        void SetCmdSettings(int x, int y);

        static const int XIdx = 3;
        static const int YIdx = 4;

    protected:
        bool eventFilter(QObject *object, QEvent *event);

    signals:
        void showPosHint(bool, int, int);

    private:
        Ui::DragCmdWidget *ui;
};

#endif // DRAGCMDWIDGET_H
