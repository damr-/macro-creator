#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <QScrollBar>
#include <QSignalMapper>
#include <QStringList>
#include <QTimer>

#include "commands.h"
#include "clickcommandwidget.h"
#include "setcursorposcommandwidget.h"
#include "waitcommandwidget.h"

#pragma comment(lib, "user32.lib")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    //CheckKey Timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkKey()));
    timer->start(50);

    //Menu->File Actions
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newProgram()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProgram()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openProgram()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveProgramAs()));

    //Menu->Edit Actions
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteSelected()));
    connect(ui->actionDuplicate, SIGNAL(triggered()), this, SLOT(duplicateSelected()));

    //"Add Command" Buttons
//    connect(ui->move, SIGNAL(clicked()), this, SLOT(addMoveCursorCommand()));
//    connect(ui->click, SIGNAL(clicked()), this, SLOT(addClickCommand()));
//    connect(ui->drag, SIGNAL(clicked()), this, SLOT(addDragMouseCommand()));
//    connect(ui->scroll, SIGNAL(clicked()), this, SLOT(addScrollCommand()));
//    connect(ui->addWriteTextCommandButton, SIGNAL(clicked()), this, SLOT(addWriteTextCommand()));
//    connect(ui->shortcut, SIGNAL(clicked()), this, SLOT(addShortcutCommand()));
//    connect(ui->addWaitCommand, SIGNAL(clicked()), this, SLOT(addWaitCommand()));
//    connect(ui->addOpenExeCommandButton, SIGNAL(clicked()), this, SLOT(addOpenExeCommand()));
//    connect(ui->chooseExeButton, SIGNAL(clicked()), this, SLOT(chooseExe()));
//    connect(ui->killProc, SIGNAL(clicked()), this, SLOT(addKillProcessCommand()));
//    connect(ui->delCmd, SIGNAL(clicked()), this, SLOT(deleteCommand()));
//    connect(ui->delCmdUndo, SIGNAL(clicked()), this, SLOT(deleteUndo()));
//    connect(ui->commandList, SIGNAL(currentRowChanged(int)), this, SLOT(refreshCommandListControls()));

    //Options
    QSignalMapper *optionsMapper = new QSignalMapper(this);
    connect(ui->defaultDelayCheckBox, SIGNAL(toggled(bool)), optionsMapper, SLOT(map()));
    connect(ui->defaultDelaySpinBox, SIGNAL(valueChanged(int)), optionsMapper, SLOT(map()));
    connect(ui->loopCheckBox, SIGNAL(toggled(bool)), optionsMapper, SLOT(map()));
    connect(ui->loopType, SIGNAL(currentIndexChanged(int)), optionsMapper, SLOT(map()));
    connect(ui->loopFrom, SIGNAL(valueChanged(int)), optionsMapper, SLOT(map()));
    connect(ui->loopTo, SIGNAL(valueChanged(int)), optionsMapper, SLOT(map()));
    connect(ui->loopAmount, SIGNAL(valueChanged(int)), optionsMapper, SLOT(map()));
    optionsMapper->setMapping(ui->defaultDelayCheckBox, 0);
    optionsMapper->setMapping(ui->defaultDelaySpinBox, 0);
    optionsMapper->setMapping(ui->loopCheckBox, 0);
    optionsMapper->setMapping(ui->loopType, 0);
    optionsMapper->setMapping(ui->loopFrom, 0);
    optionsMapper->setMapping(ui->loopTo, 0);
    optionsMapper->setMapping(ui->loopAmount, 0);
    connect(optionsMapper, SIGNAL(mapped(int)), this, SLOT(optionsChanged(int)));

    //Shortcut idiotproofing
    //connect(ui->letterBox, SIGNAL(textChanged(QString)), this, SLOT(letterBoxEdited()));
    //connect(ui->keyBox, SIGNAL(currentIndexChanged(int)), this, SLOT(keyBoxEdited()));

    //read key button
    connect(ui->readKeyButton, SIGNAL(clicked(bool)), this, SLOT(readKeyButtonPressed()));

    //start program button
    //connect(ui->startProgramButton, SIGNAL(clicked()), this, SLOT(startProgram()));

    //Loop signals
    connect(ui->loopType, SIGNAL(currentIndexChanged(int)), this, SLOT(loopTypeChanged()));
    connect(ui->loopFrom, SIGNAL(editingFinished()), this, SLOT(loopFromChanged()));
    connect(ui->loopTo, SIGNAL(editingFinished()), this, SLOT(loopToChanged()));

    //Default delay
    connect(ui->defaultDelayCheckBox, SIGNAL(toggled(bool)), this, SLOT(defaultDelayToggled()));

    //Arrange write text edit
    connect(ui->writeTextEditField, SIGNAL(textChanged(QString)), this, SLOT(updateWriteTextCount()));

    //setup context menu
    ui->commandList->setContextMenuPolicy(Qt::CustomContextMenu);

    QList<QAction *> editActions;
    QList<QMenu*> menuList = ui->menuBar->findChildren<QMenu*>();
    foreach(QMenu* menu, menuList)
    {
        if(menu->title() == "Edit")
        {
            foreach (QAction* a, menu->actions())
            {
                editActions.append(a);
            }
            break;
        }
    }
    contextMenu.addActions(editActions);

    connect(ui->commandList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->commandList, SIGNAL(itemSelectionChanged()), this, SLOT(handleSelectionChanged()));

    //react to drag&drop events
    connect(ui->commandList->model(),
            SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this,
            SLOT(handleItemChanged(QModelIndex,int,int,QModelIndex,int)));

    //disable actions by default
    handleSelectionChanged();

    //start with unnamed project
    programName = "unnamed";
    programPath = QDir::currentPath();
    ui->tabWidget->setCurrentIndex(0);
    setUnsavedChanges(false);
    unsavedProgram = true;
    isListeningForKeyInput = false;

    ui->actionSave_As->setEnabled(false);

    connect(ui->addCommandButton, SIGNAL(clicked()), this, SLOT(addNewCommand()));
    connect(ui->commandSelectBox, SIGNAL(currentIndexChanged(int)), this, SLOT(commandSelectionChanged()));

    //fill command select box
    foreach(QString name, Commands::commandNames())
    {
        ui->commandSelectBox->addItem(name);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(!isListeningForKeyInput)
        return;

    if(e->key() == Qt::Key_Escape){
        ui->readKeyButton->setText("click me!");
        isListeningForKeyInput = false;
    }
    else{
        qDebug() << e->text();
        ui->readKeyButton->setText(e->text());
    }
}

