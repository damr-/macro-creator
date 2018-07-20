#ifndef HITKEYCMDWIDGET_H
#define HITKEYCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class HitKeyCmdWidget;
}

class HitKeyCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit HitKeyCmdWidget(QWidget *parent = nullptr);
        ~HitKeyCmdWidget();
        void keyPressEvent(QKeyEvent* e);

        void CopyTo(CmdWidget *other);
        QString GetCmdSafeString();

    private:
        Ui::HitKeyCmdWidget *ui;
};

#endif // HITKEYCMDWIDGET_H
