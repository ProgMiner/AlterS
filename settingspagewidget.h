#ifndef SETTINGSPAGEWIDGET_H
#define SETTINGSPAGEWIDGET_H

#include <QWidget>

namespace Ui {
class SettingsPageWidget;
}

class SettingsPageWidget : public QWidget{
    Q_OBJECT

public:
    explicit SettingsPageWidget(QWidget *parent = 0);
    void toTab(int i);
    void refreshAccountsTable();
    ~SettingsPageWidget();

private slots:
    void on_tableWidget_customContextMenuRequested();

    void on_action_triggered();

    void on_action_3_triggered();

    void on_action_2_triggered();

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_plainTextEdit_textChanged();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::SettingsPageWidget *ui;
};

#endif // SETTINGSPAGEWIDGET_H
