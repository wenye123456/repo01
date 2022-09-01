#ifndef WORDWIDGET_H
#define WORDWIDGET_H

#include <QWidget>
#include <QTableWidget>

class WordWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit WordWidget(QWidget *parent = nullptr);

signals:

};

#endif // WORDWIDGET_H
