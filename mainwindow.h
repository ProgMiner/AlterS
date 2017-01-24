#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void applyChanges();
    ~MainWindow();

private slots:
    void on_action_4_triggered();

    void on_action_2_triggered();

    void actionGroup_triggered(QAction* act);

    void on_action_triggered();

    void on_action_5_triggered();

    void on_action_7_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