//-------------------------------------------------------
//--------------------CloseEvent-------------------------
//-------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!unsavedChanges) {
        event->accept();
        return;
    }

    UnsavedChangesMessageResult result = UnsavedChangesMessageResult::Cancel;
    QMessageBox* msgbox = showUnsavedChangesWarning(result);

    if(result == UnsavedChangesMessageResult::Cancel)
        event->ignore();
    else
    {
        if(result == UnsavedChangesMessageResult::Save)
            saveProgram();
        event->accept();
    }
    msgbox->close();
}

QMessageBox* MainWindow::showUnsavedChangesWarning(UnsavedChangesMessageResult &result)
{
    QMessageBox *msgbox = new QMessageBox(this);
    QPushButton *btn1 = new QPushButton("Save");
    QPushButton *btn2 = new QPushButton("Don't save");
    QPushButton *btn3 = new QPushButton("Cancel");

    msgbox->setText(tr("There are unsaved changes which will get lost."));
    msgbox->setWindowTitle("Warning!");

    QPixmap icon(":/images/warning.png");
    msgbox->setIconPixmap(icon);

    msgbox->addButton(btn1, QMessageBox::ActionRole);
    msgbox->addButton(btn2, QMessageBox::ActionRole);
    msgbox->addButton(btn3, QMessageBox::ActionRole);

    msgbox->exec();

    if(msgbox->clickedButton() == btn1)
        result = UnsavedChangesMessageResult::Save;
    if(msgbox->clickedButton() == btn2)
        result = UnsavedChangesMessageResult::DontSave;
    if(msgbox->clickedButton() == btn3)
        result = UnsavedChangesMessageResult::Cancel;

    return msgbox;
}

