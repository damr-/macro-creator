#ifndef PROGWINDOW_H
#define PROGWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QMenu>

#include "commandwidget.h"

namespace Ui {
class ProgWindow;
}

class ProgWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProgWindow(QWidget *parent = 0);
    ~ProgWindow();
	static const QString settingsFileName;

private slots:
	//main menu actions
    void newOpenProgram();
	void showOptions();
	void exitProgram();

	void showContextMenu(const QPoint&);

	//(menu) item actions
	void addNew();
	void deleteSelected();
	void duplicateSelected();

	void handleSelectionChanged();

private:
    Ui::ProgWindow *ui;

    //void closeEvent(QCloseEvent *event);
    void loadFromFile();
    void fillListWidget();

	void addItem(QListWidgetItem *item, CommandWidget *itemWidget, int row);
	void unselectAll();

    QString settingsFile;
    QList<QString> commandList;
	QMenu contextMenu;
	bool unsavedChanges;
};

#endif // PROGWINDOW_H
