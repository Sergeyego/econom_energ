#include "modelnorm.h"

ModelNorm::ModelNorm(QObject *parent) : DbTableModel("econom_energ_norm",parent)
{
    addColumn("id_provol",QString::fromUtf8("Марка"), new DbRelation("select id, nam from provol order by nam",0,1,this));
    addColumn("id_diam",QString::fromUtf8("Диаметр"), new DbRelation("select id, sdim from diam order by sdim",0,1,this));
    addColumn("id_spool",QString::fromUtf8("Носитель"), new DbRelation("select id, short from wire_pack_kind order by short",0,1,this));
    addColumn("draw1",QString::fromUtf8("1 подтяжка"));
    addColumn("ann",QString::fromUtf8("Отжиг"));
    addColumn("draw2",QString::fromUtf8("2 подтяжка"));
    addColumn("draw3",QString::fromUtf8("Волочение"));
    addColumn("wind",QString::fromUtf8("Перемотка"));
    setSuffix("inner join provol p on p.id = econom_energ_norm.id_provol "
              "inner join diam d on d.id = econom_energ_norm.id_diam");
    setSort("p.nam, d.diam, econom_energ_norm.id_spool");

    setColumnFlags(0,Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    setColumnFlags(1,Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    setColumnFlags(2,Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void ModelNorm::refresh(QDate beg, QDate end)
{
    QSqlQuery query;
    query.prepare("insert into econom_energ_norm (id_provol, id_diam, id_spool) "
                  "select "
                  "distinct m.id_provol, m.id_diam, p.id_pack "
                  "from wire_in_cex_data w "
                  "inner join wire_in_cex_type t on w.id_type=t.id and t.koef=1 "
                  "inner join wire_parti p on w.id_wparti=p.id "
                  "inner join wire_parti_m m on p.id_m=m.id "
                  "where w.dat between :d1 and :d2 and "
                  "(m.id_provol || '-' || m.id_diam || '-' || p.id_pack not in (select id_provol || '-' || id_diam || '-' || id_spool from econom_energ_norm)) "
                  "order by m.id_provol, m.id_diam, p.id_pack ");
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    if (!query.exec()){
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
    select();
}

QVariant ModelNorm::data(const QModelIndex &index, int role) const
{
    if (role==Qt::BackgroundColorRole){
        bool exist=false;
        for (int i=3; i<columnCount(); i++){
            exist = exist || !this->data(this->index(index.row(),i),Qt::EditRole).isNull();
            if (exist){
                break;
            }
        }
        return exist ? QVariant(QColor(255,255,255)) : QVariant(QColor(255,170,170));
    }
    return DbTableModel::data(index,role);
}

bool ModelNorm::insertRow(int /*row*/, const QModelIndex &/*parent*/)
{
    return false;
}
