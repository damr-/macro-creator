#include <Windows.h>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QList>
#include <QSignalMapper>
#include <QCloseEvent>
#include <tchar.h>
#include <string>
#include <QProcess>
#include <QApplication>
#include <QScrollBar>

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "startup.h"
#include "optionsdialog.h"
#include "keyboardutilities.h"

#pragma comment(lib, "user32.lib")

#define SCROLLUP 120
#define SCROLLDOWN -120

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

    //Menu Actions
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newProgram()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProgram()));

    //"Add Command" Buttons
    connect(ui->move, SIGNAL(clicked()), this, SLOT(addMoveCursorCommand()));
    connect(ui->click, SIGNAL(clicked()), this, SLOT(addClickCommand()));
    connect(ui->drag, SIGNAL(clicked()), this, SLOT(addDragMouseCommand()));
    connect(ui->scroll, SIGNAL(clicked()), this, SLOT(addScrollCommand()));
    connect(ui->text, SIGNAL(clicked()), this, SLOT(addWriteTextCommand()));
    connect(ui->shortcut, SIGNAL(clicked()), this, SLOT(addShortcutCommand()));
    connect(ui->sleep, SIGNAL(clicked()), this, SLOT(addSleepCommand()));
    connect(ui->addOpenExeCommandButton, SIGNAL(clicked()), this, SLOT(addOpenExeCommand()));
    connect(ui->chooseExeButton, SIGNAL(clicked()), this, SLOT(chooseExe()));
    connect(ui->killProc, SIGNAL(clicked()), this, SLOT(addKillProcessCommand()));
    connect(ui->delCmd, SIGNAL(clicked()), this, SLOT(deleteCommand()));
    connect(ui->delCmdUndo, SIGNAL(clicked()), this, SLOT(deleteUndo()));
    connect(ui->commandList, SIGNAL(currentRowChanged(int)), this, SLOT(moveItem()));

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

    //List modifications
    QSignalMapper *moveItemMapper = new QSignalMapper(this);
    connect(ui->moveup, SIGNAL(clicked()), moveItemMapper, SLOT(map()));
    connect(ui->movedown, SIGNAL(clicked()), moveItemMapper, SLOT(map()));
    moveItemMapper -> setMapping (ui->moveup, -1) ;
    moveItemMapper -> setMapping (ui->movedown, 1) ;
    connect(moveItemMapper, SIGNAL(mapped(int)), this, SLOT(moveIt(int)));

    //Shortcut idiotproofing
    connect(ui->letterBox, SIGNAL(textChanged(QString)), this, SLOT(letterBoxEdited()));    
    connect(ui->keyBox, SIGNAL(currentIndexChanged(int)), this, SLOT(keyBoxEdited()));

    //Bot start
    connect(ui->botstart, SIGNAL(clicked()), this, SLOT(botStart()));

    //Loop signals
    connect(ui->loopType, SIGNAL(currentIndexChanged(int)), this, SLOT(loopTypeChanged()));
    connect(ui->loopFrom, SIGNAL(editingFinished()), this, SLOT(loopFromChanged()));
    connect(ui->loopTo, SIGNAL(editingFinished()), this, SLOT(loopToChanged()));

    //Default delay
    connect(ui->defaultDelayCheckBox, SIGNAL(toggled(bool)), this, SLOT(defaultDelayToggled()));

    //Arrange write text edit
    connect(ui->writeTextEditField, SIGNAL(textChanged(QString)), this, SLOT(updateWriteTextCount()));

    //get and load prog
    //progName = startup::getCurrentProgName();
    //loadCommandListFromFile();
    //ui->tabWidget->setCurrentIndex(0);
    //setUnsavedChanges(false);

    //start with unnamed project
    progName = "unnamed";
    ui->tabWidget->setCurrentIndex(0);
    setUnsavedChanges(false);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    QMessageBox msgbox(this);
    QPushButton *btn1 = new QPushButton("Save and quit");
    QPushButton *btn2 = new QPushButton("Quit without saving");
    QPushButton *btn3 = new QPushButton("Cancel");

    msgbox.setText(tr("There are unsaved changes which will get lost."));
    msgbox.setWindowTitle("Warning!");

    QPixmap icon(":/images/warning.png");
    msgbox.setIconPixmap(icon);

    msgbox.addButton(btn1, QMessageBox::ActionRole);
    msgbox.addButton(btn2, QMessageBox::ActionRole);
    msgbox.addButton(btn3, QMessageBox::ActionRole);

    msgbox.exec();

    if(msgbox.clickedButton() == btn3) event->ignore();
    else
    {
        if(msgbox.clickedButton() == btn1)
            saveProgram();
        event->accept();
    }
    msgbox.close();
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
    }
    if(GetAsyncKeyState(VK_F7))
    {
        if(ui->botstart->isEnabled())
            botStart();
    }
}