//-------------------------------------------------------
//--------------------CheckKey---------------------------
//-------------------------------------------------------
void MainWindow::checkKey()
{
    if(GetAsyncKeyState(VK_INSERT))
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);

        ui->xCoord->setValue(cursorPos.x);
        ui->yCoord->setValue(cursorPos.y);
        ui->xCoordDrag->setValue(cursorPos.x);
        ui->yCoordDrag->setValue(cursorPos.y);
        ui->statusBar->showMessage("Cursor position (" + QString::number(cursorPos.x) + "," + QString::number(cursorPos.y) + ") has been saved", 10000);
    }
    if(GetAsyncKeyState(VK_F7))
    {
        //if(ui->startProgramButton->isEnabled())
        startProgram();
    }
}

//-------------------------------------------------------
//--------------------Add Command------------------------
//-------------------------------------------------------
//void MainWindow::addCommand(QString commandtype, QStringList arguments)
//{
//    QString line;
//    line.append(commandtype);
//    foreach (const QString &element, arguments)
//        line.append("|" + element);

//    int aboveRow = ui->commandList->currentRow();

//    if(ui->addAbove->isChecked())
//        commandListStrings.insert(aboveRow, line);
//    else commandListStrings.append(line);

//    fillCommandListWidget();

//    if(ui->addAbove->isChecked()){
//        ui->commandList->setCurrentRow(aboveRow);
//        ui->addAbove->setChecked(false);
//    }
//    else ui->commandList->setCurrentRow(ui->commandList->count() - 1);

//    setUnsavedChanges(true);
//}

//-------------------------------------------------------
//--------------------New Program------------------------
//-------------------------------------------------------
void MainWindow::newProgram()
{
    MainWindow *m = new MainWindow();
    if(close())
        m->show();
}

//------------------------------------------------------
//--------------------Open Program----------------------
//------------------------------------------------------
void MainWindow::openProgram()
{
    QString fullFilePath = QFileDialog::getOpenFileName(this, tr("Open program"), programPath, tr("Program Files (*.myprog)"));

    QString fileName = QFileInfo(fullFilePath).baseName();
    if(fileName.length() == 0)
    {
        ui->statusBar->showMessage("Opening aborted", 3000);
        return;
    }

    programName = fileName;

    if(unsavedChanges){
        UnsavedChangesMessageResult result = UnsavedChangesMessageResult::Cancel;
        QMessageBox* msgbox = showUnsavedChangesWarning(result);

        if(result == UnsavedChangesMessageResult::Cancel){
            ui->statusBar->showMessage("Opening aborted", 3000);
            return;
        }
        if(result == UnsavedChangesMessageResult::Save)
            saveProgram();
        msgbox->close();
    }

    loadCommandListFromFile(fullFilePath);
    ui->statusBar->showMessage("Opened " + fullFilePath, 3000);
    unsavedProgram = false;
    setUnsavedChanges(false);
    refreshWindowTitle();
    ui->actionSave_As->setEnabled(true);
}

//------------------------------------------------------
//--------------------Save Program----------------------
//------------------------------------------------------
void MainWindow::saveProgram()
{
    if(!unsavedProgram && !unsavedChanges)
    {
        ui->statusBar->showMessage("No changes to save", 3000);
        return;
    }

    QString pathPlusFileName = getFullFilePath(programPath, programName);

    if(unsavedProgram){
        pathPlusFileName = QFileDialog::getSaveFileName(this, tr("Save the program as..."), programPath, tr("Program Files (*.myprog)"));

        QString fileName = QFileInfo(pathPlusFileName).baseName();

        if(fileName.length() > 0)
            programName = fileName;
        else
        {
            ui->statusBar->showMessage("Saving aborted", 3000);
            return;
        }
        unsavedProgram = false;
    }

    QFile file(pathPlusFileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << file.errorString();
        ui->statusBar->showMessage("Unable to open the file!", 3000);
        return;
    }

    QTextStream output(&file);
    QString out;

    out.append(QString::number(ui->defaultDelayCheckBox->isChecked()) + "|"+
               QString::number(ui->defaultDelaySpinBox->value()) + "|" +
               QString::number(ui->loopCheckBox->isChecked()) + "|" +
               QString::number(ui->loopType->currentIndex()) + "|" +
               QString::number(ui->loopFrom->value()) + "|" +
               QString::number(ui->loopTo->value()) + "|" +
               QString::number(ui->loopAmount->value()) + "\n");

    for(int i = 0; i < ui->commandList->count(); ++i)
    {
        out.append(getCommandString(i));
        out.append("\n");
    }

    output << out;
    file.close();

    setUnsavedChanges(false);
    ui->statusBar->showMessage("Saved program to " + pathPlusFileName, 3000);
}

