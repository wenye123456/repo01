
#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QtNetwork>

#include <QRegularExpression>


QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;
QT_END_NAMESPACE

//! [0]
class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(QWidget *parent = nullptr);

    QString name() const;
    QString meaning() const;
    void editmeaning(const QString &name, const QString &meaning);
    void autosearch(bool);

private:
    QLineEdit *nameText;
    QTextEdit *meaningText;
    QNetworkAccessManager *manager;
};
//! [0]

#endif // ADDDIALOG_H
