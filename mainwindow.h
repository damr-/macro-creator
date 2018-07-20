#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>

#include <QAction>
#include <QList>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

#include "CmdWidgets/cmdwidget.h"
#include "defaultdelaywidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //For catching the mouse position
    void checkUserKeyInput();

    //File Actions
    void newMacro();
    void saveMacro();
    void saveMacroAs();
    void openMacro();

    //Macro Actions
    void tryRunMacro();

    //NEW METHODS
    void showContextMenu(const QPoint&);
    void handleRowMoved(QModelIndex, int, int, QModelIndex, int);
    CmdWidget *addNewCommand(int cmdIndex);
    void addItem(QListWidgetItem *item, CmdWidget *itemWidget, int row);

    //List editing
    void copySelected();
    void cutSelected();
    void pasteClipboard();
    void deleteSelected();
    void duplicateSelected();
    void updateRowNumbers();

    void handleSelectionChanged();
    void unselectAll();

    void handleCommandSettingChanged();
    void setUnsavedChanges();

private:
    Ui::MainWindow *ui;
    DefaultDelayWidget *defaultDelayWidget;
    QMenu contextMenu;

    //Running Macro
    bool isMacroRunning;
    int AllCommandsValid();
    void ExecuteCommands();
    void ExecuteCommand(QString cmd);

    //Unsafed changes
    enum UnsavedChangesMessageResult {
        Save = 0,
        DontSave = 1,
        Cancel = 2
    };

    bool isUnsavedMacro;
    bool hasUnsavedChanges;
    void setUnsavedChanges(bool newUnsavedChanges);
    void closeEvent(QCloseEvent *event);
    void RefreshWindowTitle();
    QMessageBox *showUnsavedChangesWarning(UnsavedChangesMessageResult &result);

    //Storing data
    QString macroName;
    QString macroPath;
    QString fileExtension = "pmac";
    QString fileInfo = "Personal Macro Files (*." + fileExtension + ")";
    QString getFullFilePath(QString filePath, QString fileName) { return filePath + "/" + fileName + "." + this->fileExtension; }
    //void addCommand(QString commandtype, QStringList arguments);
    void loadCommandListFromFile(QString pathPlusFilename);
    //void fillCommandListWidget(QStringList commandListStrings);
    QString getCommandString(int commandListIndex);
};

#endif // MAINWINDOW_H
