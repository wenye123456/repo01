#include "tablemodel.h"



tablemodel::tablemodel(QObject *parent)
    : QSqlTableModel(parent)
{
    this->setEditStrategy(QSqlTableModel::OnFieldChange);
}


QVariant tablemodel::data(const QModelIndex &index, int role ) const
{

    if(role==Qt::ForegroundRole)
    {
        auto ismark=this->data(this->index(index.row(),3),Qt::DisplayRole).toInt();
       if(ismark)
         return QVariant(QBrush(Qt::red));
    }
    return  QSqlTableModel::data(index,role);
}
