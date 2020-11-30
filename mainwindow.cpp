#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CheckInputFields()
{
    if(ui->velocityX_lineEdit->text()!=""&&ui->velocityY_lineEdit->text()!="")
    {
        if(ui->direction_comboBox->currentText()!="Straight" && ui->turnRadius_lineEdit->text()!="")
            ui->addInstruction_pushButton->setEnabled(true);
        else if(ui->direction_comboBox->currentText()=="Straight")
            ui->addInstruction_pushButton->setEnabled(true);
        else
            ui->addInstruction_pushButton->setEnabled(false);
    }
    else
        ui->addInstruction_pushButton->setEnabled(false);
}

void MainWindow::AddEntry(QTableWidget* table)
{
    table->setRowCount(table->rowCount() + 1);
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText(ui->direction_comboBox->currentText());
    item->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 0, item);
    QTableWidgetItem* item2 = new QTableWidgetItem;
    QString velocityString = ui->velocityX_lineEdit->text();
    velocityString+=" ; ";
    velocityString+=ui->velocityY_lineEdit->text();
    item2->setText('('+velocityString+')');
    item2->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 1, item2);
    QTableWidgetItem* item3 = new QTableWidgetItem;
    item3->setText(QString::number(round(clock()/100.0)/10.0));
    item3->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 2, item3);
    QTableWidgetItem* item4 = new QTableWidgetItem;
    if(ui->direction_comboBox->currentText() == "Straight")
        item4->setText("-");
    else
        item4->setText(ui->turnRadius_lineEdit->text());
    item4->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 3, item4);
}

void MainWindow::CreateInstruction(bool& success)
{
    bool ok;
    double velocityX = ui->velocityX_lineEdit->text().toDouble(&ok);
    if(!ok)
    {
        QMessageBox::warning(this,"Wrong input","Velocity X is not a number");
        success = false;
        return;
    }
    else
    {
        double velocityY = ui->velocityY_lineEdit->text().toDouble(&ok);
        if(!ok)
        {
            QMessageBox::warning(this,"Wrong input","Velocity Y is not a number");
            success = false;
            return;
        }
        else if(ui->direction_comboBox->currentText()=="Straight")
        {
            success = true;
            _car.AddToQueue(velocityX,velocityY);
            //AddEntry(ui->currentInstructions_tableWidget);
            //AddEntry(ui->instructionHistory_tableWidget);
        }
        else
        {
            double turnRadius = ui->turnRadius_lineEdit->text().toDouble(&ok);
            if(!ok)
            {
                QMessageBox::warning(this,"Wrong input", "Turn radius is not a number");
                success = false;
                return;
            }
            else if(turnRadius<=0)
            {
                QMessageBox::warning(this,"Wrong input", "Turn radius is less or equals to 0");
                success = false;
                return;
            }
            else
            {
                success = true;
                Direction direction = _car.StrToDir(ui->direction_comboBox->currentText().toUpper());
                _car.AddToQueue(direction,turnRadius,velocityX,velocityY);
                //AddEntry(ui->currentInstructions_tableWidget);
                //AddEntry(ui->instructionHistory_tableWidget);
            }
        }
    }
}

void MainWindow::on_velocityX_lineEdit_textChanged(const QString &arg1)
{
    CheckInputFields();
}

void MainWindow::on_velocityY_lineEdit_textChanged(const QString &arg1)
{
    CheckInputFields();
}

void MainWindow::on_turnRadius_lineEdit_textChanged(const QString &arg1)
{
    CheckInputFields();
}

void MainWindow::on_direction_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Straight")
    {
        ui->turnRadius_lineEdit->setEnabled(false);
        ui->turnRadius_label->setEnabled(false);
        ui->turnRadius_lineEdit->setText("");
    }
    else
    {
        ui->turnRadius_lineEdit->setEnabled(true);
        ui->turnRadius_label->setEnabled(true);
        ui->addInstruction_pushButton->setEnabled(false);
    }
    CheckInputFields();
}

void MainWindow::on_addInstruction_pushButton_clicked()
{
    bool success;
    CreateInstruction(success);
    if(success)
        UpdateTables();
    if(success && !ui->clearInstructions_pushButton->isEnabled())
        ui->clearInstructions_pushButton->setEnabled(true);
    if(success && !ui->countPosition_pushButton->isEnabled())
        ui->countPosition_pushButton->setEnabled(true);
}

void MainWindow::on_countPosition_pushButton_clicked()
{
    if(ui->currentInstructions_tableWidget->rowCount()==0)
        QMessageBox::warning(this,"No entries","No entries in current instructions");
    else
    {
        PositionUpdate();
    }
}

void MainWindow::on_clearInstructions_pushButton_clicked()
{
    QStringList headers;
    headers << "Direction" << "Velocity" << "Time" << "Turn Radius";

    auto ciTable=ui->currentInstructions_tableWidget;
    ciTable->clear();
    ciTable->setRowCount(0);
    ciTable->setHorizontalHeaderLabels(headers);

    auto ihTable=ui->instructionHistory_tableWidget;
    ihTable->clear();
    ihTable->setRowCount(0);
    ihTable->setHorizontalHeaderLabels(headers);

    ui->clearInstructions_pushButton->setEnabled(false);
    ui->countPosition_pushButton->setEnabled(false);
    ui->positionValue_label->setText("");
    ui->velocityValue_label->setText("");
    ui->startTimeValue_label->setText("");
    _car.ClearAllInstructions();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(0,"Open Dialog","","*.json");
    if(filePath!="")
    {
        _car.SetQueue(filePath);
        PositionUpdate();
        auto ihTable=ui->instructionHistory_tableWidget;
        ihTable->clear();
        ihTable->setRowCount(0);
        QStringList headers;
        headers << "Direction" << "Velocity" << "Time" << "Turn Radius";
        ihTable->setHorizontalHeaderLabels(headers);
        _car.PrintToTable(ui->instructionHistory_tableWidget,0);
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(0,"Save Dialog","","*.json");
    _car.SaveQueueToFile(filePath);
}

void MainWindow::PositionUpdate()
{
    _car.CountPosition(ui->startTimeValue_label);

    QString positionText = QString::number(_car.GetPosition().GetX());
    positionText+=" ; "+QString::number(_car.GetPosition().GetY());
    ui->positionValue_label->setText("("+positionText+")");

    QString velocityText = QString::number(_car.GetVelocity().GetX());
    velocityText+=" ; "+QString::number(_car.GetVelocity().GetY());
    ui->velocityValue_label->setText("("+velocityText+")");

    QStringList headers;
    headers << "Direction" << "Velocity" << "Time" << "Turn Radius";
    auto ciTable=ui->currentInstructions_tableWidget;
    ciTable->clear();
    ciTable->setRowCount(0);
    ciTable->setHorizontalHeaderLabels(headers);

    ui->countPosition_pushButton->setEnabled(false);
}

void MainWindow::UpdateTables()
{
    QStringList headers;
    headers << "Direction" << "Velocity" << "Time" << "Turn Radius";

    auto ciTable=ui->currentInstructions_tableWidget;
    ciTable->clear();
    ciTable->setRowCount(0);
    ciTable->setHorizontalHeaderLabels(headers);

    auto ihTable=ui->instructionHistory_tableWidget;
    ihTable->clear();
    ihTable->setRowCount(0);
    ihTable->setHorizontalHeaderLabels(headers);

    ui->clearInstructions_pushButton->setEnabled(false);
    _car.PrintToTable(ui->instructionHistory_tableWidget,0);
    _car.PrintToTable(ui->currentInstructions_tableWidget,1);
}
