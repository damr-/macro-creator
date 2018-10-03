#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QPushButton>
#include <QScreen>
#include <QScrollBar>
#include <QSignalMapper>
#include <QStringList>
#include <QTextStream>
#include <QTimer>

#include "CmdWidgets/delaycmdwidget.h"
#include "CmdWidgets/gotocmdwidget.h"
#include "CmdWidgets/clickcmdwidget.h"
#include "CmdWidgets/setcursorposcmdwidget.h"
#include "CmdWidgets/dragcmdwidget.h"
#include "CmdWidgets/writetextcmdwidget.h"
#include "CmdWidgets/runexecmdwidget.h"
#include "CmdWidgets/presskeycmdwidget.h"
#include "CmdWidgets/scrollcmdwidget.h"
#include "CmdWidgets/presskeycmdwidget.h"

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
    connect(ui->actionEToggleLocked, &QAction::triggered, [=](){ toggleSelectionState(StateType::LOCKED); });
    connect(ui->actionEToggleDisabled, &QAction::triggered, [=](){ toggleSelectionState(StateType::DISABLED); });

    //Menu->Add Actions
    QSignalMapper *addActionsMapper = new QSignalMapper(this);
    connect(ui->action1Delay, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action2Goto, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action3Click, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action4CursorPos, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action5Drag, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action6Scroll, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action7PressKey, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action8Write, SIGNAL(triggered()), addActionsMapper, SLOT(map()));
    connect(ui->action9RunExe, SIGNAL(triggered()), addActionsMapper, SLOT(map()));

    addActionsMapper->setMapping(ui->action1Delay, 0);
    addActionsMapper->setMapping(ui->action2Goto, 1);
    addActionsMapper->setMapping(ui->action3Click, 2);
    addActionsMapper->setMapping(ui->action4CursorPos, 3);
    addActionsMapper->setMapping(ui->action5Drag, 4);
    addActionsMapper->setMapping(ui->action6Scroll, 5);
    addActionsMapper->setMapping(ui->action7PressKey, 6);
    addActionsMapper->setMapping(ui->action8Write, 7);
    addActionsMapper->setMapping(ui->action9RunExe, 8);
    connect(addActionsMapper, SIGNAL(mapped(int)), this, SLOT(addNewCmd(int)));

    //Menu->Run Actions
    connect(ui->actionRStart, SIGNAL(triggered(bool)), this, SLOT(tryRunMacro()));

    //TODO: Different Context menu depending on clicked QWidget
    //https://stackoverflow.com/questions/12937812/how-to-create-different-popup-context-menus-for-each-type-of-qtreewidgetitem#
    //TODO: Search bar in context menu when right clicking on command list widget:
    // http://blog.qt.io/blog/2012/12/07/qt-support-28-creating-a-toolbar-widget-that-is-displayed-with-the-context-menu/
    ui->cmdList->setContextMenuPolicy(Qt::CustomContextMenu);

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
    connect(ui->cmdList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    //

    //React to selection of commands
    connect(ui->cmdList, SIGNAL(itemSelectionChanged()), this, SLOT(handleSelectionChanged()));

    //React to drag&drop events
    connect(ui->cmdList->model(),
            SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this,
            SLOT(handleRowMoved(QModelIndex,int,int,QModelIndex,int)));

    //Disable actions by default
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

    if(GetAsyncKeyState(VK_F7))
    {
        tryRunMacro();
    }

    if(!GetAsyncKeyState(VK_F6) || isMacroRunning)
        return;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    LONG x = cursorPos.x;
    LONG y = cursorPos.y;

    QString posString = "(" + QString::number(x) + "," + QString::number(y) + ")";
    QString statusMsg;

    QList<QListWidgetItem *> selectedItems = ui->cmdList->selectedItems();
    if(selectedItems.length() == 0)
    {
        CmdWidget *widget = addNewCmd(int(CmdType::SETCURSORPOS));
        qobject_cast<SetCursorPosCmdWidget*>(widget)->SetCmdSettings(x, y, false);
        statusMsg = "Added new 'Cursor Position' command for " + posString;
    }    
    else
    {
        QListWidgetItem *item;
        foreach(item, selectedItems)
        {
            CmdWidget *widget = qobject_cast<CmdWidget*>(ui->cmdList->itemWidget(item));

            switch(widget->GetCmdType())
            {
                case CmdType::SETCURSORPOS:
                {
                    SetCursorPosCmdWidget* setCursorPosCmdWidget = qobject_cast<SetCursorPosCmdWidget*>(widget);
                        setCursorPosCmdWidget->SetCmdSettings(x, y, setCursorPosCmdWidget->GetAddClick());
                        statusMsg = "Updated 'Cursor position' to " + posString;
                        break;
                }
                case CmdType::DRAG:
                    qobject_cast<DragCmdWidget*>(widget)->SetCmdSettings(x, y);
                    statusMsg = "Updated 'Drag' to " + posString;
                    break;
                default:
                    //If exactly one item is selected and it's neither a SetCurPos nor a Drag command, add a new SetCurPos command
                    if(selectedItems.length() == 1)
                    {
                        CmdWidget *newWidget = addNewCmd(int(CmdType::SETCURSORPOS));
                        qobject_cast<SetCursorPosCmdWidget*>(newWidget)->SetCmdSettings(x, y, false);
                        statusMsg = "Added new 'Cursor Position' command for " + posString;
                    }
                    break;
            }
        }

        if(selectedItems.length() > 1)
            statusMsg = "Updated all selected 'Cursor Position' and 'Drag' to " + posString;
    }

    ui->statusBar->showMessage(statusMsg, 5000);
}

