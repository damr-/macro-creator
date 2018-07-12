#include "scrollcmdwidget.h"
#include "ui_scrollcmdwidget.h"

ScrollCmdWidget::ScrollCmdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScrollCmdWidget)
{
    ui->setupUi(this);
}

ScrollCmdWidget::~ScrollCmdWidget()
{
    delete ui;
}
