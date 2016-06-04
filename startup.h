#ifndef STARTUP_H
#define STARTUP_H

#include <QWidget>

namespace Ui {
class startup;
}

class startup : public QWidget
{
    Q_OBJECT
    
public:
    static QString progListFileName;
    static QString progListDirectory;
    static QString startup::getCurrentProgName() { return currentProgName; }
    explicit startup(QWidget *parent = 0);
    ~startup();

public slots:
    void newClicked();
    void openClicked();
    void deleteClicked();

private:
    Ui::startup *ui;
    void cleanProgramList();
    void loadPrograms();
    void refreshProgramsDisplay();
    static QString currentProgName;
};

#endif // STARTUP_H
