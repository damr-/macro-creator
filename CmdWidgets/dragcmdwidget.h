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
        void SetSettings(QStringList settings) override;

        int GetX();
        int GetY();
        void SetCmdSettings(int x, int y);

        static const int XIdx = ChildIdxStart;
        static const int YIdx = ChildIdxStart + 1;

    protected:
        bool eventFilter(QObject *object, QEvent *event) override;

    signals:
        void showPosHint(bool, int, int);

    private:
        Ui::DragCmdWidget *ui;
};

#endif // DRAGCMDWIDGET_H
