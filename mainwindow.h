#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "instruction.h"
#include "velocityinstruction.h"
#include "turninstruction.h"
#include <QMainWindow>
#include <QTableWidget>
#include <iostream>
#include "car.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_velocityX_lineEdit_textChanged(const QString &arg1);

    void on_velocityY_lineEdit_textChanged(const QString &arg1);

    void on_turnRadius_lineEdit_textChanged(const QString &arg1);

    void on_direction_comboBox_currentIndexChanged(const QString &arg1);

    void on_addInstruction_pushButton_clicked();

    void on_countPosition_pushButton_clicked();

    void on_clearInstructions_pushButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

private:
    Car _car;
    Ui::MainWindow *ui;
    void CheckInputFields();
    void CreateInstruction(bool& success);
    void AddEntry(QTableWidget* table);
    void PositionUpdate();
    void UpdateTables();
};
#endif // MAINWINDOW_H
