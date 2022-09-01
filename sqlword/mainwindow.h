#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStatusBar>
#include <QTableView>
#include <QDebug>
#include <QDateTime>
#include <QCompleter>
#include <QHeaderView>
#include <QFileDialog>
#include <QLabel>
#include <QFileInfo>
#include <QTextCodec>
#include <QDesktopServices>
#include <QSettings>

#include "wordwidget.h"
#include "adddialog.h"
#include "tablemodel.h"
#include "tabledialog.h"
#include "searchdialog.h"
#include "randomdialog.h"
#include "settingdialog.h"

#ifdef QT_NETWORK_LIB
    #include <QtNetwork>
#endif


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void newTable(QString str);
    void init();
    void setupstylesheet();
    void loadstylesheet(QString str);
    void dialogsetstylesheet(void *widget);
    void loadconfig();
    void saveconfig();

#ifdef QT_NETWORK_LIB
   void netinit();
#endif


private slots:
    void on_actionclose_triggered();
    void on_actionadd_triggered();
    void on_actionopentable_triggered();
    void on_actionremove_triggered();
    void on_actionedit_triggered();
    void on_actionsearch_triggered();
    void on_actionsortbytime_triggered();
    void on_actionrefresh_triggered();
    void on_actionrandom_triggered();
    void on_actionclosetable_triggered();
    void on_tableviewpress(const QModelIndex &index);
    void on_actionshowmark_triggered();
    void on_actionmarktotabel_triggered();
    void on_actionsetting_triggered();

private:
    QVector<tablemodel *>  models;
    QVector<QTableView *>  views;

    QStringList tablenames;
    QSqlDatabase db;
    QLabel *dbtip;

    Ui::MainWindow *ui;
    WordWidget *wordwidget;
    QString currentstylesheet;
#ifdef QT_NETWORK_LIB
public:
    bool isautosearch=0;
#endif

};
#endif // MAINWINDOW_H
