#ifndef NEWPROGRAMDIALOG_H
#define NEWPROGRAMDIALOG_H

#include <QDialog>

namespace Ui {
class NewProgramDialog;
}

class NewProgramDialog : public QDialog
{
    Q_OBJECT

public:
	explicit NewProgramDialog(QWidget *parent = 0);
    bool getResult(QString &newProgName);
	~NewProgramDialog();

private slots:
    void okayClicked();

private:
	Ui::NewProgramDialog *ui;
    bool success;
    QString newProgName;
};

#endif // NEWPROGRAMDIALOG_H
