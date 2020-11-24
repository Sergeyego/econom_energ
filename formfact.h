#ifndef FORMFACT_H
#define FORMFACT_H

#include <QWidget>
#include "modelnorm.h"

namespace Ui {
class FormFact;
}

class FormFact : public QWidget
{
    Q_OBJECT

public:
    explicit FormFact(QDate beg, QDate end, double sum, QWidget *parent = 0);
    ~FormFact();

private:
    Ui::FormFact *ui;
    ModelFact *modelFact;

private slots:
    void save();
};

#endif // FORMFACT_H
