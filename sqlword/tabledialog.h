#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QCompleter>
#include <QLayout>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDialog(QWidget *parent = nullptr);
    ~TableDialog();

    QString tablename();
    void settext(QString str);
    void setlinecompelte(QCompleter *com);
    void addlabel(QString str);
    bool eventFilter(QObject* o, QEvent* e);
signals:
    void clicktable(QString);
private:
    Ui::TableDialog *ui;
    QVBoxLayout *layout;
};

#endif // TABLEDIALOG_H
