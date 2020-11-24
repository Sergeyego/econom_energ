#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "modelnorm.h"
#include "olap/cubewidget.h"
#include "formfact.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    ModelNorm *modelNorm;

private slots:
    void updNorm();
    void repNorm();
    void repFact();
    void repFactExt();
};

#endif // MAINWIDGET_H
