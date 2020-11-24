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

ModelFact::ModelFact(QObject *parent) : ModelRo(parent)
{
    dec=1;
}

QVariant ModelFact::data(const QModelIndex &item, int role) const
{
    double koef=0.0;
    if (sumnorm.at(6)!=0.0){
        koef=sumfact/sumnorm.at(6);
    }
    if (item.row()==(this->rowCount()-1)){
        if (item.column()==0 && (role==Qt::DisplayRole || role==Qt::EditRole)){
            return QString("Итого");
        }
        if (item.column()>4 && item.column()<11){
            if (role==Qt::DisplayRole) {
                return QLocale().toString(sumnorm.at(item.column()-4)*koef,'f',dec);
            } else if (role==Qt::EditRole){
                return sumnorm.at(item.column()-4)*koef;
            } else if (role==Qt::TextAlignmentRole){
                return int(Qt::AlignRight | Qt::AlignVCenter);
            }
        }
        return QVariant();
    } else if (item.column()>4 && item.column()<11) {
        double val=ModelRo::data(item,Qt::EditRole).toDouble()*koef;
        if (role==Qt::EditRole){
            return val!=0.0 ? val : QVariant();
        } else if (role==Qt::DisplayRole){
            return val!=0.0 ? QLocale().toString(val,'f',dec) : QString();
        }
    }
    return ModelRo::data(item,role);
}

void ModelFact::refresh(QDate beg, QDate end, double sum)
{
    sumfact=sum;
    QSqlQuery query;
    query.prepare("select z.tip, z.mark, z.dim, z.spool, z.post_t*1000.0, "
                  "z.post_t*coalesce(en.draw1,0) as draw1, "
                  "z.post_t*coalesce(en.ann,0) as ann, "
                  "z.post_t*coalesce(en.draw2,0) as draw2, "
                  "z.post_t*coalesce(en.draw3,0) as draw3, "
                  "z.post_t*coalesce(en.wind,0) as wind, "
                  "z.post_t*( "
                  "coalesce(en.draw1,0)+coalesce(en.ann,0)+coalesce(en.draw2,0)+coalesce(en.draw3,0)+coalesce(en.wind,0)) "
                  "from ( "
                  "select t.nam as tip, pr.nam as mark, d.sdim as dim , k.short as spool, "
                  "sum(w.m_netto)/1000.0 as post_t, m.id_provol, m.id_diam, p.id_pack "
                  "from wire_in_cex_data w "
                  "inner join wire_in_cex_type t on w.id_type=t.id and t.koef=1 "
                  "inner join wire_parti p on w.id_wparti=p.id "
                  "inner join wire_parti_m m on p.id_m=m.id "
                  "inner join provol pr on pr.id=m.id_provol "
                  "inner join diam d on d.id=m.id_diam "
                  "inner join wire_pack_kind k on p.id_pack=k.id "
                  "where w.dat between :d1 and :d2 "
                  "group by t.nam, pr.nam, d.sdim, k.short, m.id_provol, m.id_diam, p.id_pack "
                  ") as z "
                  "inner join econom_energ_norm en on "
                  "en.id_spool=z.id_pack and en.id_diam=z.id_diam and en.id_provol=z.id_provol "
                  "order by z.tip, z.mark, z.dim, z.spool");
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    if (execQuery(query)){
        setHeaderData(0,Qt::Horizontal,QString::fromUtf8("Тип поступления"));
        setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Марка"));
        setHeaderData(2,Qt::Horizontal,QString::fromUtf8("Диам."));
        setHeaderData(3,Qt::Horizontal,QString::fromUtf8("Носитель"));
        setHeaderData(4,Qt::Horizontal,QString::fromUtf8("Произведено,кг"));
        setHeaderData(5,Qt::Horizontal,QString::fromUtf8("1 подтяжка,кВтч"));
        setHeaderData(6,Qt::Horizontal,QString::fromUtf8("Отжиг,кВтч"));
        setHeaderData(7,Qt::Horizontal,QString::fromUtf8("2 подтяжка,кВтч"));
        setHeaderData(8,Qt::Horizontal,QString::fromUtf8("Волочение,кВтч"));
        setHeaderData(9,Qt::Horizontal,QString::fromUtf8("Перемотка,кВтч"));
        setHeaderData(10,Qt::Horizontal,QString::fromUtf8("Всего,кВтч"));
    }
    sumnorm.resize(7);
    sumnorm.fill(0.0);
    query.seek(-1);
    while (query.next()){
        for (int i=0; i<7; i++)
        sumnorm[i]+=query.value(i+4).toDouble();
    }
}

int ModelFact::rowCount(const QModelIndex &parent) const
{
    return ModelRo::rowCount(parent)+1;
}
