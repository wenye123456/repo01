#ifndef TABLEMODEL_H
#define TABLEMODEL_H


#include <QSqlTableModel>
#include <QVector>
#include <QVariant>
#include <QBrush>






class tablemodel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit tablemodel(QObject *parent = nullptr);

    // Header:
     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;



private:

};

#endif // TABLEMODEL_H