//-------------------------------------------------------
//--------------------New Program------------------------
//-------------------------------------------------------
void MainWindow::newProgram()
{
    MainWindow *m = new MainWindow();
    if(close())
        m->show();
}

//-------------------------------------------------------
//--------------------Add Command------------------------
//-------------------------------------------------------
void MainWindow::addCommand(QString commandtype, QStringList arguments)
{
    QString line;
    line.append(commandtype);
    foreach (const QString &element, arguments)
        line.append("|" + element);

    int aboveRow = ui->commandList->currentRow();

    if(ui->addAbove->isChecked())
        commandList.insert(aboveRow, line);
    else commandList.append(line);

    fillCommandListWidget();

    if(ui->addAbove->isChecked()){
        ui->commandList->setCurrentRow(aboveRow);
        ui->addAbove->setChecked(false);
    }
    else ui->commandList->setCurrentRow(ui->commandList->count() - 1);

    setUnsavedChanges(true);
}

//-------------------------------------------------------
//--------------------Save Program----------------------
//---
void MainWindow::saveProgram()
{
    if(progName == "unnamed"){

    }
    else {
        QString filename = (QDir::currentPath() + "/data/" + progName);
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            ui->statusBar->setStatusTip("Saving program failed!");
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

        for(int i = 0; i < commandList.size(); ++i)
        {
            out.append(commandList.at(i));
            out.append("\n");
        }

        output << out;
        file.close();
    }

    setUnsavedChanges(false);
    ui->statusBar->setStatusTip("Saved program successfully!");
}