//-------------------------------------------------------
//--------------------Save program as--------------------
//-------------------------------------------------------
void MainWindow::saveProgramAs()
{
    unsavedProgram = true;
    saveProgram();
}

//-------------------------------------------------------
//-------------------Load from File to list--------------
//-------------------------------------------------------
void MainWindow::loadCommandListFromFile(QString filename)
{
    ui->commandList->clear();

    QFile file(filename);

    if(!file.exists())
        return;

    if(!file.open(QFile::ReadOnly))
        return;

    QTextStream input(&file);
    QStringList options = input.readLine().split("|");

    if(options.size() != 7){
        QString msg = filename + " is corrupted. Aborted opening process.";
        ui->statusBar->showMessage(msg);
        qDebug() << msg;
        file.close();
        return;
    }

    ui->defaultDelayCheckBox->setChecked(options[0] == "1");
    ui->defaultDelaySpinBox->setValue(options[1].toInt());
    ui->loopCheckBox->setChecked(options[2] == "1");
    ui->loopType->setCurrentIndex(options[3].toInt());
    ui->loopFrom->setValue(options[4].toInt());
    ui->loopTo->setValue(options[5].toInt());
    ui->loopAmount->setValue(options[6].toInt());

    QStringList commandListStrings;

    QString line;
    do
    {
        line = input.readLine().trimmed();
        if(line.length() > 0)
            commandListStrings.append(line);
    }while(!line.isNull());

    fillCommandListWidget(commandListStrings);
    file.close();
}

//-------------------------------------------------------
//--------------Load from list to widget-----------------
//-------------------------------------------------------
void MainWindow::fillCommandListWidget(QStringList commandListStrings)
{
    ui->commandList->clear();

    for(int i = 0; i < commandListStrings.size(); ++i)
    {
        QStringList list = commandListStrings.at(i).split("|");

        int commandTypeIndex = list[0].toInt();

        //create list item
        QListWidgetItem *newItem = new QListWidgetItem();

        //create new widget
        CommandWidget *newWidget = CommandWidget::GetNewCommandWidget(commandTypeIndex);

        newWidget->commandType = (CommandType)commandTypeIndex;

        switch(commandTypeIndex){
        case 0:
//            QString item;
//            item.append("Make a ");
//            if( list[1] == "0" ) item.append("Left Mouse Button");
//            else if( list[1] == "1" ) item.append("Double Left Mouse Button");
//            else if( list[1] == "2" ) item.append("Right Mouse Button");
//            else if( list[1] == "3" ) item.append("Middle Mouse Button");
//            item.append(" click");
//            ui->commandList->addItem(item);

            ((ClickCommandWidget*)newWidget)->SetClickAmount(list[1].toInt());
            ((ClickCommandWidget*)newWidget)->SetClickType((ClickType)list[2].toInt());
            addItem(newItem, newWidget, i);
            break;

        case 1:
            ((SetCursorPosCommandWidget*)newWidget)->SetCoordinates(list[1].toInt(), list[2].toInt());
            addItem(newItem, newWidget, i);
            break;
        }

//        else if( list[0] == "drg" ) {
//            ui->commandList->addItem("Drag to (" + list[1] + "," + list[2] + ")");
//        }

//        else if( list[0] == "scr" ) {
//            QString item;
//            item.append("Scroll ");
//            if( list[2] == "0") item.append("up ");
//            else if ( list[2] == "1" ) item.append("down ");
//            item.append(list[1] + " times");
//            ui->commandList->addItem(item);
//        }

//        else if( list[0] == "txt" ) {
//            QString item = "Write '" + list[2] + "' " + (list[1] == "0" ? "upper-case" : "lower-case");
//            ui->commandList->addItem(item);
//        }

//        else if( list[0] == "slp" ) {
//            QString item = "Sleep for " + list[1] + (list[2] == "0" ? " Seconds" : " Milliseconds");
//            ui->commandList->addItem(item);
//        }

//        else if( list[0] == "end" ) {
//            ui->commandList->addItem("End process '" + list[1] + "'");
//        }

//        else if( list[0] == "exe" ) {
//            QString item;
//            item.append("Open '" + list[1] + "'");
//            ui->commandList->addItem(item);
//        }

//        else if( list[0] == "sct" ) {
//            QString item;
//            item.append("Press '");
//            if(list[1] == "1") item.append("CTRL + ");
//            if(list[2] == "1") item.append("ALT + ");
//            if(list[3] == "1") item.append("SHIFT + ");
//            item.append(list[5]);
//            item.append("' " + list[6] + " times");

//            ui->commandList->addItem(item);
//        }
    }

    ui->commandList->setCurrentRow(ui->commandList->count() - 1);

    if(ui->commandList->count() > 0) {
        ui->commandListTitle->setText("Command List [" + QString::number(ui->commandList->currentRow()) + "/"+ QString::number(commandListStrings.size()) + "]");
    }
    else {
        ui->commandListTitle->setText("Command List [0/"+ QString::number(commandListStrings.size()) + "]");
    }

    //refreshCommandListControls();
}


