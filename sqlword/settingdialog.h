#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QCheckBox>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

    bool isautosearch();


private:
    Ui::SettingDialog *ui;
    QCheckBox *autosearchcheckbox;
};

#endif // SETTINGDIALOG_H