//-------------------------------------------------------
//-------------------Load from File to list--------------
//---
void MainWindow::loadCommandListFromFile()
{
    commandList.clear();

    QString filename = (QDir::currentPath() + "/data/" + progName);
    QFile file(filename);

    if(!file.exists())
        return;

    file.open(QFile::ReadOnly);

    QTextStream input(&file);
    QStringList options = input.readLine().split("|");

    if(options.size() != 7){
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

    QString line;
    do
    {
        line = input.readLine().trimmed();
        if(line.length() > 0)
            commandList.append(line);
    }while(!line.isNull());

    fillCommandListWidget();
    file.close();
}

//-------------------------------------------------------
//--------------Load from list to widget-----------------
//---
void MainWindow::fillCommandListWidget()
{
    ui->commandList->clear();

    for(int i = 0; i < commandList.size(); ++i)
    {
        QStringList list = commandList.at(i).split("|");

        if( list[0] == "mov" ) {
            ui->commandList->addItem("Move cursor to (" + list[1] + "," + list[2] + ")");
        }

        else if( list[0] == "clc" ) {
            QString item;
            item.append("Make a ");
            if( list[1] == "0" ) item.append("Left Mouse Button");
            else if( list[1] == "1" ) item.append("Double Left Mouse Button");
            else if( list[1] == "2" ) item.append("Right Mouse Button");
            else if( list[1] == "3" ) item.append("Middle Mouse Button");
            item.append(" click");
            ui->commandList->addItem(item);
        }

        else if( list[0] == "drg" ) {
            ui->commandList->addItem("Drag to (" + list[1] + "," + list[2] + ")");
        }

        else if( list[0] == "scr" ) {
            QString item;
            item.append("Scroll ");
            if( list[2] == "0") item.append("up ");
            else if ( list[2] == "1" ) item.append("down ");
            item.append(list[1] + " times");
            ui->commandList->addItem(item);
        }

        else if( list[0] == "txt" ) {
            QString item;
            item.append("Write '" + list[2] + "' ");
            if( list[1] == "0" ) item.append("upper-case");
            else if ( list[1] == "1" )item.append("lower-case");
            ui->commandList->addItem(item);
        }

        else if( list[0] == "slp" ) {
            QString item;
            item.append("Sleep for " + list[1]);
            if( list[2] == "0") item.append(" Seconds");
            else if( list[2] == "1") item.append(" Milliseconds");
            ui->commandList->addItem(item);
        }

        else if( list[0] == "end" ) {
            ui->commandList->addItem("End process '" + list[1] + "'");
        }

        else if( list[0] == "exe" ) {
            QString item;
            item.append("Open '" + list[1] + "'");
            ui->commandList->addItem(item);
        }

        else if( list[0] == "sct" ) {
            QString item;
            item.append("Press '");
            if(list[1] == "1") item.append("CTRL + ");
            if(list[2] == "1") item.append("ALT + ");
            if(list[3] == "1") item.append("SHIFT + ");
            item.append(list[5]);
            item.append("' " + list[6] + " times");

            ui->commandList->addItem(item);
        }
    }

    ui->commandList->setCurrentRow(ui->commandList->count() - 1);

    if(ui->commandList->count() > 0) {
        ui->commandListTitle->setText("Command List [" + QString::number(ui->commandList->currentRow()) + "/"+ QString::number(commandList.size()) + "]");
        //ui->loopFrom->setMaximum(ui->commandList->count() - 1);
    }
    else {
        ui->commandListTitle->setText("Command List [0/"+ QString::number(commandList.size()) + "]");
        //ui->loopFrom->setMaximum(ui->commandList->count());
    }

    //ui->loopTo->setMaximum(ui->commandList->count());
    moveItem();
}


void MainWindow::optionsChanged(int dummy)
{
    dummy = 1;
    setUnsavedChanges(true);
}

//-------------------------------------------------------
//-----------------Add command Buttons-------------------
//-------------------------------------------------------
void MainWindow::addMoveCursorCommand()
{
    addCommand("mov", (QStringList() << QString::number(ui->xCoord->value()) << QString::number(ui->yCoord->value())));
}

void MainWindow::addClickCommand()
{
    addCommand("clc", (QStringList() << QString::number(ui->clickBox->currentIndex())));
}

void MainWindow::addDragMouseCommand()
{
    addCommand("drg", (QStringList() << QString::number(ui->xCoordDrag->value()) << QString::number(ui->yCoordDrag->value())));
}

void MainWindow::addScrollCommand()
{
    int scrollDir = ui->scrollDir->currentText() == "up" ? 0 : 1;
    addCommand("scr", (QStringList() << QString::number((ui->scrollAmount->value())) << QString::number(scrollDir)));
}

void MainWindow::addWriteTextCommand()
{
    addCommand("txt", (QStringList() << QString::number(ui->caseBox->currentIndex()) << ui->writeTextEditField->text()));
}

void MainWindow::updateWriteTextCount()
{
    ui->count->setText(QString::number(ui->writeTextEditField->text().length()) + " / " + QString::number(ui->writeTextEditField->maxLength()));
}

void MainWindow::addShortcutCommand()
{
    int ctrl = 0, alt = 0, shift = 0;
    QString key;
    int letter;

    if(ui->CTRLcheck->isChecked()) ctrl = 1;
    if(ui->ALTcheck->isChecked()) alt = 1;
    if(ui->SHIFTcheck->isChecked()) shift = 1;

    if(ui->letterBox->text().length() == 1 && ui->letterBox->text() != " ") {
        letter = 1;
        key = ui->letterBox->text();
    }
    else if(ui->keyBox->currentIndex() != 0) {
        letter = 0;
        key = ui->keyBox->currentText();
    }
    else
        return;

    addCommand("sct", (QStringList() << QString::number(ctrl) << QString::number(alt) << QString::number(shift) << QString::number(letter) <<
                       key << QString::number(ui->amount->value())));
}

void MainWindow::addOpenExeCommand()
{
    if(ui->exeName->text().length() > 5  && ui->exeName->text().contains(".exe"))
        addCommand("exe", (QStringList() << ui->exeName->text()));
}

void MainWindow::addSleepCommand()
{
    addCommand("slp",  (QStringList() << QString::number(ui->sleepTime->value()) << QString::number(ui->sleepComboBox->currentIndex())));
}

void MainWindow::addKillProcessCommand()
{
    QString name = ui->killProcName->text().trimmed();
    if(name.length() > 4 && name.contains(".exe") && !name.contains(" "))
        addCommand("end", (QStringList() << ui->killProcName->text()));
}

void MainWindow::refreshWindowTitle()
{
    setWindowTitle("Personal Macro - " + progName + (unsavedChanges ? "*" : ""));
}

void MainWindow::setUnsavedChanges(bool newUnsavedChanges)
{
    unsavedChanges = newUnsavedChanges;
    refreshWindowTitle();
}

//-------------------------------------------------------
//-----------------Choose a .exe file--------------------
//-------------------------------------------------------
void MainWindow::chooseExe()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose .exe"), QDir::currentPath(), tr("Executable Files (*.exe)"));
    if(fileName.length() > 1 && fileName.contains(".exe"))
        ui->exeName->setText(fileName);
    else
        ui->exeName->setText("Invalid file.");
}

