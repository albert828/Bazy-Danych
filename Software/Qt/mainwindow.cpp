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
        //QMessageBox::information(this, "Good :)", "Connected");
        ui->statusBar->showMessage("Conected :)", 5000);
        ui->comboBox->addItem("All");
        ui->comboBox_2->addItem("All");

        QSqlQuery query;
        query.prepare("SELECT name FROM sensors");
        query.exec();
        while (query.next())
        {
            QString name = query.value(0).toString();
            ui->comboBox_2->addItem(name);
        }

        query.prepare("SELECT name FROM rooms");
        query.exec();
        while (query.next())
        {
            QString name = query.value(0).toString();
            ui->comboBox->addItem(name);
        }
    }
    else
        //QMessageBox::information(this, "Bad :(", "Not connected");
        ui->statusBar->showMessage("Something goes wrong :(", 5000);
}

void MainWindow::on_actionDisconnect_triggered()
{
    if(db.open())
    {
        ui->actionDisconnect->setEnabled(false);
        ui->actionConnect->setEnabled(true);
        db.close();
        ui->statusBar->showMessage("Conection closed", 5000);
        ui->listWidget->clear();
        ui->comboBox->clear();
        ui->comboBox_2->clear();
        ui->spinBox->setValue(0);
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->listWidget->clear();
    room = ui->comboBox->currentText();
    sensor = ui->comboBox_2->currentText();
    nr_records = ui->spinBox->value();
    //qDebug() << room << " " << sensor << " " << nr_records;
    ui->statusBar->showMessage("Ok", 5000);

    QSqlQuery query;
    if(room == "All" || sensor == "All")
    {
        QSqlQuery query1, query2;
        QString q = "Select * FROM measurement ";
        if(room != "All")
        {
            query2.prepare("SELECT room_id FROM rooms WHERE name=(:room)");
            query2.bindValue(":room", room);
            query2.exec();
            query2.next();
            //qDebug() << query2.value(0).toString();
            q += "WHERE room_id=" + query2.value(0).toString();
            qDebug() << q;
        }

        if(sensor != "All")
        {
            query1.prepare("SELECT sensor_id FROM sensors WHERE name=(:sensor)");
            query1.bindValue(":sensor", sensor);
            query1.exec();
            query1.next();
            q += "WHERE sensor_id=" + query1.value(0).toString();
            qDebug() << q;
        }
        q += " ORDER BY measure_id DESC";
        if(ui->spinBox->value() != 0)
            q += " LIMIT " + QString::number(ui->spinBox->value());
        query.prepare(q);
        //query.bindValue(":room_id", room_id);
        query.exec();
        //QSqlRecord rec = query.record();
        //qDebug() << rec.count();
        while(query.next())
        {
            //qDebug() << query.value(0).toString();
            QString text = ( "ID pomiaru: " + query.value(0).toString() );
            query1.prepare("Select name FROM sensors WHERE sensor_id = (:sensor_id)");
            query1.bindValue(":sensor_id", query.value(1).toString());
            query1.exec();
            query1.next();
            text += ( "\tCzujnik: " + query1.value(0).toString() );
            text += ( "\tWartosc: " + query.value(2).toString() );
            query2.prepare("SELECT name FROM rooms WHERE room_id=(:room_id)");
            query2.bindValue(":room_id", query.value(3).toString());
            query2.exec();
            query2.next();
            //qDebug() << query.value(3).toString() << "   " << query2.value(0).toString();
            text += ( "\t\tPomieszczenie: " + query2.value(0).toString() );
            text += ( "\t\tData: " + query.value(5).toString() + " " + query.value(5).toString() );
            ui->listWidget->addItem(text);
        }
    }
    else
    {
        //QString text = "Select room_id FROM rooms WHERE name = (:name)";
        //qDebug() << text;
        query.prepare("SELECT room_id FROM rooms WHERE name = (:name)");
        query.bindValue(":name", room);
        query.exec();
        query.next();
        //qDebug() << query.value(0).toString();
        uint32_t room_id = query.value(0).toInt();
        //qDebug() << room_id;

        query.prepare("SELECT sensor_id FROM sensors WHERE name = (:name)");
        query.bindValue(":name", sensor);
        query.exec();
        query.next();
        //qDebug() << query.value(0).toString();
        uint32_t sensor_id = query.value(0).toInt();
        //qDebug() << sensor_id;
        QString qr = "SELECT * FROM measurement WHERE ( room_id = (:room_id) AND sensor_id = (:sensor_id) ) "
                     "ORDER BY measure_id DESC";
        if(ui->spinBox->value() != 0)
            qr += " LIMIT " + QString::number(ui->spinBox->value());
        query.prepare(qr);
        query.bindValue(":room_id", room_id);
        query.bindValue(":sensor_id", sensor_id);
        //query.bindValue(":rec", nr_records);
        query.exec();
        //QSqlRecord rec = query.record();
        //qDebug() << rec.count();
        while(query.next())
        {
            //qDebug() << query.value(0).toString();
            QString text = ( "ID pomiaru: " + query.value(0).toString() );
            text += ( "\tCzujnik: " + sensor );
            text += ( "\tWartosc: " + query.value(2).toString() );
            text += ( "\tPomieszczenie: " + room );
            text += ( "\tData: " + query.value(5).toString() + " " + query.value(5).toString() );
            ui->listWidget->addItem(text);
            //qDebug() << query.value().to
        }
    }

    //query.exec();
}
