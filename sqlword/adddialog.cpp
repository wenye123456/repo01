#include "adddialog.h"

#include <QtWidgets>

//! [0]
AddDialog::AddDialog(QWidget *parent)
    : QDialog(parent),
      nameText(new QLineEdit),
      meaningText(new QTextEdit)
{
    auto nameLabel = new QLabel(tr("Name"));
    auto meaningLabel = new QLabel(tr("Address"));
    auto okButton = new QPushButton(tr("OK"));
    auto cancelButton = new QPushButton(tr("Cancel"));

    this->setFixedSize(400,300);

    auto gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(nameLabel, 0, 0);
    gLayout->addWidget(nameText, 0, 1);

    gLayout->addWidget(meaningLabel, 1, 0, Qt::AlignLeft|Qt::AlignTop);
    gLayout->addWidget(meaningText, 1, 1, Qt::AlignLeft);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 2, 1, Qt::AlignRight);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    this->manager=new QNetworkAccessManager;
    setWindowTitle(tr("Add a Word"));
}

QString AddDialog::name() const
{
    return nameText->text();
}

QString AddDialog::meaning() const
{
    return meaningText->toPlainText();
}

void AddDialog::editmeaning(const QString &name, const QString &address)
{
    //nameText->setReadOnly(true);
    nameText->setText(name);
    meaningText->setPlainText(address);
}

void AddDialog::autosearch(bool flag)
{
    if(flag==1)
      connect(nameText,&QLineEdit::editingFinished,this,[&]()
    {
        auto word=this->name();
        if(word.isEmpty())
            return;
        QString url=QString("https://www.youdao.com/result?word=%1&lang=en").arg(word);

        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.131 Safari/537.36 SLBrowser/8.0.0.3161 SLBChan/25");
        manager->get(request);

        connect(manager,QOverload<QNetworkReply *>::of(&QNetworkAccessManager::finished),this,[&](QNetworkReply *reply)
        {

            auto html=QString(reply->readAll());
            if(html.isEmpty())
            {
                reply->deleteLater();
                return;
            }
            QRegularExpression exppos("<span[^>]*class=\"pos\"[^>]*>(.[^>]*)</span>");
            QRegularExpression exp("<span[^>]*class=\"trans\"[^>]*>(.[^<]*)</span>");
            QString meaning;
            auto i=exp.globalMatch(html);
            auto ipos=exppos.globalMatch(html);
            //auto temps=exp.match(html).capturedTexts();
             while(i.hasNext())
             {
                 if(!ipos.hasNext())
                     break;
                 auto matchpos=ipos.next();
                 qDebug()<<matchpos.captured(1);
                 meaning+=matchpos.captured(1);
                 auto match=i.next();
                 meaning+=match.captured(1);
                 meaning+=" ";
             }
           meaningText->setText(meaning);
            reply->deleteLater();
        });
    });
    if(flag==0)
    {
        nameText->disconnect(SIGNAL(QLineEdit::editingFinished));


    }

}
//! [0]