//-------------------------------------------------------
//------------Delete the selected command----------------
//-------------------------------------------------------
void MainWindow::deleteCommand()
{
    if(ui->commandList->count() == 0)
        return;

    delBackupPos = ui->commandList->currentRow();
    delBackupText = commandList.at(delBackupPos);

    commandList.removeAt(delBackupPos);
    fillCommandListWidget();

    setUnsavedChanges(true);
    ui->delCmdUndo->setEnabled(true);

    if(delBackupPos == ui->commandList->count()) ui->commandList->setCurrentRow(ui->commandList->count() - 1);
    else ui->commandList->setCurrentRow(delBackupPos);
}

//-------------------------------------------------------
//---------------Undo the last deletion------------------
//-------------------------------------------------------
void MainWindow::deleteUndo()
{
    commandList.insert(delBackupPos, delBackupText);
    fillCommandListWidget();
    setUnsavedChanges(true);
    ui->delCmdUndo->setEnabled(false);
    ui->commandList->setCurrentRow(delBackupPos);
}

void MainWindow::moveItem()
{
    if(ui->commandList->count() > 0)
    {
        ui->delCmd->setEnabled(true);
        ui->botstart->setEnabled(true);
        ui->addAbove->setEnabled(true);
        ui->addAbove->setCheckable(true);

        if(ui->commandList->count() > 1) {
            if(ui->commandList->currentRow() == 0) {
                ui->movedown->setEnabled(true);
                ui->moveup->setEnabled(false);
            }
            else if(ui->commandList->currentRow() == ui->commandList->count()-1) {
                ui->moveup->setEnabled(true);
                ui->movedown->setEnabled(false);
            }
            else {
                ui->moveup->setEnabled(true);
                ui->movedown->setEnabled(true);
            }
        }
        ui->commandListTitle->setText("Command List [" + QString::number(ui->commandList->currentRow()+1) + "/"+ QString::number(commandList.size()) + "]");
    }
    else
    {
        ui->delCmd->setEnabled(false);
        ui->botstart->setEnabled(false);
        ui->addAbove->setEnabled(false);
        ui->addAbove->setCheckable(false);
        ui->movedown->setEnabled(false);
        ui->moveup->setEnabled(false);
        ui->commandListTitle->setText("Command List [0/"+ QString::number(commandList.size()) + "]");
    }
}

void MainWindow::moveIt(int dir)
{
    QScrollBar *vb = ui->commandList->verticalScrollBar();
    int oldValue = vb->value();

    int curIndex = ui->commandList->currentRow();
    int newIndex = curIndex + dir;

    commandList.swap(curIndex, newIndex);
    fillCommandListWidget();

    setUnsavedChanges(true);

    ui->commandList->setCurrentRow(newIndex);
    vb->setValue(oldValue);
}

//-------------------------------------------------------
//-----Called when the letterbox editing finished--------
//-------------------------------------------------------
void MainWindow::letterBoxEdited()
{
    QString text = ui->letterBox->text();
    ui->keyBox->setCurrentIndex(0);
    ui->letterBox->setText(text);
}

//-------------------------------------------------------
//-------Called when the keybox editing finished---------
//-------------------------------------------------------
void MainWindow::keyBoxEdited()
{
    int currentPos = ui->keyBox->currentIndex();
    ui->letterBox->setText("");
    ui->keyBox->setCurrentIndex(currentPos);
}

