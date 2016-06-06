#ifndef NEWCOMMANDDIALOG_H
#define NEWCOMMANDDIALOG_H

#include "commandwidget.h"
#include <QDialog>

namespace Ui {
class NewCommandDialog;
}

class NewCommandDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit NewCommandDialog(QWidget *parent = 0);
		~NewCommandDialog();
		bool getResult(int &result);

	private slots:
		void okayClicked();
        void commandSelectionChanged();

	private:
		Ui::NewCommandDialog *ui;
};

#endif // NEWCOMMANDDIALOG_H
