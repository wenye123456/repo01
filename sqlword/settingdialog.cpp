#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "mainwindow.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    if(parent==nullptr)
        return;
    auto p=static_cast<MainWindow *>(parent);
#if defined(Q_CC_MSVC)
        QTextCodec* codec=QTextCodec::codecForName("GBK");
#endif
#if defined(Q_CC_MINGW)
        QTextCodec* codec=QTextCodec::codecForName("UTF-8");
#endif
    auto layout1=new QGridLayout;
    auto stylesheetlabel=new QLabel(codec->toUnicode("样式表:"));
    auto autosearchlabel=new QLabel(codec->toUnicode("自动查找:"));
    autosearchcheckbox=new QCheckBox(codec->toUnicode("启用"));

    autosearchcheckbox->setChecked(p->isautosearch);
    layout1->setColumnStretch(8,1);
    layout1->addWidget(stylesheetlabel,0,0);
    layout1->addWidget(autosearchlabel,1,0);
    layout1->addWidget(autosearchcheckbox,1,1);
    ui->page1->setLayout(layout1);

    connect(ui->applybutton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(ui->exitbutton, &QAbstractButton::clicked, this, &QDialog::reject);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

bool SettingDialog::isautosearch()
{
    return autosearchcheckbox->isChecked();
}
