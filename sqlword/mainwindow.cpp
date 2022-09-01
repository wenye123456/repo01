#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->wordwidget=new WordWidget();
    this->setCentralWidget(wordwidget);
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("word.db");
    if(!db.open())
    {
        QMessageBox::critical(this," ","database can not open");
        close();
    }
    dbtip=new QLabel(db.databaseName());
    this->ui->statusbar->addWidget(dbtip);
    //this->ui->statusbar->showMessage(db.databaseName());

    init();
    setupstylesheet();

}

MainWindow::~MainWindow()
{

    saveconfig();
    delete ui;
}

void MainWindow::newTable(QString str)
{

    if(tablenames.contains(str))
    {
            QMessageBox::critical(this,"","this  table already opened");
            return;
    };
    tablenames.push_back(str);
    auto model =new tablemodel();
    models.push_back(model);

    model->setTable(str);
    model->setHeaderData(1,Qt::Orientation::Horizontal,"word");
    model->setHeaderData(2,Qt::Orientation::Horizontal,"meaning");


    auto tableview=new QTableView;
    views.push_back(tableview);
    tableview->setModel(model);
    tableview->setColumnHidden(0,true);
    tableview->setColumnHidden(3,true);
    tableview->setColumnHidden(4,true);
    tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableview->horizontalHeader()->setStretchLastSection(true);
    tableview->verticalHeader()->hide();
    tableview->setSortingEnabled(true);
    connect(tableview,SIGNAL(pressed(const QModelIndex &)),this,SLOT(on_tableviewpress(const QModelIndex &)));

    this->wordwidget->addTab(tableview,str);
    model->select();
}

void MainWindow::init()
{
    this->wordwidget->setTabsClosable(true);
    connect(this->wordwidget,&WordWidget::tabCloseRequested,this,[&](int index)
    {
        models.remove(index);
        tablenames.removeAt(index);
        views.remove(index);
        wordwidget->removeTab(index);
    });
     loadconfig();
}

void MainWindow::setupstylesheet()
{
    QString stylesheetdir= ":/qss/qss/";

    QDir  dir(stylesheetdir);
    QFileInfoList fileinfos=dir.entryInfoList();
    for(auto info:fileinfos)
    {
        if(info.isDir())
            continue;
        if(info.suffix()!="qss")
            continue;
        QAction* action=new QAction;
        action->setText(info.fileName());
        ui->menustyle->addAction(action);
        connect(action,&QAction::triggered,this,[=]()
        {
            loadstylesheet(stylesheetdir+info.fileName());
        });

    }
}

void MainWindow::loadstylesheet(QString str)
{
    if(str.isEmpty())
        return;
    QFile qssfile(str,this);

    qssfile.open(QFile::ReadOnly);
    QString stylesheet=qssfile.readAll();
    if(!qssfile.isOpen())
    {
        QMessageBox::information(this,"error","the file is not found");
        qssfile.close();
        return;
    }
    this->setStyleSheet(stylesheet);
    currentstylesheet=str;
    qssfile.close();
}

void MainWindow::dialogsetstylesheet(void *widget)
{
    if(currentstylesheet=="")
    {
        return;
    }
    QString qssfilepath=currentstylesheet;
    QFile qssfile(qssfilepath,this);
    qssfile.open(QFile::ReadOnly);
    QString stylesheet=qssfile.readAll();
    if(!qssfile.isOpen())
    {
        QMessageBox::information(this,"error","the file is not found");
        qssfile.close();
        return;
    }
    static_cast<QWidget *>(widget)->setStyleSheet(stylesheet);
    return;
}

void MainWindow::loadconfig()
{
    QSettings setting("init.ini",QSettings::IniFormat);
    setting.beginGroup("NORMCONFIG");
    isautosearch=setting.value("autosearch").toBool();
    currentstylesheet=setting.value("stylesheet").toString();
    setting.endGroup();
    loadstylesheet(currentstylesheet);

}

void MainWindow::saveconfig()
{
    QSettings setting("init.ini",QSettings::IniFormat);
    setting.beginGroup("NORMCONFIG");
    setting.setValue("autosearch",isautosearch);
    setting.setValue("stylesheet",currentstylesheet);
    setting.endGroup();
}

