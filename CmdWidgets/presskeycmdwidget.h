#ifndef PRESSKEYCMDWIDGET_H
#define PRESSKEYCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class PressKeyCmdWidget;
}

class PressKeyCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit PressKeyCmdWidget(QWidget *parent = nullptr);
        ~PressKeyCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();
        int GetCmdStrLen();
        bool IsValidCmd();

        class Modifiers
        {
        public:
            Modifiers()
            {
                CTRL = false;
                SHIFT = false;
                ALT = false;
            }

            Modifiers(bool ctrl, bool shift, bool alt)
            {
                CTRL = ctrl;
                SHIFT = shift;
                ALT = alt;
            }

            bool CTRL, SHIFT, ALT;
            static const int CTRLIdx = 0;
            static const int SHIFTIdx = 1;
            static const int ALTIdx = 2;
        };

        void SetCmdSettings(Modifiers modifiers, int keyType, QString letter, QString keySequenceLetter, int specialKeyIndex);
        Modifiers GetModifiers();
        int GetKeyType();
        QString GetLetter();
        QString GetKeySequenceLetter();
        int GetSpecialKeyIndex();

        static const int ModCTRLIdx = 1;
        static const int ModSHIFTIdx = 2;
        static const int ModALTIdx = 3;
        static const int KeyTypeIdx = 4;
        static const int LetterIdx = 5;
        static const int SeqLetterIdx = 6;
        static const int SpcKeyIndexIdx = 7;

    private slots:
        void truncateKeySequence();
        void clearKeySequence();
        void updateVisibility();

    private:
        Ui::PressKeyCmdWidget *ui;
};

#endif // PRESSKEYCMDWIDGET_H
