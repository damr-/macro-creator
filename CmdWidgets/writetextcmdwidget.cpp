#include "writetextcmdwidget.h"
#include "ui_writetextcmdwidget.h"

WriteTextCmdWidget::WriteTextCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::WriteTextCmdWidget)
{
    ui->setupUi(this);
}

WriteTextCmdWidget::~WriteTextCmdWidget()
{
    delete ui;
}

void WriteTextCmdWidget::CopyTo(CmdWidget *other)
{
    //TODO
}

QString WriteTextCmdWidget::GetCmdSafeString()
{
    return QString("WriteTextCmdWidget");
}