void MainWindow::on_actionclose_triggered()
{
    this->close();
}


void MainWindow::on_actionadd_triggered()
{
    if(wordwidget->currentIndex()==-1)
    {
        QMessageBox::critical(this,"","no table open");
        return;

    }
    AddDialog * addia=new AddDialog();
    dialogsetstylesheet(addia);
      addia->autosearch(isautosearch);

    if(QDialog::Accepted==addia->exec())
    {
    auto name=addia->name();
    auto meaning=addia->meaning();
    auto index=this->wordwidget->currentIndex();
    auto model=models.at(index);
    auto tableview=views.at(index);

    meaning=meaning.simplified();
    if(meaning.length()>50)
    {
        QMessageBox::critical(this,"","the meaning too long");
        return;
    }
    QStringList list;
    for(int i=0;i<model->rowCount();i++)
    {
        list<<model->data(model->index(i,1)).toString();
    }
    if(list.contains(name))
    {
        this->ui->statusbar->showMessage("the word already exist");
        return;
    }
    if(name=="")
        return;

    model->insertRow(0) ;
    model->setData(model->index(0,1),name);
    model->setData(model->index(0,2),meaning);
    model->setData(model->index(0,3),0);
    model->setData(model->index(0,4),QDateTime::currentDateTime());
    tableview->selectRow(model->index(0,0).row());
    }
}


void MainWindow::on_actionopentable_triggered()
{
     auto tabdia=new TableDialog;
     QSqlQuery initquery(db);
     QStringList list;


      initquery.exec("SELECT name _id FROM sqlite_master WHERE type ='table'");
      while(initquery.next())
      {
          if(initquery.value("_id").toString()=="sqlite_sequence")
              continue;
          list<<initquery.value("_id").toString();
          tabdia->addlabel(initquery.value("_id").toString());
      }
      tabdia->setlinecompelte(new QCompleter(list));
      dialogsetstylesheet(tabdia);
      connect(tabdia,QOverload<QString>::of(&TableDialog::clicktable),this,[&](QString str)
      {
          newTable(str);
          tabdia->close();
          return;
      });

    if(tabdia->exec())
    {
        QString tablename=tabdia->tablename();
        if(tablename=="sqlite_sequence")
        {
            QMessageBox::critical(this,"","illegal name");
            return;
        }
        QSqlQuery query(db);
        query.exec(QString("SELECT name _id FROM sqlite_master WHERE type ='table' and name='%1'").arg(tablename));
        if(!query.next())
        {
            query.exec(QString("Create table %1 ("
                               "id integer primary key autoincrement,"
                               "name varchar(20) not null unique,"
                               "meaning varchar(50) not null,"
                               "mark int,"
                               "date datetime);").arg(tablename));
            qDebug()<<query.lastError();
        }
        newTable(tablename);
    }
}


void MainWindow::on_actionremove_triggered()
{
    if(this->wordwidget->currentIndex()==-1)
    {
          QMessageBox::critical(this,"","no table open");
          return;
    }
    auto index=this->wordwidget->currentIndex();
    auto model=models.at(index);
    auto tableview=views.at(index);


    auto temp=tableview->selectionModel();
    auto indexes=temp->selectedIndexes();
    for (QModelIndex index : indexes) {
        int row = index.row();
        model->removeRows(row, 1, QModelIndex());
    }
    model->select();
}


void MainWindow::on_actionedit_triggered()
{
    if(this->wordwidget->currentIndex()==-1)
    {
          QMessageBox::critical(this,"","no table open");
          return;
    }
    auto index=this->wordwidget->currentIndex();
    auto model=models.at(index);
    auto tableview=views.at(index);
    auto temp=tableview->selectionModel();
    auto indexes=temp->selectedIndexes();
    if(indexes.isEmpty())
    {
        QMessageBox::critical(this,"","no titem selected");
  }
    AddDialog *editdia=new AddDialog;

    dialogsetstylesheet(editdia);
    for (QModelIndex index : indexes) {
        int row = index.row();
        auto setname=model->data(model->index(row,1)).toString();
        auto setmeaning=model->data(model->index(row,2)).toString();
        editdia->editmeaning(setname,setmeaning);
    }
    if(editdia->exec())
    {

        auto name=editdia->name();
        auto meaning=editdia->meaning();
        if(meaning.length()>50)
        {
            QMessageBox::critical(this,"","the meaning too long");
            return;
        }

        for (QModelIndex index : indexes) {
            int row = index.row();
            model->setData(model->index(row,1),name);
            model->setData(model->index(row,2),meaning);
        }
    }
}