void MainWindow::optionsChanged(int dummy)
{
    dummy = 1;
    setUnsavedChanges(true);
}

void MainWindow::readKeyButtonPressed()
{
    if(isListeningForKeyInput)
        return;

    isListeningForKeyInput = true;
    ui->readKeyButton->setText("make the sc!");
}

void MainWindow::updateWriteTextCount()
{
    ui->count->setText(QString::number(ui->writeTextEditField->text().length()) + " / " + QString::number(ui->writeTextEditField->maxLength()));
}

void MainWindow::refreshWindowTitle()
{
    setWindowTitle(programName + (unsavedChanges ? "*" : "") + " - Personal Macro");
}

void MainWindow::setUnsavedChanges(bool newUnsavedChanges)
{
    unsavedChanges = newUnsavedChanges;
    refreshWindowTitle();
}

QString MainWindow::getCommandString(int commandIndex)
{
    QListWidgetItem* item = ui->commandList->item(commandIndex);
    QWidget* itemWidget = ui->commandList->itemWidget(item);
    return ((CommandWidget*)itemWidget)->GetCommandString();
}

//-------------------------------------------------------
//-----------------Choose a .exe file--------------------
//-------------------------------------------------------
//void MainWindow::chooseExe()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose .exe"), QDir::currentPath(), tr("Executable Files (*.exe)"));
//    if(fileName.length() > 1 && fileName.contains(".exe"))
//        ui->exeName->setText(fileName);
//    else
//        ui->statusBar->showMessage("invalid or no file", 3000);
//}

//-------------------------------------------------------
//------------Delete the selected command----------------
//-------------------------------------------------------
//void MainWindow::deleteCommand()
//{
//    if(ui->commandList->count() == 0)
//        return;

//    delBackupPos = ui->commandList->currentRow();
//    delBackupText = commandListStrings.at(delBackupPos);

//    commandListStrings.removeAt(delBackupPos);
//    fillCommandListWidget();

//    setUnsavedChanges(true);
//    ui->delCmdUndo->setEnabled(true);

//    if(delBackupPos == ui->commandList->count())
//        ui->commandList->setCurrentRow(ui->commandList->count() - 1);
//    else
//        ui->commandList->setCurrentRow(delBackupPos);
//}

//-------------------------------------------------------
//---------------Undo the last deletion------------------
//-------------------------------------------------------
//void MainWindow::deleteUndo()
//{
//    commandListStrings.insert(delBackupPos, delBackupText);
//    fillCommandListWidget();
//    setUnsavedChanges(true);
//    ui->delCmdUndo->setEnabled(false);
//    ui->commandList->setCurrentRow(delBackupPos);
//}

//-------------------------------------------------------
//----------Refresh right hand side controls-------------
//-------------------------------------------------------
//void MainWindow::refreshCommandListControls()
//{
//    if(ui->commandList->count() > 0)
//    {
//        ui->delCmd->setEnabled(true);
//        ui->startProgramButton->setEnabled(true);
//        ui->addAbove->setEnabled(true);
//        ui->addAbove->setCheckable(true);

