#include "poshint.h"
#include "ui_poshint.h"

PosHint::PosHint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PosHint)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint |  Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
//    setAttribute(Qt::WA_DeleteOnClose);

    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);
}

PosHint::~PosHint()
{
    delete ui;
}

void PosHint::SetPosition(int x, int y)
{
    move(x - width()/2, y - height()/2);
}
