#ifndef PRESSKEYCMDWIDGET_H
#define PRESSKEYCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class PressKeyCmdWidget;
}

enum KeyType
{
    NOTHING = 0,
    LETTER = 1,
    KEYDET = 2,
    SPECKEY = 3
};

class PressKeyCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit PressKeyCmdWidget(QWidget *parent = nullptr);
        ~PressKeyCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();
        void ToggleLocked();
        void SetSettings(QStringList settings);

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

        Modifiers GetModifiers();
        KeyType GetKeyType();
        QString GetLetter();
        QString GetKeySequenceLetter();
        int GetSpecialKeyIndex();
        void SetCmdSettings(Modifiers modifiers, KeyType keyType, QString letter, QString keySequenceLetter, int specialKeyIndex);

        static const int ModCTRLIdx = 3;
        static const int ModSHIFTIdx = 4;
        static const int ModALTIdx = 5;
        static const int KeyTypeIdx = 6;
        static const int LetterIdx = 7;
        static const int SeqLetterIdx = 8;
        static const int SpcKeyIndexIdx = 9;

    private slots:
        void truncateKeySequence();
        void clearKeySequence();
        void updateVisibility();

    private:
        Ui::PressKeyCmdWidget *ui;
};

#endif // PRESSKEYCMDWIDGET_H
