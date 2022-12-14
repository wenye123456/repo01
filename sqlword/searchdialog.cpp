#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::setcompleter(QCompleter *com)
{
    ui->SearchlineEdit->setCompleter(com);
}

QString SearchDialog::GetName()
{
    return ui->SearchlineEdit->text();
}
