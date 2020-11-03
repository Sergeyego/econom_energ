#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "modelnorm.h"
#include "olap/cubewidget.h"

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
};

#endif // MAINWIDGET_H
