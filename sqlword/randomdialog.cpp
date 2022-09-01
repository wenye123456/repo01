#include "randomdialog.h"
#include "ui_randomdialog.h"

RandomDialog::RandomDialog(QWidget *parent)
{
    close();
}

RandomDialog::RandomDialog(QWidget *parent, void *model):
    QDialog(parent),
    ui(new Ui::RandomDialog)
{
    if(model==nullptr)
    {
       close();
    }
    ui->setupUi(this);


    eventconnect();

    auto datamodel=static_cast<QSqlTableModel *>(model);
    for(int i=0;i<datamodel->rowCount();i++)
    {
        wordlist<<datamodel->data(datamodel->index(i,1)).toString();
        meaninglist<<datamodel->data(datamodel->index(i,2)).toString();
    }
    ui->wordlabel->setText(wordlist.first());
    ui->meaninglabel->setText("");
}

void RandomDialog::eventconnect()
{
    ui->horizontalSlider->setMaximum(20);
    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setValue(5);
    ui->spinBox->setValue(5);
    ui->spinBox->setMinimum(1);
    wordtimer.setInterval(5000);
    meaningtimer.setInterval(2500);
    currentindex=0;

    connect(this->ui->exitButton,&QPushButton::clicked,this,[&](){
        this->close();
    });
    connect(this->ui->horizontalSlider,&QSlider::valueChanged,this,[&](int v)
    {
        ui->spinBox->setValue(v);
        wordtimer.setInterval(v*1000);
        meaningtimer.setInterval(v*500);
        wordtimer.start();
        meaningtimer.start();
    });
    connect(this->ui->spinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,[&](int v)
    {
       ui->horizontalSlider->setValue(v);
       wordtimer.setInterval(v*1000);
       meaningtimer.setInterval(v*500);
       wordtimer.start();
       meaningtimer.start();
    });
    connect(&this->wordtimer,&QTimer::timeout,this,[&]()
    {
        if(!ui->randomcheckBox->isChecked())
        {
          currentindex=(++currentindex)%(wordlist.size());
         }
        else
        {
            QRandomGenerator g(QDateTime::currentMSecsSinceEpoch());
            currentindex=g.bounded(0,wordlist.size());
        }
        wordtimer.setInterval(wordtimer.interval());
        ui->wordlabel->setText(wordlist.at(currentindex));
        ui->meaninglabel->setText("");

        meaningtimer.start();
        wordtimer.start();

    });
    connect(&this->meaningtimer,&QTimer::timeout,this,[&]()
    {
        meaningtimer.setInterval(meaningtimer.interval());
        ui->meaninglabel->setText(meaninglist.at(currentindex));;
    }
    );
}





RandomDialog::~RandomDialog()
{
    delete ui;
}

bool RandomDialog::eventFilter(QObject *obj, QEvent *e)
{
    return QDialog::eventFilter(obj,e);
}

void RandomDialog::on_stoppushButton_clicked()
{
    if(ui->stoppushButton->isChecked())
    {
        ui->stoppushButton->setText(QString("start"));
        wordtimer.stop();
        meaningtimer.stop();
        ui->horizontalSlider->setUpdatesEnabled(false);
        ui->spinBox->setUpdatesEnabled(false);
    }
    else
    {
        ui->stoppushButton->setText(QString("stop"));
        wordtimer.start();
        meaningtimer.start();
        ui->horizontalSlider->setUpdatesEnabled(true);
        ui->spinBox->setUpdatesEnabled(true);
    }
}

