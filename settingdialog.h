#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private:
    Ui::SettingDialog *ui;
    int num=10;

public slots:
    void saveSLOT();
    void cliekZidingyi();
    void clickMoren();
    void clickWuxianzhi();

signals:
    void sendDeviceNumSIGNAL(int num);
};

#endif // SETTINGDIALOG_H
