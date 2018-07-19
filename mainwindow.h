#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Windows.h>

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //For catching the mouse position
    void checkKey();

    //Menu Actions
    void newMacro();
    void saveMacro();
    void saveMacroAs();
    void openMacro();

    //Bot Actions
    void tryRunMacro();

    //NEW METHODS
    void showContextMenu(const QPoint&);
    void addNewCommand(int cmdIndex);
    void handleItemChanged(QModelIndex, int, int, QModelIndex, int);
    void addItem(QListWidgetItem *item, CmdWidget *itemWidget, int row);

    //List editing
    void copySelected();
    void cutSelected();
    void pasteClipboard();
    void deleteSelected();
    void duplicateSelected();

    void handleSelectionChanged();
    void unselectAll();

    void handleCommandSettingChanged();
    void setUnsavedChanges();

private:
    Ui::MainWindow *ui;

//    QString delBackupText;
//    int delBackupPos;

    bool isUnsavedMacro;
    bool hasUnsavedChanges;
    bool isMacroRunning;

    QMenu contextMenu;

    QString macroName;
    QString macroPath;
    QString fileExtension = "pmac";
    QString fileInfo = "Personal Macro Files (*." + fileExtension + ")";

    DefaultDelayWidget *defaultDelayWidget;

    //void addCommand(QString commandtype, QStringList arguments);
    void loadCommandListFromFile(QString pathPlusFilename);
    void fillCommandListWidget(QStringList commandListStrings);

    void closeEvent(QCloseEvent *event);
    void executeCommand(QString cmd);

    void refreshWindowTitle();
    void setUnsavedChanges(bool newUnsavedChanges);

    QString getCommandString(int commandListIndex);

    enum UnsavedChangesMessageResult {
        Save = 0,
        DontSave = 1,
        Cancel = 2
    };

    QMessageBox *showUnsavedChangesWarning(UnsavedChangesMessageResult &result);

    QString getFullFilePath(QString filePath, QString fileName) { return filePath + "/" + fileName + "." + this->fileExtension; }

    void ExecuteCommands();
};

#endif // MAINWINDOW_H
