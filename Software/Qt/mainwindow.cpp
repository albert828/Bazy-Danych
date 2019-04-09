#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QtSql/QSql>
//#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionDisconnect->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("microclimate");

    if(db.open())
    {
        ui->actionDisconnect->setEnabled(true);
        ui->actionConnect->setEnabled(false);
        QMessageBox::information(this, "Good :)", "Connected");
        QSqlQuery query;

        query.prepare("SELECT name FROM sensors");
        query.exec();
        while (query.next())
        {
            QString name = query.value(0).toString();
            ui->comboBox->addItem(name);
        }

        query.prepare("SELECT name FROM rooms");
        query.exec();
        while (query.next())
        {
            QString name = query.value(0).toString();
            ui->comboBox_2->addItem(name);
        }
    }
    else
        QMessageBox::information(this, "Bad :(", "Not connected");
}

void MainWindow::on_actionDisconnect_triggered()
{
    if(db.open())
    {
        ui->actionDisconnect->setEnabled(false);
        ui->actionConnect->setEnabled(true);
        db.close();
    }
}