//-------------------------------------------------------
//------------------Start the bot------------------------
//-------------------------------------------------------
void MainWindow::botStart()
{
    ui->botstart->setEnabled(false);
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
                ui->botstart->setEnabled(true);
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
        for(int i = 0; i < commandList.size(); ++i)
        {
            if(GetAsyncKeyState(VK_F7)) paused = false;
            if(GetAsyncKeyState(VK_F8)) paused = true;
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
                        setWindowTitle("Bot looping w/ delay");
                    else setWindowTitle("Bot running w/ delay");
                }
                else
                {
                    if(ui->loopCheckBox->isChecked())
                        setWindowTitle("Bot looping w/o delay");
                    else setWindowTitle("Bot running w/o delay");
                }

                if(i == stop && partLoop == true && times > 0)
                {
                    i = start - 1;
                    --times;
                }

                executeCommand(commandList.at(i));
                if(ui->defaultDelayCheckBox->isChecked())
                    Sleep(ui->defaultDelaySpinBox->value());
                qApp->processEvents();
            }
            else
            {
                --i;
                setWindowTitle("Bot paused");
                qApp->processEvents();
                Sleep(50);
            }
        }
    }while(loop);

    refreshWindowTitle();
    showNormal();
    ui->botstart->setEnabled(true);
}

//-------------------------------------------------------
//-----------------Execute Command-----------------------
//-------------------------------------------------------
void MainWindow::executeCommand(QString cmd)
{
    QStringList command = cmd.split("|");

    if( command[0] == "mov" )
    {
        SetCursorPos( command[1].toInt(), command[2].toInt() );
        Sleep(50);
        if(command[3] == "1")
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(50);
        }
    }
    else if( command[0] == "clc" )
    {
        if( command[1] == "0" )
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(50);
        }
        else if( command[1] == "1" )
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(50);
        }
        else if( command[1] == "2" )
        {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            Sleep(50);
        }
        else if( command[1] == "3" )
        {
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
            Sleep(50);
        }
    }
    else if( command[0] == "drg" )
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        Sleep(10);
        SetCursorPos(command[1].toInt(), command[2].toInt());
        Sleep(10);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        Sleep(50);
    }
    else if( command[0] == "scr" )
    {
        int direction = command[2] == "0" ? SCROLLUP : SCROLLDOWN;
        for(int i = 0; i<=command[1].toInt(); ++i)
        {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, direction, 0);
            Sleep(50);
        }
    }
    else if( command[0] == "txt" )
    {
        if( command[1] == "0" )
        {
            keybd_event(VK_SHIFT, 0xAA, 0, 0);
            Sleep(50);
        }

        KeyboardUtilities::writeText(command[2].toStdString());

        if( command[1] == "0" )
        {
            keybd_event(VK_SHIFT, 0xAA, KEYEVENTF_KEYUP, 0);
            Sleep(50);
        }
    }
    else if( command[0] == "slp" )
    {
        int multi = command[2] == "0" ? 1000 : 1;
        Sleep(command[1].toInt() * multi);
    }
    else if( command[0] == "end" )
    {
        WinExec("taskkill /F /IM " + command[1].toUtf8(), SW_HIDE);
        Sleep(50);
    }
    else if( command[0] == "exe" )
    {
        WinExec(command[1].toUtf8(), SW_NORMAL);
        Sleep(50);
    }
    else if( command[0] == "sct" )
    {
        for(int i = 0; i < command[6].toInt(); ++i)
        {
            if(command[1] == "1")
                keybd_event(VK_CONTROL, 0x9D, 0, 0);
            if(command[2] == "1")
                keybd_event(VK_MENU, 0xB8, 0, 0);
            if(command[3] == "1")
                keybd_event(VK_SHIFT, 0xAA, 0, 0);

            if(command[4] == "1")
                KeyboardUtilities::writeText(command[5].toStdString());
            else
                KeyboardUtilities::pressVK(command[5].toStdString());

            if(command[1] == "1")
                keybd_event(VK_CONTROL, 0x9D, KEYEVENTF_KEYUP, 0);
            if(command[2] == "1")
                keybd_event(VK_MENU, 0xB8, KEYEVENTF_KEYUP, 0);
            if(command[3] == "1")
                keybd_event(VK_SHIFT, 0xAA, KEYEVENTF_KEYUP, 0);
            Sleep(50);
        }
    }
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
