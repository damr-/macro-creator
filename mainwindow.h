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

#define DELAY_OPTIONS_LEN 3
#define WINDOW_OPTIONS_LEN 5

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
    //For catching the mouse position and the key to run the macro
    void checkUserKeyInput();

    //File Actions
    void setupBlankMacro();
    void newMacro();
    void saveMacro();
    void saveMacroAs();
    void openMacro();

    //Macro Actions
    void tryRunMacro();

    //NEW METHODS
    void showContextMenu(const QPoint&);
    void handleRowMoved(QModelIndex, int, int, QModelIndex, int);
    CmdWidget *addNewCommand(int cmdType);
    void addCmdListItem(QListWidgetItem *item, CmdWidget *itemWidget, int row);
    void setUnsavedChanges(bool newUnsavedChanges);

    //Commandlist editing
    void copySelected();
    void cutSelected();
    void pasteClipboard();
    void deleteSelected();
    void duplicateSelected();
    void updateRowNumbers();

    void handleSelectionChanged();
    void selectRow(int row);

    void handleCommandSettingChanged();

private:
    Ui::MainWindow *ui;
    DefaultDelayWidget *defaultDelayWidget;
    QMenu contextMenu;
    int defX, defY, defW, defH;

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

    bool isMacroWithoutSaveFile;
    bool hasUnsavedChanges;
    void closeEvent(QCloseEvent *event);
    QMessageBox *showUnsavedChangesWarning(UnsavedChangesMessageResult &result);
    void showMessage(QString title, QString message, QMessageBox::Icon type);

    //Storing data
    QString macroName;
    QString macroPath;
    QString fileExtension = "pmac";
    QString fileInfo = "Personal Macro Files (*." + fileExtension + ")";
    QString getFullFilePath(QString filePath, QString fileName) { return filePath + "/" + fileName + "." + this->fileExtension; }

    bool tryLoadCmdsFromFile(QString pathPlusFilename);
    QList<QListWidgetItem *> fillCommandListWidget(QStringList cmdListStrings, int startRow, bool &success);
    QString getCmdString(int cmdListIndex);
};

#endif // MAINWINDOW_H
