#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QScrollBar>
#include <QSignalMapper>
#include <QStringList>
#include <QTextStream>
#include <QTimer>

#include "defaultdelaywidget.h"

#include "commands.h"
#include "CmdWidgets/clickcmdwidget.h"
#include "CmdWidgets/setcursorposcmdwidget.h"
#include "CmdWidgets/delaycmdwidget.h"
#include "CmdWidgets/dragcmdwidget.h"
#include "CmdWidgets/writetextcmdwidget.h"
#include "CmdWidgets/runexecmdwidget.h"
#include "CmdWidgets/presskeycmdwidget.h"
#include "CmdWidgets/scrollcmdwidget.h"
#include "CmdWidgets/presskeycmdwidget.h"

//#pragma comment(lib, "user32.lib")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    //Figure out and set maximum possible size for the window
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect = screen->geometry();
    QSize maxSize = QSize(rect.width(), rect.height());
    setMaximumSize(maxSize);

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
    connect(ui->action6PressKey, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action7Write, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action8RunExe, SIGNAL(triggered()), addActionsMapper, SLOT(map()));

    addActionsMapper->setMapping(ui->action1Delay, 0);
    addActionsMapper->setMapping(ui->action2Click, 1);
    addActionsMapper->setMapping(ui->action3CursorPos, 2);
    addActionsMapper->setMapping(ui->action4Drag, 3);
    addActionsMapper->setMapping(ui->action5Scroll, 4);
    addActionsMapper->setMapping(ui->action6PressKey, 5);
    addActionsMapper->setMapping(ui->action7Write, 6);
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

    //Setup default delay widget
    defaultDelayWidget = new DefaultDelayWidget(this);
    ui->toolBar->addWidget(defaultDelayWidget);
    connect(defaultDelayWidget, &DefaultDelayWidget::SettingsChanged, [=](){ setUnsavedChanges(true); });

    //start with unnamed macro
    setupBlankMacro();

    //Store default window properties
    show();
    defX = x();
    defY = y();
    defW = width();
    defH = height();
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
    msgbox->setIcon(QMessageBox::Icon::Warning);

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
            widget->SetCmdSettings(x, y);
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
                        qobject_cast<SetCursorPosCmdWidget*>(widget)->SetCmdSettings(x, y);
                        ui->statusBar->showMessage("Updated 'Cursor position': (" + QString::number(x) + "," + QString::number(y) + ")", 5000);
                        break;
                    case CmdType::DRAG:
                        qobject_cast<DragCmdWidget*>(widget)->SetCmdSettings(x, y);
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

void MainWindow::setupBlankMacro()
{
    macroName = "unnamed";
    macroPath = QDir::currentPath();
    isUnsavedMacro = true;

    ui->actionFSaveAs->setEnabled(false);

    ui->commandList->clear();
    ui->statusBar->clearMessage();

    defaultDelayWidget->Reset();
    setUnsavedChanges(false);
}

void MainWindow::newMacro()
{
    if(hasUnsavedChanges)
    {
        UnsavedChangesMessageResult result = UnsavedChangesMessageResult::Cancel;
        QMessageBox* msgbox = showUnsavedChangesWarning(result);

        if(result == UnsavedChangesMessageResult::Cancel)
            return;
        if(result == UnsavedChangesMessageResult::Save)
            saveMacro();
        msgbox->close();
    }

    setupBlankMacro();
    move(defX, defY);
    resize(defW, defH);
}

