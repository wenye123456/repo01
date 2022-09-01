#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QCompleter>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

    void setcompleter(QCompleter * com);
    QString GetName();

private:
    Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