void MainWindow::setupBlankMacro()
{
    macroName = "unnamed";
    macroPath = QDir::currentPath();
    hasNoSafeFile = true;

    ui->actionFSaveAs->setEnabled(false);

    ui->cmdList->clear();
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

    if(hasNoSafeFile){
        pathPlusFileName = QFileDialog::getSaveFileName(this, tr("Save the macro as..."), macroPath, fileInfo);

        QString fileName = QFileInfo(pathPlusFileName).baseName();
        if(fileName.length() == 0)
        {
            ui->statusBar->showMessage("Saving aborted", 3000);
            return;
        }
        macroName = fileName;
        hasNoSafeFile = false;
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
    out.append(QString::number(settings->amount) + "|" +
               QString::number(settings->timeScale) + "\n");

    //Write commands
    for(int i = 0; i < ui->cmdList->count(); ++i)
    {
        QListWidgetItem* item = ui->cmdList->item(i);
        QWidget* itemWidget = ui->cmdList->itemWidget(item);
        QString cmdString = qobject_cast<CmdWidget*>(itemWidget)->GetCmdString();

        out.append(cmdString + "\n");
    }

    output << out;
    file.close();

    setUnsavedChanges(false);
    ui->statusBar->showMessage("Saved macro to " + pathPlusFileName, 3000);
}

void MainWindow::saveMacroAs()
{
    hasNoSafeFile = true;
    saveMacro();
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

    if(hasUnsavedChanges)
    {
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

    macroName = fileName;
    macroPath = QFileInfo(fullFilePath).absolutePath();

    if(!tryLoadCmdsFromFile(fullFilePath))
    {
        newMacro();
        setUnsavedChanges(false);
        return;
    }

    ui->statusBar->showMessage("Opened " + fullFilePath, 3000);
    hasNoSafeFile = false;
    ui->actionFSaveAs->setEnabled(true);
    setUnsavedChanges(false);
}

bool MainWindow::tryLoadCmdsFromFile(QString filename)
{
    ui->cmdList->clear();

    QFile file(filename);

    if(!file.exists() || !file.open(QFile::ReadOnly))
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
    settings->amount = options[DefaultDelaySettings::AmountIdx].toInt();
    settings->timeScale = options[DefaultDelaySettings::TimeScaleIdx].toInt();
    defaultDelayWidget->SetSettings(settings);

    //Load rest of commands
    QStringList cmdListStrings;
    QString line;
    do
    {
        line = input.readLine().trimmed();
        if(line.length() > 0)
            cmdListStrings.append(line);
    }while(!line.isNull());

    bool success;
    QList<QListWidgetItem *> newItems = fillCmdListWidget(cmdListStrings, 0, success);
    if(!success)
    {
        showMessage("Error", filename + " is corrupted.\nAborted opening process.", QMessageBox::Icon::Critical);
        ui->statusBar->showMessage("Opening aborted", 3000);
        file.close();
        return false;
    }
    file.close();
    return true;
}

QList<QListWidgetItem *> MainWindow::fillCmdListWidget(QStringList cmdListStrings, int startRow, bool &success)
{
    QList<QListWidgetItem *> newItems;

    for(int i = 0; i < cmdListStrings.size(); i++)
    {
        QStringList cmdStr = cmdListStrings.at(i).split("|");        
        CmdType cmdTypeIndex = CmdType(cmdStr[CmdWidget::CmdTypeIdx].toInt());
        CmdWidget *newWidget = CmdWidget::GetNewCmdWidget(cmdTypeIndex);

        if(cmdStr.length() != newWidget->GetCmdStringLen())
        {
            ui->cmdList->clear();
            success = false;
            return newItems;
        }

        newWidget->SetStates(cmdStr[CmdWidget::LockedStateIdx].toInt(), cmdStr[CmdWidget::DisabledStateIdx].toInt());
        newWidget->SetSettings(cmdStr);

        QListWidgetItem *newItem = new QListWidgetItem();
        addCmdListItem(newItem, newWidget, startRow + i);
        newItems.append(newItem);
    }

    updateRowNumbers();
    selectRow(ui->cmdList->row(newItems.last()));

    success = true;
    return newItems;
}

void MainWindow::setUnsavedChanges(bool newUnsavedChanges = true)
{
    hasUnsavedChanges = newUnsavedChanges;
    updateWindowTitle();
}

void MainWindow::updateWindowTitle()
{
    QString suffix = "Macro Creator";
    if(isMacroRunning)
        suffix = isMacroExecutionPaused ? "Paused" : "Running";

    setWindowTitle(macroName + (hasUnsavedChanges ? "*" : "") + " - " + suffix);
}

void MainWindow::tryRunMacro()
{
    if(isMacroRunning)
        return;

    if(ui->cmdList->count() == 0)
    {
        showMessage("Error", "Macro is empty", QMessageBox::Icon::Critical);
        return;
    }

    int result = AllCmdsValid();
    if(result != -1)
    {
        QString msg = QString("Possible error in row " + QString::number(result + 1));
        showMessage("Warning", msg, QMessageBox::Icon::Warning);
        ui->statusBar->showMessage(msg, 10000);

        selectRow(result);
        return;
    }

    SaveOrRestoreGotoTotalAmounts(true);
    selectRow(0);

    isMacroRunning = true;
    updateWindowTitle();

    setWindowOpacity(0.75);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
    show();

    ExecuteCmds();

    setWindowOpacity(1);
    setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint & ~Qt::WindowTransparentForInput & ~Qt::WindowDoesNotAcceptFocus);
    show();

    activateWindow();
    setFocus();
    raise();

    SaveOrRestoreGotoTotalAmounts(false);
    isMacroRunning = false;
    updateWindowTitle();
}

int MainWindow::AllCmdsValid()
{
    QListWidgetItem *item;

    for(int i = 0, total = ui->cmdList->count(); i < total; i++)
    {
        item = ui->cmdList->item(i);
        CmdWidget *widget = qobject_cast<CmdWidget *>(ui->cmdList->itemWidget(item));
        if(!widget->IsValidCmd())
            return i;
    }
    return -1;
}

void MainWindow::ExecuteCmds()
{
    isMacroExecutionPaused = false;
    DefaultDelaySettings *defaultDelaySettings = defaultDelayWidget->GetSettings();

    for(int i = 0, total = ui->cmdList->count(); i < total; i++)
    {
        //Check for user input
        if(GetAsyncKeyState(VK_F7) && isMacroExecutionPaused)
        {            
            isMacroExecutionPaused = false;
            updateWindowTitle();
        }
        if(GetAsyncKeyState(VK_F8) && !isMacroExecutionPaused)
        {
            isMacroExecutionPaused = true;
            updateWindowTitle();
        }
        if(GetAsyncKeyState(VK_F9))
            break;
        //

        if(isMacroExecutionPaused)
        {
            i--;
            qApp->processEvents();
            continue;
        }

        selectRow(i);
        qApp->processEvents();

        QListWidgetItem* item = ui->cmdList->item(i);
        CmdWidget *widget = qobject_cast<CmdWidget*>(ui->cmdList->itemWidget(item));

        if(!widget->isEnabled())
        {
            continue;
        }
        else if(widget->GetCmdType() == CmdType::GOTO)
        {
            GotoCmdWidget *gotoWidget = qobject_cast<GotoCmdWidget *>(widget);
            int amount = gotoWidget->GetAmount();
            if(amount != 0)
            {
                int targetRow = gotoWidget->GetTargetRow();
                i = targetRow - 2; // -1 for 0 based index and -1 for the loop's i++

                if(amount > 0)
                    gotoWidget->SetCmdSettings(targetRow, amount - 1);
            }
        }
        else
        {
            Commands::ExecuteCmd(widget->GetCmdString());
        }

        Sleep(DWORD(defaultDelaySettings->amount) * (defaultDelaySettings->timeScale == 0 ? 1000 : 1));
    }
}

void MainWindow::SaveOrRestoreGotoTotalAmounts(bool save)
{
    for(int i = 0, total = ui->cmdList->count(); i < total; i++)
    {
        QListWidgetItem* item = ui->cmdList->item(i);
        CmdWidget *widget = qobject_cast<CmdWidget*>(ui->cmdList->itemWidget(item));

        if(widget->GetCmdType() == CmdType::GOTO)
        {
            GotoCmdWidget *gotoWidget = qobject_cast<GotoCmdWidget *>(widget);
            if(save)
                gotoWidget->TotalAmount = gotoWidget->GetAmount();
            else
                gotoWidget->SetCmdSettings(gotoWidget->GetTargetRow(), gotoWidget->TotalAmount);
        }
    }
}

QList<QListWidgetItem *> MainWindow::GetSortedSelectedItems()
{
    QList<QListWidgetItem *> selectedItems = ui->cmdList->selectedItems();

    std::sort(selectedItems.begin(), selectedItems.end(),
              [this](QListWidgetItem *a, QListWidgetItem *b){
                CmdWidget *aw = qobject_cast<CmdWidget *>(ui->cmdList->itemWidget(a));
                CmdWidget *bw = qobject_cast<CmdWidget *>(ui->cmdList->itemWidget(b));
                return aw->GetRowNumber() < bw->GetRowNumber();
    });
    return selectedItems;
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->cmdList->mapToGlobal(pos);
    contextMenu.exec(globalPos);
}

void MainWindow::copySelected()
{
    if(ui->cmdList->selectedItems().size() == 0)
        return;

    QList<QListWidgetItem *> selectedItems = GetSortedSelectedItems();

    QString clipBoardText = "";
    for (int i = 0, total = selectedItems.size(); i < total; i++)
    {
        QListWidgetItem *item = selectedItems.at(i);
        CmdWidget *selectedItemWidget = qobject_cast<CmdWidget*>(ui->cmdList->itemWidget(item));
        QString cmdStr = selectedItemWidget->GetCmdString();
        clipBoardText.append(cmdStr + ";");
    }

    //Remove trailing ';'
    clipBoardText.chop(1);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(clipBoardText);
}

void MainWindow::cutSelected()
{
    copySelected();
    deleteSelected();
}

void MainWindow::pasteClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->text().isEmpty())
        return;

    QStringList cmds = clipboard->text().split(';');

    bool success = true;

    QString cmd;
    foreach(cmd, cmds)
    {
        if(!cmd.contains('|'))
            success = false;
    }

    if(!success)
        return;

    int row = ui->cmdList->currentRow() + 1;
    if(ui->cmdList->selectedItems().isEmpty())
        row = ui->cmdList->count();

    QList<QListWidgetItem *> newItems = fillCmdListWidget(cmds, row, success);

    if(!success)
        return;

    selectRow(ui->cmdList->row(newItems.at(0)));

    QListWidgetItem *i;
    foreach(i, newItems)
        i->setSelected(true);

    updateRowNumbers();
    setUnsavedChanges(true);
}

