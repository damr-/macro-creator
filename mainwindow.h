#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>

#include <QAction>
#include <QList>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

#include "commands.h"
#include "defaultdelaywidget.h"
#include "poshint.h"
#include "CmdWidgets/cmdwidget.h"

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
        void checkUserKeyInput();

        //File Actions
        void setupBlankMacro();
        void newMacro();
        void saveMacro();
        void saveMacroAs();
        void openMacro();

        void tryRunMacro();

        void showContextMenu(const QPoint&);

        //Adding commands
        CmdWidget *addNewCmd(int cmdType);
        void addCmdListItem(QListWidgetItem *item, CmdWidget *itemWidget, int row);

        void setUnsavedChanges(bool newUnsavedChanges);
        void updateWindowTitle();

        //Commandlist editing
        void copySelected();
        void cutSelected();
        void pasteClipboard();
        void deleteSelected();
        void duplicateSelected();

        void toggleSelectionState(StateType type);

        void updateRowNumbers();

        void handleRowMoved(QModelIndex, int, int, QModelIndex, int);
        void handleSelectionChanged();
        void selectRow(int row);

        void handleCmdSettingChanged(CmdWidget *widget);
        void showPosHint(bool visible, int x, int y);

    private:
        Ui::MainWindow *ui;
        DefaultDelayWidget *defaultDelayWidget;
        PosHint *posHint = new PosHint();
        QMenu contextMenu;
        int defX, defY, defW, defH;

        //Running Macro
        bool isMacroRunning = false;
        bool isMacroExecutionPaused;
        int AllCmdsValid();
        void ExecuteCmds();
        void SaveOrRestoreGotoTotalAmounts(bool save);

        QList<QListWidgetItem *> GetSortedSelectedItems();

        void showMessage(QString title, QString message, QMessageBox::Icon type);

        //Storing data
        QString macroName;
        QString macroPath;
        QString fileExtension = "mccr";
        QString fileInfo = "Macro Creator Files (*." + fileExtension + ")";
        QString getFullFilePath(QString filePath, QString fileName) { return filePath + "/" + fileName + "." + this->fileExtension; }

        bool hasNoSafeFile;
        bool hasUnsavedChanges;

        void closeEvent(QCloseEvent *event);

        enum UnsavedChangesMessageResult {
            Save = 0,
            DontSave = 1,
            Cancel = 2
        };
        QMessageBox *showUnsavedChangesWarning(UnsavedChangesMessageResult &result);

        bool tryLoadCmdsFromFile(QString pathPlusFilename);
        QList<QListWidgetItem *> fillCmdListWidget(QStringList cmdListStrings, int startRow, bool &success);
};

#endif // MAINWINDOW_H
