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
    ui->pushButton->setEnabled(false);
    ui->recommendations->setEnabled(false);
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
        ui->pushButton->setEnabled(true);
        ui->recommendations->setEnabled(true);
        //QMessageBox::information(this, "Good :)", "Connected");
        ui->statusBar->showMessage("Conected :)", 5000);
        ui->comboBox->addItem("All");
        ui->comboBox_2->addItem("All");
        ui->comboBox_3->addItem("Descending");
        ui->comboBox_3->addItem("Ascending");

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
        ui->pushButton->setEnabled(false);
        ui->recommendations->setEnabled(false);
        db.close();
        ui->statusBar->showMessage("Conection closed", 5000);
        ui->listWidget->clear();
        ui->comboBox->clear();
        ui->comboBox_2->clear();
        ui->spinBox->setValue(0);
        ui->comboBox_3->clear();
        ui->comboBox_3->clear();
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

    QSqlQuery query, query_statement;
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
        q += " ORDER BY measure_id";
        if(ui->comboBox_3->currentText() == "Descending")
            q += " DESC";
        else
            q += " ASC";
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
            text += ( "\t\tData: " + query.value(4).toString());// + " " + query.value(5).toString() );
            ui->listWidget->addItem(text);
        }
        QString text_statement = ("SELECT Temperatura, Cisnienie, Smog, CO2, Wilgotnosc, Alkohol, "
                                  "Halas, Dym, Swiatlo FROM rooms WHERE name=(:room_name)");
        query_statement.prepare(text_statement);
        query_statement.bindValue(":room_name", query2.value(0).toString());
        query_statement.exec();
        query_statement.next();
        QString statement_text = ("Aktualny komunikat: "
                                  + query_statement.value(0).toString() +
                                  ", "  + query_statement.value(1).toString() +
                                  ", "  + query_statement.value(2).toString() +
                                  ", "  + query_statement.value(3).toString() +
                                  ", "  + query_statement.value(4).toString() +
                                  ", "  + query_statement.value(5).toString() +
                                  ", "  + query_statement.value(6).toString() +
                                  ", "  + query_statement.value(7).toString() +
                                  ", "  + query_statement.value(8).toString());
        ui->listWidget->insertItem(0,statement_text);
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
                     "ORDER BY measure_id";
        if(ui->comboBox_3->currentText() == "Descending")
            qr += " DESC";
        else
            qr += " ASC";
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
            text += ( "\tData: " + query.value(4).toString());// + " " + query.value(5).toString() );
            ui->listWidget->addItem(text);
            //qDebug() << query.value().to
        }
        QString text_statement = ("SELECT Temperatura, Cisnienie, Smog, CO2, Wilgotnosc, Alkohol, "
                                  "Halas, Dym, Swiatlo FROM rooms WHERE name=(:room_name)");
        query_statement.prepare(text_statement);
        query_statement.bindValue(":room_name", room);
        query_statement.exec();
        query_statement.next();
        QString statement_text = ("Aktualny komunikat: "
                                  + query_statement.value(0).toString() +
                                  ", "  + query_statement.value(1).toString() +
                                  ", "  + query_statement.value(2).toString() +
                                  ", "  + query_statement.value(3).toString() +
                                  ", "  + query_statement.value(4).toString() +
                                  ", "  + query_statement.value(5).toString() +
                                  ", "  + query_statement.value(6).toString() +
                                  ", "  + query_statement.value(7).toString() +
                                  ", "  + query_statement.value(8).toString());
        ui->listWidget->insertItem(0,statement_text);
    }

    //query.exec();
}

void MainWindow::on_recommendations_clicked()
{
    ui->listWidget->clear();
    room = ui->comboBox->currentText();
    ui->statusBar->showMessage("Ok", 5000);
    QSqlQuery query;
    if(room == "All")
    {
        QString q = "SELECT * from rooms";
        query.prepare(q);
        query.exec();
        QString text = ("Aktualny komunikat:");
        ui->listWidget->insertItem(0,text);
        while(query.next())
        {
            text = "Pomieszczenie: ";
            for (int32_t counter{0}; counter < 10; ++counter)
            {
                if(counter == 0)
                    continue;
                text += query.value(counter).toString();
                if(text.back() != ' ')
                    text += ' ';
            }
            ui->listWidget->insertItem(1,text);
        }
    }
    else
    {
        QString q = "SELECT * from rooms WHERE name=(:room_name)";
        query.prepare(q);
        query.bindValue(":room_name", room);
        query.exec();
        query.next();
        QString text = ("Aktualny komunikat:\n");
        text += "Pomieszczenie: ";
        text += room;
        text += ' ';
        for (int32_t counter{2}; counter < 10; ++counter)
        {
            text += query.value(counter).toString();
            if(text.back() != ' ')
                text += ' ';
        }
        ui->listWidget->insertItem(0,text);
    }
}
