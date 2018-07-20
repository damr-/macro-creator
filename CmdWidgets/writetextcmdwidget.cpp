#include "writetextcmdwidget.h"
#include "ui_writetextcmdwidget.h"

WriteTextCmdWidget::WriteTextCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::WriteTextCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::WRITETEXT;

    //connect(ui->textEdit, SIGNAL(textChanged(QString)), this, SLOT("emit change signal"()));
}

WriteTextCmdWidget::~WriteTextCmdWidget()
{
    delete ui;
}

void WriteTextCmdWidget::CopyTo(CmdWidget *other)
{
    qobject_cast<WriteTextCmdWidget*>(other)->SetText(GetText());
}

QString WriteTextCmdWidget::GetCmdSafeString()
{
    return QString("WriteTextCmdWidget");
}

bool WriteTextCmdWidget::IsValidCmd()
{
    return !ui->textEdit->text().isEmpty();
}

QString WriteTextCmdWidget::GetText()
{
    return ui->textEdit->text();
}

void WriteTextCmdWidget::SetText(QString text)
{
    ui->textEdit->setText(text);
}
