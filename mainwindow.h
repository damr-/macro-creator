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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* e);

private slots:
    //For catching the mouse position
    void checkKey();

    //Menu Actions
    void newProgram();
    void saveProgram();
    void saveProgramAs();
    void openProgram();
    void showOptionsDialog();

    //Slots for adding commands
//    void addMoveCursorCommand();
//    void addClickCommand();
//    void addDragMouseCommand();
//    void addScrollCommand();
//    void addWriteTextCommand();
//    void addShortcutCommand();
//    void chooseExe();
//    void addOpenExeCommand();
//    void addWaitCommand();
//    void addKillProcessCommand();

    //ListWidget modification
//    void deleteCommand();
//    void deleteUndo();
//    void refreshCommandListControls();

    //Shortcut Finemaker
//    void letterBoxEdited();
//    void keyBoxEdited();

    //Bot Actions
    void tryStartProgram();

    //Loop
    void loopTypeChanged();
    void loopFromChanged();
    void loopToChanged();

    //LineEdit
    void updateWriteTextCount();

    //DEFAULT DELAY
    void defaultDelayToggled();

    void optionsChanged(int dummy);

    void readKeyButtonPressed();

    //NEW METHODS
    void showContextMenu(const QPoint&);
    void addNewCommand(int commandIndex);
    void deleteSelected();
    void duplicateSelected();
    void handleSelectionChanged();
    void handleItemChanged(QModelIndex, int, int, QModelIndex, int);
    void addItem(QListWidgetItem *item, CmdWidget *itemWidget, int row);
    void unselectAll();

    void commandSelectionChanged();

    void handleCommandSettingChanged();

private:
    Ui::MainWindow *ui;

    QString programName;
    QString programPath;

//    QString delBackupText;
//    int delBackupPos;

    bool isUnsavedProgram;
    bool hasUnsavedChanges;

    bool isListeningForKeyInput;
    bool isProgramRunning;

    QMenu contextMenu;

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

    static QString getFullFilePath(QString filePath, QString fileName) { return filePath + "/" + fileName + ".myprog"; }
};

#endif // MAINWINDOW_H
