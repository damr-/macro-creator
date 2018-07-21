#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QList>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QSignalMapper>
#include <QStringList>
#include <QTimer>
#include <QPushButton>

#include "defaultdelaywidget.h"

#include "commands.h"
#include "CmdWidgets/clickcmdwidget.h"
#include "CmdWidgets/setcursorposcmdwidget.h"
#include "CmdWidgets/delaycmdwidget.h"
#include "CmdWidgets/dragcmdwidget.h"
#include "CmdWidgets/writetextcmdwidget.h"
#include "CmdWidgets/runexecmdwidget.h"
#include "CmdWidgets/hitkeycmdwidget.h"
#include "CmdWidgets/scrollcmdwidget.h"

//#pragma comment(lib, "user32.lib")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    //CheckKey Timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkUserKeyInput()));
    timer->start(50);

    //Menu->File Actions
    connect(ui->actionFExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionFNew, SIGNAL(triggered()), this, SLOT(newMacro()));
    connect(ui->actionFSave, SIGNAL(triggered()), this, SLOT(saveMacro()));
    connect(ui->actionFOpen, SIGNAL(triggered()), this, SLOT(openMacro()));
    connect(ui->actionFSaveAs, SIGNAL(triggered()), this, SLOT(saveMacroAs()));

    //Menu->Edit Actions
    connect(ui->actionECopy, SIGNAL(triggered()), this, SLOT(copySelected()));
    connect(ui->actionECut, SIGNAL(triggered()), this, SLOT(cutSelected()));
    connect(ui->actionEPaste, SIGNAL(triggered()), this, SLOT(pasteClipboard()));
    connect(ui->actionEDelete, SIGNAL(triggered()), this, SLOT(deleteSelected()));
    connect(ui->actionEDuplicate, SIGNAL(triggered()), this, SLOT(duplicateSelected()));

    //Menu->Add Actions
    QSignalMapper *addActionsMapper = new QSignalMapper(this);
    connect(ui->action1Delay, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action2Click, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action3CursorPos, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action4Drag, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action5Scroll, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action6Write, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action7HitKey, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action8RunExe, SIGNAL(triggered()), addActionsMapper, SLOT(map()));

    addActionsMapper->setMapping(ui->action1Delay, 0);
    addActionsMapper->setMapping(ui->action2Click, 1);
    addActionsMapper->setMapping(ui->action3CursorPos, 2);
    addActionsMapper->setMapping(ui->action4Drag, 3);
    addActionsMapper->setMapping(ui->action5Scroll, 4);
    addActionsMapper->setMapping(ui->action6Write, 5);
    addActionsMapper->setMapping(ui->action7HitKey, 6);
    addActionsMapper->setMapping(ui->action8RunExe, 7);
    connect(addActionsMapper, SIGNAL(mapped(int)), this, SLOT(addNewCommand(int)));

    //Menu->Run Actions
    connect(ui->actionRStart, SIGNAL(triggered(bool)), this, SLOT(tryRunMacro()));

    //TODO: Different Context menu depending on clicked QWidget
    //https://stackoverflow.com/questions/12937812/how-to-create-different-popup-context-menus-for-each-type-of-qtreewidgetitem#
    //TODO: Search bar in context menu when right clicking on command list widget:
    // http://blog.qt.io/blog/2012/12/07/qt-support-28-creating-a-toolbar-widget-that-is-displayed-with-the-context-menu/
    ui->commandList->setContextMenuPolicy(Qt::CustomContextMenu);

    //Context Menu
    QList<QAction *> editActions;
    QList<QAction *> addActions;
    QList<QMenu*> menuList = ui->menuBar->findChildren<QMenu*>();
    foreach(QMenu* menu, menuList)
    {
        if(menu->title() == "Edit")
        {
            foreach (QAction* a, menu->actions())
                editActions.append(a);
        }
        else if(menu->title() == "Add")
        {
            foreach (QAction* a, menu->actions())
                addActions.append(a);
        }
    }
    contextMenu.addMenu("Add")->addActions(addActions);
    contextMenu.addActions(editActions);
    connect(ui->commandList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    //

    //Handle selection of cmds
    connect(ui->commandList, SIGNAL(itemSelectionChanged()), this, SLOT(handleSelectionChanged()));

    //react to drag&drop events
    connect(ui->commandList->model(),
            SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this,
            SLOT(handleRowMoved(QModelIndex,int,int,QModelIndex,int)));

    //disable actions by default
    handleSelectionChanged();

    //start with unnamed project
    macroName = "unnamed";
    macroPath = QDir::currentPath();
    setUnsavedChanges(false);
    isUnsavedMacro = true;
    ui->actionFSaveAs->setEnabled(false);

    //Setup default delay widget
    defaultDelayWidget = new DefaultDelayWidget(this);
    ui->toolBar->addWidget(defaultDelayWidget);
    connect(defaultDelayWidget, SIGNAL(SettingsChanged(DefaultDelaySettings*)), this, SLOT(setUnsavedChanges()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!hasUnsavedChanges) {
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
            saveMacro();
        event->accept();
    }
    msgbox->close();
}

QMessageBox *MainWindow::showUnsavedChangesWarning(UnsavedChangesMessageResult &result)
{
    QMessageBox *msgbox = new QMessageBox(this);
    QPushButton *btn1 = new QPushButton("Save");
    QPushButton *btn2 = new QPushButton("Don't save");
    QPushButton *btn3 = new QPushButton("Cancel");

    msgbox->setText(tr("There are unsaved changes which will be lost."));
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

void MainWindow::showMessage(QString title, QString message, QMessageBox::Icon type)
{
    switch(type)
    {
    case QMessageBox::Icon::Information:
        QMessageBox::information(this, title, message);
        break;
    case QMessageBox::Icon::Warning:
        QMessageBox::warning(this, title, message);
        break;
    default:
        QMessageBox::critical(this, title, message);
        break;
    }
}

void MainWindow::checkUserKeyInput()
{
    if(GetAsyncKeyState(VK_F6) && !isMacroRunning)
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);

        LONG x = cursorPos.x;
        LONG y = cursorPos.y;

        if(ui->commandList->selectedItems().length() == 0)
        {
            CmdWidget *w = addNewCommand(int(CmdType::CURPOS));
            SetCursorPosCmdWidget* widget = qobject_cast<SetCursorPosCmdWidget*>(w);
            widget->SetCoordinates(x, y);
            ui->statusBar->showMessage("Added new 'Cursor position' command for (" + QString::number(x) + "," + QString::number(y) + ")", 5000);
        }
        else
        {
            QList<QListWidgetItem *> selItems = ui->commandList->selectedItems();
            QListWidgetItem *item;

            foreach(item, selItems)
            {
                CmdWidget *widget = qobject_cast<CmdWidget*>(ui->commandList->itemWidget(item));

                switch(widget->GetCmdType())
                {
                    case CmdType::CURPOS:
                        qobject_cast<SetCursorPosCmdWidget*>(widget)->SetCoordinates(x, y);
                        ui->statusBar->showMessage("Updated 'Cursor position': (" + QString::number(x) + "," + QString::number(y) + ")", 5000);
                        break;
                    case CmdType::DRAG:
                        qobject_cast<DragCmdWidget*>(widget)->SetCoordinates(x, y);
                        ui->statusBar->showMessage("Updated 'Drag': (" + QString::number(x) + "," + QString::number(y) + ")", 5000);
                        break;
                    default:
                        ui->statusBar->showMessage("Select nothing, a 'Cursor Position' or a 'Drag' command.", 5000);
                        break;
                }
            }

            if(selItems.length() > 1)
            {
                ui->statusBar->showMessage("Updated all selected 'Cursor position' and 'Drag' to (" + QString::number(x) + "," + QString::number(y) + ")", 5000);
            }
        }
    }
    if(GetAsyncKeyState(VK_F7))
    {
        tryRunMacro();
    }
}