void MainWindow::deleteSelected()
{
    while(ui->cmdList->selectedItems().length() > 0)
    {
        QListWidgetItem *item = ui->cmdList->selectedItems().at(0);
        delete item;
    }
    setUnsavedChanges(true);
    updateRowNumbers();
}

void MainWindow::duplicateSelected()
{
    if(ui->cmdList->selectedItems().size() == 0)
        return;

    QList<QListWidgetItem *> selectedItems = GetSortedSelectedItems();

    QList<QListWidgetItem *> newItems;
    for (int i = 0, total = selectedItems.size(); i < total; ++i)
    {
        QListWidgetItem *item = selectedItems.at(i);

        CmdWidget *selectedItemWidget = qobject_cast<CmdWidget*>(ui->cmdList->itemWidget(item));
        CmdType selectedItemCmdType = selectedItemWidget->GetCmdType();

        //create new widget
        CmdWidget *newWidget = CmdWidget::GetNewCmdWidget(selectedItemCmdType);

        //copy widget values to new one
        selectedItemWidget->CopyTo(newWidget);

        QListWidgetItem *newItem = new QListWidgetItem();
        addCmdListItem(newItem, newWidget, ui->cmdList->row(item) + 1);
        newItems.append(newItem);
    }

    selectRow(ui->cmdList->row(newItems.at(0)));

    QListWidgetItem *i;
    foreach(i, newItems)
        i->setSelected(true);

    updateRowNumbers();
    setUnsavedChanges(true);
}

