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
    connect(ui->exeName, SIGNAL(textChanged(QString)), this, SLOT(emitCmdChangedSignal()));
}

RunExeCmdWidget::~RunExeCmdWidget()
{
    delete ui;
}

void RunExeCmdWidget::CopyTo(CmdWidget *other)
{
    RunExeCmdWidget *widget = qobject_cast<RunExeCmdWidget*>(other);
    widget->SetCmdSettings(GetFilePathHex());
    CmdWidget::CopyTo(widget);
}

QString RunExeCmdWidget::GetCmdString()
{
    return CmdWidget::GetCmdString() + "|" + GetFilePathHex();
}

void RunExeCmdWidget::ToggleLocked()
{
    CmdWidget::ToggleLocked();
    ui->exeName->setEnabled(!isLocked);
    ui->selectFileButton->setEnabled(!isLocked);
}

void RunExeCmdWidget::SetSettings(QStringList settings)
{
    QString path = settings[PathIdx];
    path = path.length() == 0 ? "" : path;
    SetCmdSettings(path);
}

bool RunExeCmdWidget::IsValidCmd()
{
    currentFilePath = currentFilePath.trimmed();
    return !currentFilePath.isEmpty() && currentFilePath != "";
}

QString RunExeCmdWidget::GetFilePathHex()
{
    return currentFilePath.toUtf8().toHex();
}

void RunExeCmdWidget::SetCmdSettings(QString filePath)
{
    currentFilePath = CmdWidget::FromHex(filePath);
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