void MainWindow::newMacro()
{
    MainWindow *m = new MainWindow();
    if(close())
        m->show();
}

void MainWindow::openMacro()
{
    QString fullFilePath = QFileDialog::getOpenFileName(this, tr("Open macro"), macroPath, fileInfo);

    QString fileName = QFileInfo(fullFilePath).baseName();
    if(fileName.length() == 0)
    {
        ui->statusBar->showMessage("Opening aborted", 3000);
        return;
    }

    macroName = fileName;

    if(hasUnsavedChanges){
        UnsavedChangesMessageResult result = UnsavedChangesMessageResult::Cancel;
        QMessageBox* msgbox = showUnsavedChangesWarning(result);

        if(result == UnsavedChangesMessageResult::Cancel){
            ui->statusBar->showMessage("Opening aborted", 3000);
            return;
        }
        if(result == UnsavedChangesMessageResult::Save)
            saveMacro();
        msgbox->close();
    }

    if(!tryLoadCmdsFromFile(fullFilePath))
        return;

    ui->statusBar->showMessage("Opened " + fullFilePath, 3000);
    isUnsavedMacro = false;
    setUnsavedChanges(false);
    RefreshWindowTitle();
    ui->actionFSaveAs->setEnabled(true);
}

void MainWindow::saveMacro()
{
    if(!isUnsavedMacro && !hasUnsavedChanges)
    {
        ui->statusBar->showMessage("No changes to save", 3000);
        return;
    }

    QString pathPlusFileName = getFullFilePath(macroPath, macroName);

    if(isUnsavedMacro){
        pathPlusFileName = QFileDialog::getSaveFileName(this, tr("Save the macro as..."), macroPath, fileInfo);

        QString fileName = QFileInfo(pathPlusFileName).baseName();
        if(fileName.length() == 0)
        {
            ui->statusBar->showMessage("Saving aborted", 3000);
            return;
        }
        macroName = fileName;
        isUnsavedMacro = false;
    }

    QFile file(pathPlusFileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        showMessage("Error", "Unable to open the file!", QMessageBox::Icon::Critical);
        return;
    }

    QTextStream output(&file);
    QString out;

    DefaultDelaySettings *settings = defaultDelayWidget->GetSettings();
    out.append(QString::number(settings->enabled) + "|" +
               QString::number(settings->amount) + "|" +
               QString::number(settings->timeScale) + "\n");

    for(int i = 0; i < ui->commandList->count(); ++i)
    {
        out.append(getCmdString(i));
        out.append("\n");
    }

    output << out;
    file.close();

    setUnsavedChanges(false);
    ui->statusBar->showMessage("Saved macro to " + pathPlusFileName, 3000);
}

