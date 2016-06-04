#include <QSignalMapper>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringList>
#include <QApplication>

#include "startup.h"
#include "ui_startup.h"
#include "mainwindow.h"
#include "newprogramdialog.h"
#include "progwindow.h"

#include <QDebug>

QString startup::progListFileName = "proglist";
QString startup::progListDirectory = (QDir::currentPath() + "/data/" + startup::progListFileName);
QString startup::currentProgName = "";

startup::startup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startup)
{
    ui->setupUi(this);
	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

    connect (ui->newButton, SIGNAL(clicked()), this, SLOT(newClicked())) ;
    connect (ui->openButton, SIGNAL(clicked()), this, SLOT(openClicked())) ;
    connect (ui->progList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(openClicked()));
    connect (ui->del, SIGNAL(clicked()), this, SLOT(deleteClicked()));

    if(!QDir("data").exists())
        QDir().mkdir("data");

    refreshProgramsDisplay();

    ui->progList->setFocus();
}

startup::~startup()
{
    delete ui;
}

void startup::cleanProgramList()
{
    QStringList *programList = new QStringList();

    QFile f(progListDirectory);
	if(f.open(QFile::ReadOnly)){
        QTextStream stream(&f);
        QString line;

        do{
            line = stream.readLine();
            if(line.length() > 0 && !programList->contains(line) && f.exists(QDir::currentPath() + "/data/" + line))
            {
                programList->append(line);
            }
        }while(!line.isNull());
        f.close();
    }

	if(!f.open(QFile::WriteOnly | QFile::Truncate))
		return;

	QTextStream output(&f);
	QString l;

	for(int i=0; i < programList->size(); ++i)
	{
		l.append(programList->at(i));
		l.append("\n");
	}

	output << l;
	f.close();
}

void startup::loadPrograms()
{
    QFile f(progListDirectory);
    if(!f.open(QFile::ReadOnly))
        return;

    QTextStream stream(&f);
    QString line;

    do{
        line = stream.readLine();
        if(line.length() > 0 && QFile::exists(QDir::currentPath() + "/data/" + line))
            ui->progList->addItem(line);
    }while(!line.isNull());

    ui->progList->setCurrentRow(0);
}

void startup::newClicked()
{
	NewProgramDialog newProgramDialog;
	if(!newProgramDialog.exec()){
		refreshProgramsDisplay();
		return;
	}

	QString progName = "";
	bool success = newProgramDialog.getResult(progName);

	if(!success)
		return;

	QFile file(progListDirectory);
	if(file.open(QFile::WriteOnly | QFile::Append)){
		QTextStream output(&file);

		QString text;
		text.append(progName);
		text.append("\n");

		output << text;

		file.close();
	}
    QFile progFile(QDir::currentPath() + "/data/" + progName);
    if(progFile.open(QFile::WriteOnly | QFile::Text))
        progFile.close();

    refreshProgramsDisplay();

    ui->progList->setCurrentRow(0);
}

void startup::openClicked()
{
    if(ui->progList->count() == 0)
        return;

    currentProgName = ui->progList->currentItem()->text().toLower();

    MainWindow *w = new MainWindow;
    w->show();
    this->close();
}

void startup::deleteClicked()
{
    if(ui->progList->count() == 0)
        return;

    QString fileDir = (QDir::currentPath() + "/data/" + ui->progList->currentItem()->text());
    QFile f(fileDir);
    f.remove();

    refreshProgramsDisplay();
}


void startup::refreshProgramsDisplay()
{
    cleanProgramList();
    ui->progList->clear();
    loadPrograms();

    bool buttonsActive = ui->progList->count() > 0;
    ui->openButton->setEnabled(buttonsActive);
    ui->del->setEnabled(buttonsActive);
}