void MainWindow::on_actionsearch_triggered()
{
    if(this->wordwidget->currentIndex()==-1)
    {
          QMessageBox::critical(this,"","no table open");
          return;
    }
  SearchDialog * searchdia=new SearchDialog;
  dialogsetstylesheet(searchdia);

  QStringList list;
  auto index=this->wordwidget->currentIndex();
  auto model=models.at(index);
  auto tableview=views.at(index);
  for(int i=0;i<model->rowCount();i++)
  {
      list<<model->data(model->index(i,1)).toString();
  }
  searchdia->setcompleter(new QCompleter(list));
  if(searchdia->exec())
  {
      auto name=searchdia->GetName();
      int i=-1;
      for(auto word:list)
      {
        if(word==name)
        {

            i=list.indexOf(word);
            break;
        }
       }
    if(i==-1)
    {
        QMessageBox::information(this,"error","no found");
        return;
    }
    tableview->selectRow(i);
      }
  }


void MainWindow::on_actionsortbytime_triggered()
{
    if(this->wordwidget->currentIndex()==-1)
    {
          QMessageBox::critical(this,"","no table open");
          return;
    }
    auto index=this->wordwidget->currentIndex();
    auto tableview=views.at(index);
    auto tablename=tablenames.at(index);

    auto model=new QSqlQueryModel();
    model->setQuery(QString("select * from '%1'  order by date asc").arg(tablename));
    tableview->setModel(model);
    this->ui->actionedit->setEnabled(false);
    this->ui->actionsearch->setEnabled(false);
    this->ui->actionremove->setEnabled(false);
    this->ui->actionadd->setEnabled(false);
}