void MainWindow::toggleSelectionState(StateType type)
{
    QList<QListWidgetItem *> selectedItems = GetSortedSelectedItems();
    for (int i = 0, total = selectedItems.size(); i < total; i++)
    {
        QListWidgetItem *item = selectedItems.at(i);
        CmdWidget *selectedItemWidget = qobject_cast<CmdWidget*>(ui->cmdList->itemWidget(item));

        switch(type)
        {
            case StateType::LOCKED:
                selectedItemWidget->ToggleLocked();
                break;
            case StateType::DISABLED:
                selectedItemWidget->ToggleEnabled();
                break;
        }
    }
}

void MainWindow::updateRowNumbers()
{
    QListWidgetItem *item;
    for(int i = 0, total = ui->cmdList->count(); i < total; i++)
    {
        item = ui->cmdList->item(i);
        CmdWidget *widget = qobject_cast<CmdWidget *>(ui->cmdList->itemWidget(item));
        widget->SetRowNumber(i + 1);
    }
}

CmdWidget* MainWindow::addNewCmd(int cmdType)
{
    CmdWidget *itemWidget = CmdWidget::GetNewCmdWidget(CmdType(cmdType));

    QListWidgetItem *item = new QListWidgetItem();

    int row = ui->cmdList->currentRow() + 1;
    if(ui->cmdList->selectedItems().isEmpty())
        row = ui->cmdList->count();

    addCmdListItem(item, itemWidget, row);

    selectRow(row);
    updateRowNumbers();
    setUnsavedChanges(true);

    //TODO fix issue when adding commands and then selecting multiple cmds with 'Shift': wrong items get selected

    return itemWidget;
}

