#ifndef ACCOUNTPAGEWIDGET_H
#define ACCOUNTPAGEWIDGET_H

#include "account.h"
#include <QWidget>

namespace Ui {
class AccountPageWidget;
}

class AccountPageWidget : public QWidget{
    Q_OBJECT

public:
    explicit AccountPageWidget(Main::Account acc, QWidget *parent = 0);
    Main::Account account;
    ~AccountPageWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void exportPubkey_action();

    void exportPrvkey_action();

    void on_commandLinkButton_clicked();

private:
    Ui::AccountPageWidget *ui;
};

#endif // ACCOUNTPAGEWIDGET_H