void MainWindow::saveMacroAs()
{
    isUnsavedMacro = true;
    saveMacro();
}

bool MainWindow::tryLoadCmdsFromFile(QString filename)
{
    ui->commandList->clear();

    QFile file(filename);

    if(!file.exists())
        return false;

    if(!file.open(QFile::ReadOnly))
        return false;

    QTextStream input(&file);
    QStringList options = input.readLine().split("|");

    if(options.size() != 3){
        showMessage("Error", filename + " is corrupted.\nAborted opening process.", QMessageBox::Icon::Critical);
        ui->statusBar->showMessage("Opening aborted", 3000);
        file.close();
        return false;
    }

    //Apply saved options
    DefaultDelaySettings *settings = new DefaultDelaySettings();
    settings->enabled = options[0].toInt();
    settings->amount = options[1].toInt();
    settings->timeScale = options[2].toInt();
    defaultDelayWidget->SetSettings(settings);

    //Load rest of commands
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
    return true;
}

void MainWindow::fillCommandListWidget(QStringList commandListStrings)
{
    ui->commandList->clear();

    for(int i = 0; i < commandListStrings.size(); ++i)
    {
        QStringList list = commandListStrings.at(i).split("|");

        CmdType commandTypeIndex = CmdType(list[0].toInt());

        QListWidgetItem *newItem = new QListWidgetItem();
        CmdWidget *newWidget = CmdWidget::GetNewCommandWidget(commandTypeIndex);

        switch(commandTypeIndex){
            case CmdType::DELAY:
                qobject_cast<DelayCmdWidget*>(newWidget)->SetWaitSettings(list[1].toInt(), list[2].toInt()); break;
            case CmdType::CLICK:
                qobject_cast<ClickCmdWidget*>(newWidget)->SetClickSettings(list[1].toInt(), static_cast<ClickType>(list[2].toInt())); break;
            case CmdType::CURPOS:
                qobject_cast<SetCursorPosCmdWidget*>(newWidget)->SetCoordinates(list[1].toInt(), list[2].toInt());  break;
            case CmdType::DRAG:
                qobject_cast<DragCmdWidget*>(newWidget)->SetCoordinates(list[1].toInt(), list[2].toInt()); break;
            case CmdType::SCROLL:
                qobject_cast<ScrollCmdWidget*>(newWidget)->SetScrollSettings(list[1].toInt(), list[2].toInt()); break;
            case CmdType::WRITETEXT:
                qobject_cast<WriteTextCmdWidget*>(newWidget)->SetText(list[1]); break;
            case CmdType::RUNEXE:
                qobject_cast<RunExeCmdWidget*>(newWidget)->SetFilePath(list[1]); break;
            //TODO CmdType::HITKEY
            default:
                break;
        }
        addCmdListItem(newItem, newWidget, i);

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

    updateRowNumbers();
    ui->commandList->setCurrentRow(ui->commandList->count() - 1);
}

void MainWindow::RefreshWindowTitle()
{
    setWindowTitle(macroName + (hasUnsavedChanges ? "*" : "") + " - Personal Macro");
}

void MainWindow::setUnsavedChanges()
{
    setUnsavedChanges(true);
}

void MainWindow::setUnsavedChanges(bool newUnsavedChanges)
{
    hasUnsavedChanges = newUnsavedChanges;
    RefreshWindowTitle();
}

QString MainWindow::getCmdString(int commandListIndex)
{
    QListWidgetItem* item = ui->commandList->item(commandListIndex);
    QWidget* itemWidget = ui->commandList->itemWidget(item);
    return qobject_cast<CmdWidget*>(itemWidget)->GetCmdString();
}

void MainWindow::tryRunMacro()
{
    if(isMacroRunning)
        return;

    int result = AllCommandsValid();
    if(result != -1)
    {
        QString msg = QString("Possible error in row " + QString::number(result + 1));
        showMessage("Warning", msg, QMessageBox::Icon::Warning);
        ui->statusBar->showMessage(msg, 10000);

        ui->commandList->setCurrentRow(result);
        unselectAll();
        ui->commandList->item(result)->setSelected(true);
        return;
    }

    isMacroRunning = true;
    setWindowTitle(macroName + " - Running");
    setWindowOpacity(0.5);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput);// | Qt::WindowDoesNotAcceptFocus);
    show();

    ExecuteCommands();

    setWindowOpacity(1);
    setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint & ~Qt::WindowTransparentForInput);// & ~Qt::WindowDoesNotAcceptFocus);
    show();

    isMacroRunning = false;
    RefreshWindowTitle();
}

