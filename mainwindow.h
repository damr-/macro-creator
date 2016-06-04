#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <Windows.h>
#include <map>
#include <string>

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
    //For INSERT+DELETE
    void checkKey();

    //Menu Actions
    void closeThisAndOpenStartup();
    void saveProgram();

    //Slots for adding commands
    void addMoveCursorCommand();
    void addClickCommand();
    void addDragMouseCommand();
    void addScrollCommand();
    void addWriteTextCommand();
    void addShortcutCommand();
    void addOpenExeCommand();
    void addOpenPathCommand();
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

    //delete cmd backups
    QString delBackupText;
    int delBackupPos;

    //cmd list
    QList<QString> commandList;

    //quitbool
    bool unsavedChanges;

    void addCommand(QString commandtype, QStringList arguments);
    void loadCommandListFromFile();
    void fillCommandListWidget();
    void closeEvent(QCloseEvent *event);

    void executeCommand(QString cmd);
    void writeText(std::string text);
    void pressVK(std::string vk);

    std::map<std::string, BYTE> byteCodes;
    void defineByteCodes();
    void refreshWindowTitle();
    void setUnsavedChanges(bool newUnsavedChanges);
};

#endif // MAINWINDOW_H