//        ui->commandListTitle->setText("Command List [" + QString::number(ui->commandList->currentRow()+1) + "/"+ QString::number(commandListStrings.size()) + "]");
//    }
//    else
//    {
//        ui->delCmd->setEnabled(false);
//        ui->startProgramButton->setEnabled(false);
//        ui->addAbove->setEnabled(false);
//        ui->addAbove->setCheckable(false);
//        ui->commandListTitle->setText("Command List [0/"+ QString::number(commandListStrings.size()) + "]");
//    }
//}

//-------------------------------------------------------
//-------------Letterbox editing finished----------------
//-------------------------------------------------------
//void MainWindow::letterBoxEdited()
//{
//    ui->keyBox->setCurrentIndex(0);
//    QString text = ui->letterBox->text();
//    ui->letterBox->setText(text);
//}

//-------------------------------------------------------
//---------------Keybox editing finished-----------------
//-------------------------------------------------------
//void MainWindow::keyBoxEdited()
//{
//    int currentPos = ui->keyBox->currentIndex();
//    ui->letterBox->setText("");
//    ui->keyBox->setCurrentIndex(currentPos);
//}

//-------------------------------------------------------
//----------------Start the program----------------------
//-------------------------------------------------------
void MainWindow::startProgram()
{
    if(isProgramRunning)
        return;

    isProgramRunning = true;
    //ui->startProgramButton->setEnabled(false);
    showMinimized();

    bool loop = false, partLoop = false, paused = false;
    int start, stop, times;

    if(ui->loopCheckBox->isChecked()) {
        if(ui->loopType->currentIndex() == 1) {
            loop = false;
            partLoop = true;
            start = ui->loopFrom->value();
            stop = ui->loopTo->value();
            times = ui->loopAmount->value();
            if(start > ui->commandList->count() || stop > ui->commandList->count())
            {
                //ui->startProgramButton->setEnabled(true);
                refreshWindowTitle();
                showNormal();
                return;
            }
        }
        else
            loop = true;
    }

    do
    {
        for(int i = 0; i < ui->commandList->count(); ++i)
        {
            if(GetAsyncKeyState(VK_F7))
                paused = false;
            if(GetAsyncKeyState(VK_F8))
                paused = true;
            if(GetAsyncKeyState(VK_F9)) {
                loop = false;
                partLoop = false;
                break;
            }

            if(!paused)
            {
                if(ui->loopCheckBox->isChecked())
                {
                    if(ui->loopCheckBox->isChecked())
                        setWindowTitle("Program looping w/ delay");
                    else setWindowTitle("Program running w/ delay");
                }
                else
                {
                    if(ui->loopCheckBox->isChecked())
                        setWindowTitle("Program looping w/o delay");
                    else setWindowTitle("Program running w/o delay");
                }

                if(i == stop && partLoop == true && times > 0)
                {
                    i = start - 1;
                    --times;
                }

                Commands::ExecuteCommand(getCommandString(i));
                if(ui->defaultDelayCheckBox->isChecked())
                    Sleep(ui->defaultDelaySpinBox->value());
                qApp->processEvents();
            }
            else
            {
                --i;
                setWindowTitle("Program paused");
                qApp->processEvents();
                Sleep(50);
            }
        }
    }while(loop);

    refreshWindowTitle();
    showNormal();
    //ui->startProgramButton->setEnabled(true);
    isProgramRunning = false;
}

//-------------------------------------------------------
//---------Default delay checkbox toggled----------------
//-------------------------------------------------------
void MainWindow::defaultDelayToggled()
{
    if(ui->defaultDelayCheckBox->isChecked())
        ui->defaultDelaySpinBox->setEnabled(true);
    else ui->defaultDelaySpinBox->setEnabled(false);
}

//-------------------------------------------------------
//----------------Loop type changed----------------------
//-------------------------------------------------------
void MainWindow::loopTypeChanged()
{
    bool loopAll = ui->loopType->currentIndex() == 0;
    ui->loopFrom->setEnabled(!loopAll);
    ui->loopTo->setEnabled(!loopAll);
    ui->loopAmount->setEnabled(!loopAll);
}

//-------------------------------------------------------
//------------Loop from value changed--------------------
//-------------------------------------------------------
void MainWindow::loopFromChanged()
{
    if(ui->loopFrom->value() > ui->loopTo->value())
        ui->loopTo->setValue(ui->loopFrom->value());
}