void MainWindow::addCmdListItem(QListWidgetItem *item, CmdWidget *itemWidget, int row)
{
    ui->cmdList->insertItem(row, item);
    ui->cmdList->setItemWidget(item, itemWidget);
    item->setSizeHint(QSize(0, itemWidget->height()));
    connect(itemWidget, SIGNAL(cmdChanged(CmdWidget*)), this, SLOT(handleCmdSettingChanged(CmdWidget*)));

    if(itemWidget->GetCmdType() == CmdType::SETCURSORPOS)
    {
        SetCursorPosCmdWidget *w = qobject_cast<SetCursorPosCmdWidget*>(itemWidget);
        connect(w, SIGNAL(showPosHint(bool, int, int)), this, SLOT(showPosHint(bool, int, int)));
    }
    else if(itemWidget->GetCmdType() == CmdType::DRAG)
    {
        DragCmdWidget *w = qobject_cast<DragCmdWidget*>(itemWidget);
        connect(w, SIGNAL(showPosHint(bool, int, int)), this, SLOT(showPosHint(bool, int, int)));
    }
}

void MainWindow::handleCmdSettingChanged(CmdWidget *widget)
{
    if(isMacroRunning)
        return;

    setUnsavedChanges(true);

    if(widget->GetCmdType() == CmdType::GOTO)
    {
        qobject_cast<GotoCmdWidget *>(widget)->ValidateRowNumber(ui->cmdList->count());
    }
}

void MainWindow::showPosHint(bool visible, int x, int y)
{
    if(!visible && posHint != nullptr)
    {
        posHint->close();
    }

    if(visible)
    {
        posHint = new PosHint();
        posHint->SetPosition(x, y);
        posHint->show();
        activateWindow();
    }
}

void MainWindow::handleSelectionChanged()
{
    bool itemSelected = ui->cmdList->selectedItems().count() > 0;

    ui->actionEDuplicate->setEnabled(itemSelected);
    ui->actionEDelete->setEnabled(itemSelected);
    ui->actionECopy->setEnabled(itemSelected);
    ui->actionECut->setEnabled(itemSelected);

    QClipboard *clipboard = QApplication::clipboard();
    ui->actionEPaste->setEnabled(!clipboard->text().isEmpty());

    ui->actionEToggleDisabled->setEnabled(itemSelected);
    ui->actionEToggleLocked->setEnabled(itemSelected);
}

void MainWindow::selectRow(int row)
{
    ui->cmdList->clearSelection();
    ui->cmdList->setCurrentRow(row);
}

void MainWindow::handleRowMoved(QModelIndex, int, int, QModelIndex, int)
{
    updateRowNumbers();
    setUnsavedChanges(true);
}