void MainWindow::saveMacro()
{
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

    //Write window properties
    out.append(QString::number(x()) + "|" +
               QString::number(y()) + "|" +
               QString::number(width()) + "|" +
               QString::number(height()) + "|" +
               QString::number(isMaximized()) + "\n");

    //Write default delay settings
    DefaultDelaySettings *settings = defaultDelayWidget->GetSettings();
    out.append(QString::number(settings->enabled) + "|" +
               QString::number(settings->amount) + "|" +
               QString::number(settings->timeScale) + "\n");

    //Write commands
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

void MainWindow::openMacro()
{
    QString fullFilePath = QFileDialog::getOpenFileName(this, tr("Open macro"), macroPath, fileInfo);

    QString fileName = QFileInfo(fullFilePath).baseName();
    if(fileName.length() == 0)
    {
        ui->statusBar->showMessage("Opening aborted", 3000);
        setUnsavedChanges(false);
        newMacro();
        return;
    }

    if(hasUnsavedChanges)
    {
        UnsavedChangesMessageResult result = UnsavedChangesMessageResult::Cancel;
        QMessageBox* msgbox = showUnsavedChangesWarning(result);

        if(result == UnsavedChangesMessageResult::Cancel){
            ui->statusBar->showMessage("Opening aborted", 3000);
            setUnsavedChanges(false);
            newMacro();
            return;
        }
        if(result == UnsavedChangesMessageResult::Save)
            saveMacro();
        msgbox->close();
    }

    macroName = fileName;

    if(!tryLoadCmdsFromFile(fullFilePath))
    {
        setUnsavedChanges(false);
        newMacro();
        return;
    }

    ui->statusBar->showMessage("Opened " + fullFilePath, 3000);
    isUnsavedMacro = false;
    setUnsavedChanges(false);
    ui->actionFSaveAs->setEnabled(true);
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

    //Read and apply window size
    QStringList options = input.readLine().split("|");
    if(options.size() != WINDOW_OPTIONS_LEN)
    {
        showMessage("Error", filename + " is corrupted.\nAborted opening process.", QMessageBox::Icon::Critical);
        ui->statusBar->showMessage("Opening aborted", 3000);
        file.close();
        return false;
    }

    int x = options[0].toInt();
    int y = options[1].toInt();
    int width  = options[2].toInt();
    int height = options[3].toInt();
    bool isMaximized = options[4].toInt();

    move(x, y);
    resize(width, height);
    if(isMaximized)
        showMaximized();

    //Read and apply default delay options
    options = input.readLine().split("|");
    if(options.size() != DELAY_OPTIONS_LEN)
    {
        showMessage("Error", filename + " is corrupted.\nAborted opening process.", QMessageBox::Icon::Critical);
        ui->statusBar->showMessage("Opening aborted", 3000);
        file.close();
        return false;
    }

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

    if(!fillCommandListWidget(commandListStrings))
    {
        showMessage("Error", filename + " is corrupted.\nAborted opening process.", QMessageBox::Icon::Critical);
        ui->statusBar->showMessage("Opening aborted", 3000);
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool MainWindow::fillCommandListWidget(QStringList commandListStrings)
{
    for(int i = 0; i < commandListStrings.size(); i++)
    {
        QStringList list = commandListStrings.at(i).split("|");
        CmdType commandTypeIndex = CmdType(list[0].toInt());

        QListWidgetItem *newItem = new QListWidgetItem();
        CmdWidget *newWidget = CmdWidget::GetNewCommandWidget(commandTypeIndex);

        if(list.length() != newWidget->GetCmdStrLen())
        {
            ui->commandList->clear();
            return false;
        }

        switch(commandTypeIndex){
            case CmdType::DELAY:
                qobject_cast<DelayCmdWidget*>(newWidget)->SetCmdSettings(
                            list[DelayCmdWidget::AmountIdx].toInt(),
                            list[DelayCmdWidget::ScaleIdx].toInt());
                break;
            case CmdType::CLICK:
                qobject_cast<ClickCmdWidget*>(newWidget)->SetCmdSettings(
                            list[ClickCmdWidget::AmountIdx].toInt(),
                            static_cast<ClickType>(list[ClickCmdWidget::TypeIdx].toInt()));
                break;
            case CmdType::CURPOS:
                qobject_cast<SetCursorPosCmdWidget*>(newWidget)->SetCmdSettings(
                            list[SetCursorPosCmdWidget::XIdx].toInt(),
                            list[SetCursorPosCmdWidget::YIdx].toInt());
                break;
            case CmdType::DRAG:
                qobject_cast<DragCmdWidget*>(newWidget)->SetCmdSettings(
                            list[DragCmdWidget::XIdx].toInt(),
                            list[DragCmdWidget::YIdx].toInt());
                break;
            case CmdType::SCROLL:
                qobject_cast<ScrollCmdWidget*>(newWidget)->SetCmdSettings(
                            list[ScrollCmdWidget::AmountIdx].toInt(),
                            list[ScrollCmdWidget::DirIdx].toInt());
                break;
            case CmdType::PRESSKEY:
            {
                PressKeyCmdWidget::Modifiers mod = PressKeyCmdWidget::Modifiers(
                            list[PressKeyCmdWidget::ModCTRLIdx].toInt(),
                            list[PressKeyCmdWidget::ModSHIFTIdx].toInt(),
                            list[PressKeyCmdWidget::ModALTIdx].toInt());

                qobject_cast<PressKeyCmdWidget*>(newWidget)->SetCmdSettings(
                            mod,
                            KeyType(list[PressKeyCmdWidget::KeyTypeIdx].toInt()),
                            list[PressKeyCmdWidget::LetterIdx],
                            list[PressKeyCmdWidget::SeqLetterIdx],
                            list[PressKeyCmdWidget::SpcKeyIndexIdx].toInt());
                break;
            }
            case CmdType::WRITETEXT:
                qobject_cast<WriteTextCmdWidget*>(newWidget)->SetCmdSettings(
                            list[WriteTextCmdWidget::TypeIdx].toInt(),
                            list[WriteTextCmdWidget::CharsIdx],
                            list[WriteTextCmdWidget::AmountIdx].toInt(),
                            list[WriteTextCmdWidget::TextIdx]);
                break;
            case CmdType::RUNEXE:
            {
                QString path = list[RunExeCmdWidget::PathIdx];
                path = path.length() == 0 ? "" : path;
                qobject_cast<RunExeCmdWidget*>(newWidget)->SetCmdSettings(path);
                break;
            }
        }
        addCmdListItem(newItem, newWidget, i);
    }

    updateRowNumbers();
    ui->commandList->setCurrentRow(ui->commandList->count() - 1);
    return true;
}

void MainWindow::setUnsavedChanges(bool newUnsavedChanges = true)
{
    hasUnsavedChanges = newUnsavedChanges;
    setWindowTitle(macroName + (hasUnsavedChanges ? "*" : "") + " - Personal Macro");
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

    if(ui->commandList->count() == 0)
    {
        showMessage("Error", "Macro is empty", QMessageBox::Icon::Critical);
        return;
    }

    int result = AllCommandsValid();
    if(result != -1)
    {
        QString msg = QString("Possible error in row " + QString::number(result + 1));
        showMessage("Warning", msg, QMessageBox::Icon::Warning);
        ui->statusBar->showMessage(msg, 10000);

        selectRow(result);
        return;
    }

    isMacroRunning = true;
    selectRow(0);
    setWindowTitle(macroName + " - Running");
    setWindowOpacity(0.75);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput);
    show();

    ExecuteCommands();

    setWindowOpacity(1);
    setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint & ~Qt::WindowTransparentForInput);
    show();

    setWindowTitle(macroName + " - Personal Macro");
    activateWindow();
    setFocus();
    raise();
    isMacroRunning = false;
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
        if(GetAsyncKeyState(VK_F7) && paused)
        {
            setWindowTitle(macroName + " - Running");
            paused = false;
        }
        if(GetAsyncKeyState(VK_F8) && !paused)
        {
            setWindowTitle(macroName + " - Paused");
            paused = true;
        }
        if(GetAsyncKeyState(VK_F9))
            break;

        if(!paused)
        {
            selectRow(i);
            qApp->processEvents();

            Commands::ExecuteCommand(getCmdString(i));

            if(s->enabled)
                Sleep(DWORD(s->amount) * (s->timeScale == 0 ? 1000 : 1));
        }
        else
        {
            i--;
            qApp->processEvents();
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
    updateRowNumbers();
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

    selectRow(ui->commandList->row(newItems.at(0)));

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

CmdWidget* MainWindow::addNewCommand(int cmdType)
{
    CmdWidget *itemWidget = CmdWidget::GetNewCommandWidget(CmdType(cmdType));

    QListWidgetItem *item = new QListWidgetItem();

    int row = ui->commandList->currentRow() + 1;
    if(ui->commandList->selectedItems().isEmpty())
        row = ui->commandList->count();

    addCmdListItem(item, itemWidget, row);

    selectRow(row);
    updateRowNumbers();
    setUnsavedChanges(true);
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

    //TODO
    ui->actionECopy->setEnabled(false);
    ui->actionECut->setEnabled(false);
    ui->actionEPaste->setEnabled(false);
}

void MainWindow::selectRow(int row)
{
    ui->commandList->clearSelection();
    ui->commandList->setCurrentRow(row);
}

void MainWindow::handleRowMoved(QModelIndex, int, int, QModelIndex, int)
{
    updateRowNumbers();
    setUnsavedChanges(true);
}
