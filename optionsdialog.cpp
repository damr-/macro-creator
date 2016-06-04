#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <qdir.h>
#include <qsettings.h>
#include <QPushButton>

#include "mainwindow.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

	connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okayClicked()));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::okayClicked()
{
	this->accept();
}