void MainWindow::on_actionrefresh_triggered()
{
    for(int i=0;i<models.size();i++)
    {
        auto model=models.at(i);
        auto tableview=views.at(i);

        tableview->setModel(model);
        tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
    this->ui->actionedit->setEnabled(true);
    this->ui->actionsearch->setEnabled(true);
    this->ui->actionremove->setEnabled(true);
    this->ui->actionadd->setEnabled(true);
}


void MainWindow::on_actionrandom_triggered()
{
    if(wordwidget->currentIndex()==-1)
    {
        QMessageBox::critical(this,"","no table open");
        return;
    }
    auto model=models.at(wordwidget->currentIndex());
   RandomDialog *randdia=new RandomDialog(this,model);
   dialogsetstylesheet(randdia);

   randdia->show();
}


void MainWindow::on_actionclosetable_triggered()
{
     auto tabdia=new TableDialog;
     QSqlQuery initquery(db);
     QStringList list;

     initquery.exec("SELECT name _id FROM sqlite_master WHERE type ='table'");
     while(initquery.next())
     {
         if(initquery.value("_id").toString()=="sqlite_sequence")
             continue;
         list<<initquery.value("_id").toString();
         tabdia->addlabel(initquery.value("_id").toString());
     }
     tabdia->setlinecompelte(new QCompleter(list));
     dialogsetstylesheet(tabdia);
     tabdia->settext("input the table you want delete");

      if(tabdia->exec())
      {
          QString tablename=tabdia->tablename();
          if(tablename=="sqlite_sequence")
          {
              QMessageBox::critical(this,"","illegal name");
              return;
          }
          QSqlQuery query(db);

          if(list.contains(tablename))
          {
              if(QMessageBox::question(this,"",QString("confirm to delete %1").arg(tablename))==QMessageBox::Yes)
              {
                  query.exec(QString("drop table %1").arg(tablename));
              }
              return;
          }
          QMessageBox::critical(this,"","no exist");
      }
      return;


}

void MainWindow::on_tableviewpress(const QModelIndex &index)
{
    if(QGuiApplication::mouseButtons()==Qt::RightButton)
    {
        auto model=models.at(this->wordwidget->currentIndex());
        auto ismark= model->data(model->index(index.row(),3)).toInt();
        QMenu *menu=new QMenu;
#if defined(Q_CC_MSVC)
        QTextCodec* codec=QTextCodec::codecForName("GBK");
#endif
#if defined(Q_CC_MINGW)
        QTextCodec* codec=QTextCodec::codecForName("UTF-8");
#endif
        QAction *newaction1=new QAction(codec->toUnicode("收藏"));
        QAction *newaction2=new QAction(codec->toUnicode("取消收藏"));
        QAction *newaction3=new QAction(codec->toUnicode("搜索"));
        menu->addAction(newaction1);
        menu->addAction(newaction2);
        menu->addAction(newaction3);
        if(ismark)
        {
            newaction1->setEnabled(0);
            newaction2->setEnabled(1);
        }
        else
        {
            newaction1->setEnabled(1);
            newaction2->setEnabled(0);
        }
        connect(newaction1,&QAction::triggered,this,[&]()
        {
            model->setData((model->index(index.row(),3)),1);
            model->submit();
        });
        connect(newaction2,&QAction::triggered,this,[&]()
        {
            model->setData((model->index(index.row(),3)),0);
            model->submit();
        });
        connect(newaction3,&QAction::triggered,this,[&]()
        {
            QString word=model->data(model->index(index.row(),1)).toString();
            QString url=QString("https://www.youdao.com/result?word=%1&lang=en").arg(word);
            QDesktopServices::openUrl(QUrl(url));
        });
        menu->exec(QCursor::pos());
    }
}


void MainWindow::on_actionshowmark_triggered()
{
    if(this->wordwidget->currentIndex()==-1)
    {
          QMessageBox::critical(this,"","no table open");
          return;
    }
    auto index=this->wordwidget->currentIndex();
    auto tableview=views.at(index);
    auto tablename=tablenames.at(index);

    auto model=new QSqlQueryModel();
    model->setQuery(QString("select * from '%1'  where mark=1").arg(tablename));
    qDebug()<<model->lastError();
    tableview->setModel(model);
    this->ui->actionedit->setEnabled(false);
    this->ui->actionsearch->setEnabled(false);
    this->ui->actionremove->setEnabled(false);
    this->ui->actionadd->setEnabled(false);
}


void MainWindow::on_actionmarktotabel_triggered()
{
    if(this->wordwidget->currentIndex()==-1)
    {
          QMessageBox::critical(this,"","no table open");
          return;
    }
    SearchDialog *dia=new SearchDialog;
    dialogsetstylesheet(dia);

    auto modelold=models.at(this->wordwidget->currentIndex());
    QStringList list;
     QSqlQuery initquery(db);
      initquery.exec("SELECT name _id FROM sqlite_master WHERE type ='table'");
      while(initquery.next())
      {
          list<<initquery.value("_id").toString();
      }

      dia->exec();
      auto filename=dia->GetName();
      if(list.contains(filename))
      {
          QMessageBox::critical(this,"","tabel already exist");
          return;
      }
      initquery.exec(QString("Create table %1 ("
                         "id integer primary key autoincrement,"
                         "name varchar(20) not null unique,"
                         "meaning varchar(50) not null,"
                         "mark int,"
                         "date datetime);").arg(filename));
      newTable(filename);
      auto modelnew=models.back();
      for (int i=0;i<modelold->rowCount() ;i++ ) {
          auto ismark=modelold->data(modelold->index(i,3)).toInt();
          if(ismark)
          {
          auto name=modelold->data(modelold->index(i,1)).toString();
          auto meaning=modelold->data(modelold->index(i,2)).toString();
          modelnew->insertRow(0) ;
          modelnew->setData(modelnew->index(0,1),name);
          modelnew->setData(modelnew->index(0,2),meaning);
          modelnew->setData(modelnew->index(0,3),0);
          modelnew->setData(modelnew->index(0,4),QDateTime::currentDateTime());
          modelnew->submit();
          }
      }
}


void MainWindow::on_actionsetting_triggered()
{
    SettingDialog *settingdia=new SettingDialog(this);
    if(settingdia->exec()==QDialog::Accepted)
    {
        this->isautosearch=settingdia->isautosearch();
    }
}

