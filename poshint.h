#ifndef POSHINT_H
#define POSHINT_H

#include <QDialog>

namespace Ui {
class PosHint;
}

class PosHint : public QDialog
{
    Q_OBJECT

public:
    explicit PosHint(QWidget *parent = nullptr);
    ~PosHint();
    void SetPosition(int x, int y);

private:
    Ui::PosHint *ui;
};

#endif // POSHINT_H
