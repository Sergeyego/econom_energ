#ifndef MODELNORM_H
#define MODELNORM_H

#include <QObject>
#include "db/dbtablemodel.h"
#include <QMessageBox>
#include "modelro.h"

class ModelNorm : public DbTableModel
{
public:
    ModelNorm(QObject *parent);
    void refresh(QDate beg, QDate end);
    QVariant data(const QModelIndex &index, int role) const;
    bool insertRow(int row, const QModelIndex &parent);
};

class ModelFact : public ModelRo
{
  public:
    ModelFact(QObject *parent=0);
    QVariant data(const QModelIndex &item, int role) const;
    void refresh(QDate beg, QDate end, double sum);
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
private:
    QVector<double> sumnorm;
    double sumfact;
};

#endif // MODELNORM_H
