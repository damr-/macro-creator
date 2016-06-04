#include "newcommanddialog.h"
#include "ui_newcommanddialog.h"
#include "clickcommandwidget.h"
#include "commandwidget.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QDebug>

NewCommandDialog::NewCommandDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewCommandDialog)
{
	ui->setupUi(this);

	connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okayClicked()));
}

NewCommandDialog::~NewCommandDialog()
{
	delete ui;
}


void NewCommandDialog::okayClicked()
{
	this->accept();
}

bool NewCommandDialog::getResult(int &result){
	result = 0;
	return true;
}
