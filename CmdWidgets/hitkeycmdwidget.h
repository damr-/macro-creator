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

        void CopyTo(CmdWidget *other);
        QString GetCmdSafeString();
        bool IsValidCmd();

        void SetKeySequence(QKeySequence keySequence);
        QKeySequence GetKeySequence();

    private:
        Ui::HitKeyCmdWidget *ui;
};

#endif // HITKEYCMDWIDGET_H
