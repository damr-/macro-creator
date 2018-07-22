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
        QString GetCmdString();
        bool IsValidCmd();

        void SetSettings(int isSpecialKey, int specialKeyIndex, QKeySequence keySequence);
        int GetIsSpecialKey();
        int GetSpecialKeyIndex();
        QKeySequence GetKeySequence();

        static const int KeySeqIdx = 3;

    private slots:
        void truncateKeySequence();
        void clearKeySequence();
        void updateVisibility();

    private:
        Ui::HitKeyCmdWidget *ui;
};

#endif // HITKEYCMDWIDGET_H
