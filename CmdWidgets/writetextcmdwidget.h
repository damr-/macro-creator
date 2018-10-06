#ifndef WRITETEXTCMDWIDGET_H
#define WRITETEXTCMDWIDGET_H

#include "cmdwidget.h"

#include <QList>
#include <QString>
#include <QLineEdit>

namespace Ui {
class WriteTextCmdWidget;
}

enum WriteTextType
{
    Given = 0,
    Random = 1
};

class WriteTextCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit WriteTextCmdWidget(QWidget *parent = nullptr);
        ~WriteTextCmdWidget() override;

        void CopyTo(CmdWidget *other) override;
        QString GetCmdString() override;
        void SetSettings(QStringList settings) override;

        bool IsValidCmd() override;

        int GetIsRandom();
        QString GetPossibleCharsHex();
        int GetRandomAmount();
        QString GetTextHex();
        bool GetUsePaste();
        void SetCmdSettings(bool isRandom, QString possibleChars, int randomAmount, QString text, bool usePaste);

        static const int TypeIdx = ChildIdxStart;
        static const int CharsIdx = ChildIdxStart + 1;
        static const int AmountIdx = ChildIdxStart + 2;
        static const int TextIdx = ChildIdxStart + 3;
        static const int PasteIdx = ChildIdxStart + 4;

        const QRegExp WriteRegExp = QRegExp("[A-Za-z0-9,. ]+");
        const QRegExp NotWriteRegExp = QRegExp("[^A-Za-z0-9,. ]");

    private slots:
        void updateVisibility();
        void updateAllowedChars();
        void applyPreset(int presetIndex);

    private:
        Ui::WriteTextCmdWidget *ui;

        void cleanseLineEdit(QLineEdit *lineEdit);
        QString cleanseText(QString text);

        int randomCBDefaultX = 240;
        int randomCBRandomX = 300;

        QString alphabet = "abcdefghijklmnopqrstuvwxyz ";
        QString numbers = "0123456789";
        QString punctuation = ".,";

        QList<QString> Presets = QList<QString>{
            alphabet,
            alphabet + numbers,
            alphabet + punctuation,
            alphabet + numbers + punctuation
        };
};

#endif // WRITETEXTCMDWIDGET_H
