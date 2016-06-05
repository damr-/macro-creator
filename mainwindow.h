#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <Windows.h>

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
    void newProgram();
    void saveProgram();    

    //Slots for adding commands
    void addMoveCursorCommand();
    void addClickCommand();
    void addDragMouseCommand();
    void addScrollCommand();
    void addWriteTextCommand();
    void addShortcutCommand();
    void chooseExe();
    void addOpenExeCommand();
    void addSleepCommand();
    void addKillProcessCommand();

    //ListWidget modification
    void deleteCommand();
    void deleteUndo();
    void moveItem();
    void moveIt(int dir);

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

private:
    Ui::MainWindow *ui;

    QString progName;

    QString delBackupText;
    int delBackupPos;

    QList<QString> commandList;

    bool unsavedChanges;

    void addCommand(QString commandtype, QStringList arguments);
    void loadCommandListFromFile();
    void fillCommandListWidget();
    void closeEvent(QCloseEvent *event);

    void executeCommand(QString cmd);

    void refreshWindowTitle();
    void setUnsavedChanges(bool newUnsavedChanges);
};

#endif // MAINWINDOW_H
