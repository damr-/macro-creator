#include "writetextcmdwidget.h"
#include "ui_writetextcmdwidget.h"

WriteTextCmdWidget::WriteTextCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::WriteTextCmdWidget)
{
    ui->setupUi(this);

    //connect(ui->textEdit, SIGNAL(textChanged(QString)), this, SLOT(updateWriteTextCount()));
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

/*
void WriteTextCmdWidget::updateWriteTextCount()
{
    ui->count->setText(QString::number(ui->textEdit->text().length()) + " / " + QString::number(ui->textEdit->maxLength()));
}*/
