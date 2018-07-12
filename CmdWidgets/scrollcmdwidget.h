#ifndef SCROLLCMDWIDGET_H
#define SCROLLCMDWIDGET_H

#include <QWidget>

namespace Ui {
class ScrollCmdWidget;
}

class ScrollCmdWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollCmdWidget(QWidget *parent = 0);
    ~ScrollCmdWidget();

private:
    Ui::ScrollCmdWidget *ui;
};

#endif // SCROLLCMDWIDGET_H
