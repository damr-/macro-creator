#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Windows.h>

#include <QAction>
#include <QList>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

#include "commandlistitem.h"
#include "commandwidget.h"

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

    //Slots for adding commands
    void addMoveCursorCommand();
    void addClickCommand();
    void addDragMouseCommand();
    void addScrollCommand();
    void addWriteTextCommand();
    void addShortcutCommand();
    void chooseExe();
    void addOpenExeCommand();
    void addWaitCommand();
    void addKillProcessCommand();

    //ListWidget modification
    void deleteCommand();
    void deleteUndo();
    void refreshCommandListControls();
    void moveItem(int direction);

    //Shortcut Finemaker
    void letterBoxEdited();
    void keyBoxEdited();

    //Bot Actions
    void botStart();

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
    void addNewCommand();
    void deleteSelected();
    void duplicateSelected();
    void handleSelectionChanged();
    void handleItemChanged(QModelIndex, int, int, QModelIndex, int);
    void addItem(CommandListItem *item, CommandWidget *itemWidget, int row);
    void unselectAll();

    void commandSelectionChanged();

private:
    Ui::MainWindow *ui;

    QString programName;
    QString programPath;

    QString delBackupText;
    int delBackupPos;

    QList<QString> commandList;

    bool unsavedProgram;

    bool unsavedChanges;

    //contect menu
    QMenu contextMenu;

    bool isListeningForKeyInput;

    void addCommand(QString commandtype, QStringList arguments);
    void loadCommandListFromFile(QString pathPlusFilename);
    void fillCommandListWidget();
    void closeEvent(QCloseEvent *event);

    void executeCommand(QString cmd);

    void refreshWindowTitle();
    void setUnsavedChanges(bool newUnsavedChanges);

    static QString getFullFilePath(QString filePath, QString fileName) { return filePath + "/" + fileName + ".myprog"; }

    enum UnsavedChangesMessageResult {
        Save = 0,
        DontSave = 1,
        Cancel = 2
    };

    QMessageBox *showUnsavedChangesWarning(UnsavedChangesMessageResult &result);
};

#endif // MAINWINDOW_H
