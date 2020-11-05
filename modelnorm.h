#ifndef MODELNORM_H
#define MODELNORM_H

#include <QObject>
#include "db/dbtablemodel.h"
#include <QMessageBox>

class ModelNorm : public DbTableModel
{
public:
    ModelNorm(QObject *parent);
    void refresh(QDate beg, QDate end);
    QVariant data(const QModelIndex &index, int role) const;
    bool insertRow(int row, const QModelIndex &parent);
};

#endif // MODELNORM_H
