#include "formfact.h"
#include "ui_formfact.h"

FormFact::FormFact(QDate beg, QDate end, double sum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFact)
{
    ui->setupUi(this);
    modelFact = new ModelFact(this);
    modelFact->refresh(beg,end,sum);
    ui->label->setText(QString::fromUtf8("Расход электрической энергии на производство проволоки, кВтч с %1 по %2").arg(beg.toString("dd.MM.yy")).arg(end.toString("dd.MM.yy")));
    ui->tableView->setModel(modelFact);
    ui->tableView->resizeToContents();

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),modelFact,SLOT(select()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormFact::~FormFact()
{
    delete ui;
}

void FormFact::save()
{
    ui->tableView->save(ui->label->text(),modelFact->decimal(),true,Qt::LandscapeOrientation);
}
