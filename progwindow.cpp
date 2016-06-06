#include "progwindow.h"
#include "ui_progwindow.h"
#include "startup.h"
#include "optionsdialog.h"
#include "newcommanddialog.h"
#include "clickcommandwidget.h"

#include <QSettings>
#include <QMessageBox>
#include <QPushButton>
#include <QDir>
#include <QTextStream>
#include <QDebug>

const QString ProgWindow::settingsFileName = "settings.ini";

ProgWindow::ProgWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProgWindow)
{
    ui->setupUi(this);
	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

	//Setup File actions
	connect(ui->actionNew_Open, SIGNAL(triggered()), this, SLOT(newOpenProgram()));
    //connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveCommandList()));
	connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(showOptions()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitProgram()));

	//Setup Edit actions
	connect(ui->actionNew_Command, SIGNAL(triggered()), this, SLOT(addNew()));
	connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteSelected()));
	connect(ui->actionDuplicate, SIGNAL(triggered()), this, SLOT(duplicateSelected()));

	//Setup context menu
	contextMenu.addAction("Add Command", this, SLOT(addNew()));
	contextMenu.addAction("Delete",  this, SLOT(deleteSelected()));
	ui->commandList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->commandList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    connect(ui->commandList, SIGNAL(itemSelectionChanged()), this, SLOT(handleSelectionChanged()));

	unsavedChanges = false;
}

ProgWindow::~ProgWindow()
{
    delete ui;
}

void ProgWindow::newOpenProgram()
{
    startup *n = new startup();
    if(this->close())
        n->show();
}

void ProgWindow::showOptions()
{
	OptionsDialog *o = new OptionsDialog();
	o->exec();
}

void ProgWindow::exitProgram()
{
	this->close();
}

//\///////////////////////////
//(menu) item actions
//\//

void ProgWindow::showContextMenu(const QPoint &pos)
{
	QPoint globalPos = ui->commandList->mapToGlobal(pos);
	contextMenu.exec(globalPos);
}

void ProgWindow::addNew()
{
	NewCommandDialog *n = new NewCommandDialog();
	if(!n->exec())
		return;

	CommandWidget *itemWidget;
	int c = -1;
	if(!n->getResult(c))
		return;

	switch(c){
		case 0:
			itemWidget = new ClickCommandWidget();
			break;
	}

	QListWidgetItem *item = new QListWidgetItem();
	addItem(item, itemWidget, ui->commandList->count());

	ui->commandList->setCurrentRow(ui->commandList->count() - 1);
	unselectAll();
	item->setSelected(true);
}

void ProgWindow::deleteSelected()
{
	while(ui->commandList->selectedItems().length() > 0)
	{
		QListWidgetItem *item = ui->commandList->selectedItems().at(0);
		delete item;
	}
}

void ProgWindow::duplicateSelected()
{
	if(ui->commandList->selectedItems().size() == 0)
		return;

	QList<QListWidgetItem *> newItems;

	QList<QListWidgetItem *> selectedItems = ui->commandList->selectedItems();

	for (int i = 0; i < selectedItems.size(); ++i)
	{
		QListWidgetItem *item = selectedItems.at(i);
		QListWidgetItem *newItem = item->clone();
        //addItem(newItem, , ui->commandList->row(item)+1);
		newItems.append(newItem);
	}

	ui->commandList->setCurrentRow(ui->commandList->row(newItems.at(0)));
	unselectAll();

	QListWidgetItem *i;
	foreach(i, newItems){
		i->setSelected(true);
	}
}

void ProgWindow::addItem(QListWidgetItem *item, CommandWidget *itemWidget, int row)
{
	ui->commandList->insertItem(row, item);
	ui->commandList->setItemWidget(item, itemWidget);
	item->setSizeHint(QSize(0, itemWidget->height()));
	commandList.insert(row, QString::number(itemWidget->index));
}

void ProgWindow::unselectAll()
{
	QList<QListWidgetItem *> selectedItems = ui->commandList->selectedItems();
	for (int i = 0; i < selectedItems.size(); ++i)
	{
		QListWidgetItem *item = selectedItems.at(i);
		item->setSelected(false);
	}
}

void ProgWindow::handleSelectionChanged()
{
	bool itemSelected = ui->commandList->selectedItems().count() > 0;

	ui->actionDuplicate->setEnabled(itemSelected);
	ui->actionDelete->setEnabled(itemSelected);
}
