#ifndef GOTOCMDWIDGET_H
#define GOTOCMDWIDGET_H

#include "cmdwidget.h"

namespace Ui {
class GotoCmdWidget;
}

class GotoCmdWidget : public CmdWidget
{
    Q_OBJECT

    public:
        explicit GotoCmdWidget(QWidget *parent = nullptr);
        ~GotoCmdWidget();

        void CopyTo(CmdWidget *other);
        QString GetCmdString();
        void ToggleLocked();
        void SetSettings(QStringList settings);

        bool IsValidCmd();

        int GetTargetRow();
        int GetAmount();
        void SetCmdSettings(int targetRow, int amount);

        void ValidateRowNumber(int cmdListCmdCount);

        int TotalAmount;

        static const int TargetRowIdx = 3;
        static const int AmountIdx = 4;

    private slots:
        void amountChanged(int amount);

    private:
        Ui::GotoCmdWidget *ui;
};

#endif // GOTOCMDWIDGET_H
