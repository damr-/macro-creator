#ifndef WRITETEXTCMDWIDGET_H
#define WRITETEXTCMDWIDGET_H

#include "cmdwidget.h"

#include <QList>
#include <QString>

namespace Ui {
class WriteTextCmdWidget;
}

class WriteTextCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit WriteTextCmdWidget(QWidget *parent = nullptr);
        ~WriteTextCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();
        void ToggleLocked();
        void SetSettings(QStringList settings);

        bool IsValidCmd();

        int GetIsRandom();
        QString GetPossibleChars();
        int GetRandomAmount();
        QString GetText();
        void SetCmdSettings(bool isRandom, QString possibleChars, int randomAmount, QString text);

        static const int TypeIdx = 3;
        static const int CharsIdx = 4;
        static const int AmountIdx = 5;
        static const int TextIdx = 6;

    private slots:
        void updateVisibility();
        void applyPreset(int presetIndex);

    private:
        Ui::WriteTextCmdWidget *ui;

        int randomCBDefaultX = 240;
        int randomCBRandomX = 300;

        QString alphabet = "abcdefghijklmnopqrstuvwxyz ";
        QString numbers = "0123456789";
        QString punctuation = "!?.,";

        QList<QString> Presets = QList<QString>{
            alphabet,
            alphabet + numbers,
            alphabet + punctuation,
            alphabet + numbers + punctuation
        };
};

#endif // WRITETEXTCMDWIDGET_H
