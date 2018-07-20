#include "runexecmdwidget.h"
#include "ui_runexecmdwidget.h"

#include <QFileDialog>

RunExeCmdWidget::RunExeCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::RunExeCmdWidget)
{
    ui->setupUi(this);

    cmdType = CmdType::RUNEXE;

    connect(ui->selectFileButton, SIGNAL(clicked()), this, SLOT(chooseExe()));

    connect(ui->exeName, SIGNAL(textChanged(QString)), this, SLOT(emitCommandChangedSignal()));
}

RunExeCmdWidget::~RunExeCmdWidget()
{
    delete ui;
}

void RunExeCmdWidget::CopyTo(CmdWidget *other)
{
    qobject_cast<RunExeCmdWidget*>(other)->SetFilePath(GetFilePath());
}

QString RunExeCmdWidget::GetCmdSafeString()
{
    return QString("RunExeCmdWidget");
}

bool RunExeCmdWidget::IsValidCmd()
{
    currentFilePath = currentFilePath.trimmed();
    return !currentFilePath.isEmpty() && currentFilePath != "";
}

QString RunExeCmdWidget::GetFilePath()
{
    return currentFilePath;
}

void RunExeCmdWidget::SetFilePath(QString filePath)
{
    currentFilePath = filePath;
    ui->exeName->setText(currentFilePath);
}

void RunExeCmdWidget::chooseExe()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose .exe"), QDir::currentPath(), tr("Executable Files (*.exe)"));
    if(filePath.length() <= 1 || !filePath.endsWith(".exe"))
        filePath = "";
    ui->exeName->setText(filePath);
    currentFilePath = filePath;
}
