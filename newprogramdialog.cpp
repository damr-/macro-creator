#include "newprogramdialog.h"
#include "ui_newprogramdialog.h"

#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QDialogButtonBox>

#include "startup.h"

QRegExp nameRX("[A-Za-z]+[A-Za-z0-9_]*");

NewProgramDialog::NewProgramDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::NewProgramDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

	connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okayClicked()));

    ui->nameEdit->setValidator(new QRegExpValidator (nameRX, this));
    success = false;
    newProgName = "";
}

void NewProgramDialog::okayClicked()
{
    QString statusText = "";
    newProgName = ui->nameEdit->text().trimmed();
    ui->statusLabel->setText("");

    success = true;

    if(!ui->nameEdit->hasAcceptableInput() || newProgName == startup::progListFileName || newProgName == "unnamed") {
        success = false;
        statusText = "'" + newProgName + "' is not valid.";
    }

    if(newProgName.length() == 0){
        success = false;
        statusText = "No name given";
    }

    QFile f(startup::progListDirectory);

    if(f.open(QFile::ReadOnly)){
        QTextStream input(&f);
        QString line;

        do {
            line = input.readLine();
            if(line == newProgName && line.length() > 0)
            {
                success = false;
                statusText = "'" + newProgName + "' already exists.";
                break;
            }
        }while(!line.isNull());

        f.close();
    }

    if(!success){
        ui->statusLabel->setText(statusText);
		ui->nameEdit->setFocus();
		ui->nameEdit->selectAll();
        return;
    }
    this->accept();
}

NewProgramDialog::~NewProgramDialog()
{
    delete ui;
}

bool NewProgramDialog::getResult(QString &progName){
    progName = this->newProgName;
	return success;
}
