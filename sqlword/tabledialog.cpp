#include "tabledialog.h"
#include "ui_tabledialog.h"

TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    layout=new QVBoxLayout();

}

TableDialog::~TableDialog()
{
    delete ui;
}

QString TableDialog::tablename()
{
    return ui->lineEdit->text();
}

void TableDialog::settext(QString str)
{
    this->ui->labelhint->setText(str);
}

void TableDialog::setlinecompelte(QCompleter* com)
{
    ui->lineEdit->setCompleter(com);
}

void TableDialog::addlabel(QString str)
{
    QLabel * label=new QLabel(str,this->ui->scrollAreaWidgetContents);
    //label->setFixedSize(QSize(230,80));
    label->setFixedHeight(80);
   // label->setFrameShape(QFrame::StyledPanel);
    layout->addWidget(label);
    ui->scrollAreaWidgetContents->setLayout(layout);
    label->installEventFilter(this);
    //connect(label,QOverload<QMouseEvent>::of(&QLabel::mousePressEvent));
}

bool TableDialog::eventFilter(QObject* o, QEvent* e)
{

    if(e->type()==QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseenevt = static_cast<QMouseEvent*>(e);

        if(mouseenevt->button()==Qt::LeftButton)
        {
            QLabel* label=static_cast<QLabel*>(o);
            emit clicktable(label->text());
        }
    }
    if(e->type()==QEvent::QEvent::Enter)
    {
        QLabel* label=static_cast<QLabel*>(o);

        label->setFrameShape(QFrame::StyledPanel);
    }
    if(e->type()==QEvent::QEvent::Leave)
    {
        QLabel* label=static_cast<QLabel*>(o);

        label->setFrameShape(QFrame::NoFrame);
    }

    return QDialog::eventFilter(o,e);
}
