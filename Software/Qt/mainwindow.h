#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    void on_pushButton_clicked();

    void on_recommendations_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString room;
    QString sensor;
    uint32_t nr_records;
};

#endif // MAINWINDOW_H
