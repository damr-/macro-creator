#ifndef PRESSKEYCMDWIDGET_H
#define PRESSKEYCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class PressKeyCmdWidget;
}

enum KeyType
{
    KEY = 0,
    SPECIAL = 1,
    NOTHING = 2
};

enum SpecialKeyType
{
    Tab = 0,
    Print = 1,
};

class PressKeyCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit PressKeyCmdWidget(QWidget *parent = nullptr);
        ~PressKeyCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void ToggleLocked() override;
        void SetSettings(QStringList settings) override;

        bool IsValidCmd() override;

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
        QString GetKeySequenceLetter();
        int GetSpecialKeyIndex();
        void SetCmdSettings(Modifiers modifiers, KeyType keyType, QString keySequenceLetter, int specialKeyIndex);

        static const int ModCTRLIdx = 3;
        static const int ModSHIFTIdx = 4;
        static const int ModALTIdx = 5;
        static const int KeyTypeIdx = 6;
        static const int SeqLetterIdx = 7;
        static const int SpcKeyIndexIdx = 8;

        static const QString GetCopyCmd() { return "4|0|0|1|0|0|0|C|0"; }
        static const QString GetPasteCmd() { return "4|0|0|1|0|0|0|V|0"; }

    private slots:
        void truncateKeySequence();
        void clearKeySequence();
        void updateVisibility();

    private:
        Ui::PressKeyCmdWidget *ui;
};

#endif // PRESSKEYCMDWIDGET_H