//-------------------------------------------------------
//--------------Loop to value changed--------------------
//-------------------------------------------------------
void MainWindow::loopToChanged()
{
    if(ui->loopTo->value() < ui->loopFrom->value())
        ui->loopFrom->setValue(ui->loopTo->value());
}

//-------------------------------------------------------
//-------------------Context menu------------------------
//-------------------------------------------------------
void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->commandList->mapToGlobal(pos);
    contextMenu.exec(globalPos);
}

//-------------------------------------------------------
//-----------------Add new command-----------------------
//-------------------------------------------------------
void MainWindow::addNewCommand()
{
    int currentCommandIndex = ui->commandSelectBox->currentIndex();

    CommandWidget *itemWidget = CommandWidget::GetNewCommandWidget(currentCommandIndex);
    QListWidgetItem *item = new QListWidgetItem();
    //item->commandIndex = commandIndex;
    itemWidget->commandType = (CommandType)currentCommandIndex;
    addItem(item, itemWidget, ui->commandList->count());

    ui->commandList->setCurrentRow(ui->commandList->count() - 1);
    unselectAll();
    item->setSelected(true);    
    setUnsavedChanges(true);
}

void MainWindow::deleteSelected()
{
    while(ui->commandList->selectedItems().length() > 0)
    {
        QListWidgetItem *item = ui->commandList->selectedItems().at(0);
        delete item;
    }
    setUnsavedChanges(true);
}

void MainWindow::duplicateSelected()
{
    if(ui->commandList->selectedItems().size() == 0)
        return;

    QList<QListWidgetItem *> selectedItems = ui->commandList->selectedItems();

    QList<QListWidgetItem *> newItems;
    for (int i = 0; i < selectedItems.size(); ++i)
    {
        QListWidgetItem *item = (QListWidgetItem *)selectedItems.at(i);
        QListWidgetItem *newItem = new QListWidgetItem();

        CommandWidget *selectedItemWidget = (CommandWidget*)ui->commandList->itemWidget(item);
        CommandType selectedItemCommandType = selectedItemWidget->commandType;

        //create new widget
        CommandWidget *newWidget = CommandWidget::GetNewCommandWidget((int)selectedItemCommandType);

        //copy widget values to new one
        selectedItemWidget->CopyTo(newWidget);

        //copy list item command index to new item
        newWidget->commandType = selectedItemCommandType;

        addItem(newItem, newWidget, ui->commandList->row(item)+1);
        newItems.append(newItem);
    }

    ui->commandList->setCurrentRow(ui->commandList->row(newItems.at(0)));
    unselectAll();

    QListWidgetItem *i;
    foreach(i, newItems)
    {
        i->setSelected(true);
    }
    setUnsavedChanges(true);
}

//-------------------------------------------------------
//-------------Add item with widget at row---------------
//-------------------------------------------------------
void MainWindow::addItem(QListWidgetItem *item, CommandWidget *itemWidget, int row)
{
    ui->commandList->insertItem(row, item);
    ui->commandList->setItemWidget(item, itemWidget);
    item->setSizeHint(QSize(0, itemWidget->height()));
    connect(itemWidget, SIGNAL(commandChanged()), this, SLOT(handleCommandChanged()));
}

//-------------------------------------------------------
//--------------------Unselect all-----------------------
//-------------------------------------------------------
void MainWindow::unselectAll()
{
    QList<QListWidgetItem *> selectedItems = ui->commandList->selectedItems();
    for (int i = 0; i < selectedItems.size(); ++i)
    {
        QListWidgetItem *item = selectedItems.at(i);
        item->setSelected(false);
    }
}

void MainWindow::commandSelectionChanged()
{

}

void MainWindow::handleCommandChanged()
{
    setUnsavedChanges(true);
}

void MainWindow::handleSelectionChanged()
{
    bool itemSelected = ui->commandList->selectedItems().count() > 0;

    ui->actionDuplicate->setEnabled(itemSelected);
    ui->actionDelete->setEnabled(itemSelected);
    ui->actionCopy->setEnabled(itemSelected);
    ui->actionCut->setEnabled(itemSelected);
}

void MainWindow::handleItemChanged(QModelIndex, int, int, QModelIndex, int)
{
    setUnsavedChanges(true);
    //refreshCommandListControls();
}