int MainWindow::AllCommandsValid()
{
    QListWidgetItem *item;

    for(int i = 0, total = ui->commandList->count(); i < total; i++)
    {
        item = ui->commandList->item(i);
        CmdWidget *widget = qobject_cast<CmdWidget *>(ui->commandList->itemWidget(item));
        if(!widget->IsValidCmd())
            return i;
    }
    return -1;
}

void MainWindow::ExecuteCommands()
{
    bool paused = false;
    DefaultDelaySettings *s = defaultDelayWidget->GetSettings();

    for(int i = 0, total = ui->commandList->count(); i < total; i++)
    {
        if(GetAsyncKeyState(VK_F7))
        {
            setWindowTitle(macroName + " - Running");
            paused = false;
        }
        if(GetAsyncKeyState(VK_F8))
        {
            setWindowTitle(macroName + " - Paused");
            paused = true;
        }
        if(GetAsyncKeyState(VK_F9))
            break;

        if(!paused)
        {
            Commands::ExecuteCommand(getCmdString(i));

            if(s->enabled)
                Sleep(DWORD(s->amount) * (s->timeScale == 0 ? 1000 : 1));
            qApp->processEvents();

            //Internal default delay (maybe not necessary)
            Sleep(50);
        }
        else
        {
            i--;
            qApp->processEvents();
            Sleep(50);
        }
    }
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->commandList->mapToGlobal(pos);
    contextMenu.exec(globalPos);
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
    for (int i = 0, total = selectedItems.size(); i < total; ++i)
    {
        QListWidgetItem *item = static_cast<QListWidgetItem *>(selectedItems.at(i));
        QListWidgetItem *newItem = new QListWidgetItem();

        CmdWidget *selectedItemWidget = qobject_cast<CmdWidget*>(ui->commandList->itemWidget(item));
        CmdType selectedItemCommandType = selectedItemWidget->GetCmdType();

        //create new widget
        CmdWidget *newWidget = CmdWidget::GetNewCommandWidget(selectedItemCommandType);

        //copy widget values to new one
        selectedItemWidget->CopyTo(newWidget);

        addCmdListItem(newItem, newWidget, ui->commandList->row(item) + 1);
        newItems.append(newItem);
    }

    ui->commandList->setCurrentRow(ui->commandList->row(newItems.at(0)));
    unselectAll();

    QListWidgetItem *i;
    foreach(i, newItems)
        i->setSelected(true);

    updateRowNumbers();
    setUnsavedChanges(true);
}

