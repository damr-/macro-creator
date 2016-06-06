#include "newcommanddialog.h"
#include "ui_newcommanddialog.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QDebug>

#include "clickcommandwidget.h"
#include "commandwidget.h"
#include "commands.h"

NewCommandDialog::NewCommandDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewCommandDialog)
{
	ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

	connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okayClicked()));

    foreach(QString name, Commands::commandNames())
    {
        ui->commandSelectBox->addItem(name);
    }

    connect(ui->commandSelectBox, SIGNAL(currentIndexChanged(int)), this, SLOT(commandSelectionChanged()));
}

NewCommandDialog::~NewCommandDialog()
{
	delete ui;
}


void NewCommandDialog::okayClicked()
{
	this->accept();
}

void NewCommandDialog::commandSelectionChanged()
{

}

bool NewCommandDialog::getResult(int &result)
{
    result = ui->commandSelectBox->currentIndex();
	return true;
}
