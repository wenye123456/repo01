#ifndef RANDOMDIALOG_H
#define RANDOMDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSlider>
#include <QRandomGenerator>
#include <QDateTime>

namespace Ui {
class RandomDialog;
}

class RandomDialog : public QDialog
{
    Q_OBJECT

public:\
    explicit RandomDialog(QWidget *parent = nullptr);
    explicit RandomDialog(QWidget *parent = nullptr,void *model=nullptr);
    void eventconnect();
    ~RandomDialog();
protected:
     bool eventFilter(QObject *obj, QEvent *e);
private slots:
     void on_stoppushButton_clicked();

private:
    Ui::RandomDialog *ui;
    QStringList wordlist;
    QStringList meaninglist;
    int currentindex;
    QTimer wordtimer;
    QTimer meaningtimer;
};


#endif // RANDOMDIALOG_H
