#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    ui->lineEditKvo->setValidator(new QIntValidator(0,1000000000,this));
    ui->lineEditKvo->setText("331800");

    modelNorm = new ModelNorm(this);
    ui->tableViewNorm->setModel(modelNorm);
    ui->tableViewNorm->setColumnWidth(0,150);
    for (int i=1; i<ui->tableViewNorm->model()->columnCount(); i++){
        ui->tableViewNorm->setColumnWidth(i,100);
    }

    connect(ui->dateEditBeg,SIGNAL(dateChanged(QDate)),this,SLOT(updNorm()));
    connect(ui->dateEditEnd,SIGNAL(dateChanged(QDate)),this,SLOT(updNorm()));
    connect(ui->pushButtonPlan,SIGNAL(clicked(bool)),this,SLOT(repNorm()));
    connect(ui->pushButtonFact,SIGNAL(clicked(bool)),this,SLOT(repFact()));

    updNorm();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::updNorm()
{
    modelNorm->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
}

void MainWidget::repNorm()
{
    CubeWidget *w = new CubeWidget(30);
    w->setRange(ui->dateEditBeg->date(),ui->dateEditEnd->date(),false);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWidget::repFact()
{
    CubeWidget *w = new CubeWidget(30);
    w->setWindowTitle("Фактический расход эл. энергии на производство проволоки, кВтч");
    w->setSum(ui->lineEditKvo->text().toInt());
    w->setRange(ui->dateEditBeg->date(),ui->dateEditEnd->date());
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}
