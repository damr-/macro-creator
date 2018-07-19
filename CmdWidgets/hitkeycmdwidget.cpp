#include "hitkeycmdwidget.h"
#include "ui_hitkeycmdwidget.h"

HitKeyCmdWidget::HitKeyCmdWidget(QWidget *parent) :
    CmdWidget(parent),
    ui(new Ui::HitKeyCmdWidget)
{
    ui->setupUi(this);
}

HitKeyCmdWidget::~HitKeyCmdWidget()
{
    delete ui;
}

void HitKeyCmdWidget::CopyTo(CmdWidget *other)
{
    //TODO
}

QString HitKeyCmdWidget::GetCmdSafeString()
{
    return QString("HitKeyCmdWidget");
}

void HitKeyCmdWidget::keyPressEvent(QKeyEvent *e)
{
    /*
    if(!isListeningForKeyInput)
        return;

    if(e->key() == Qt::Key_Escape){
        ui->readKeyButton->setText("click me!");
        isListeningForKeyInput = false;
    }
    else{
        qDebug() << e->text();
        ui->readKeyButton->setText(e->text());
    }*/
}

/*
void HitKeyCmdWidget::readKeyButtonPressed()
{
    if(isListeningForKeyInput)
        return;

    isListeningForKeyInput = true;
    ui->readKeyButton->setText("make the sc!");
}*/
