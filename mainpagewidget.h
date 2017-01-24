#ifndef MAINPAGEWIDGET_H
#define MAINPAGEWIDGET_H

#include <QWidget>

namespace Ui {
class MainPageWidget;
}

class MainPageWidget : public QWidget{
    Q_OBJECT

public:
    explicit MainPageWidget(QWidget *parent = 0);
    ~MainPageWidget();

private slots:
    void on_commandLinkButton_clicked();

private:
    Ui::MainPageWidget *ui;
};

#endif // MAINPAGEWIDGET_H