void MainWindow::updateRowNumbers()
{
    QListWidgetItem *item;
    for(int i = 0, total = ui->commandList->count(); i < total; i++)
    {
        item = ui->commandList->item(i);
        CmdWidget *widget = qobject_cast<CmdWidget *>(ui->commandList->itemWidget(item));
        widget->SetRowNumber(i + 1);
    }
}

void MainWindow::copySelected()
{

}

void MainWindow::cutSelected()
{

}

void MainWindow::pasteClipboard()
{

}

void MainWindow::unselectAll()
{
    QList<QListWidgetItem *> selectedItems = ui->commandList->selectedItems();
    for (int i = 0; i < selectedItems.size(); ++i)
    {
        QListWidgetItem *item = selectedItems.at(i);
        item->setSelected(false);
    }
}

CmdWidget* MainWindow::addNewCommand(int cmdType)
{
    CmdWidget *itemWidget = CmdWidget::GetNewCommandWidget(CmdType(cmdType));

    QListWidgetItem *item = new QListWidgetItem();

    int row = ui->commandList->currentRow() + 1;
    if(ui->commandList->selectedItems().isEmpty())
        row = ui->commandList->count();

    addCmdListItem(item, itemWidget, row);

    ui->commandList->setCurrentRow(row);
    unselectAll();
    item->setSelected(true);
    setUnsavedChanges(true);

    updateRowNumbers();
    return itemWidget;
}

void MainWindow::addCmdListItem(QListWidgetItem *item, CmdWidget *itemWidget, int row)
{
    ui->commandList->insertItem(row, item);
    ui->commandList->setItemWidget(item, itemWidget);
    item->setSizeHint(QSize(0, itemWidget->height()));
    connect(itemWidget, SIGNAL(commandChanged()), this, SLOT(handleCommandSettingChanged()));
}

void MainWindow::handleCommandSettingChanged()
{
    setUnsavedChanges(true);
}

void MainWindow::handleSelectionChanged()
{
    bool itemSelected = ui->commandList->selectedItems().count() > 0;

    ui->actionEDuplicate->setEnabled(itemSelected);
    ui->actionEDelete->setEnabled(itemSelected);
    ui->actionECopy->setEnabled(itemSelected);
    ui->actionECut->setEnabled(itemSelected);
    ui->actionEPaste->setEnabled(itemSelected);
}

void MainWindow::handleRowMoved(QModelIndex, int, int, QModelIndex, int)
{
    updateRowNumbers();
    setUnsavedChanges(true);
}
