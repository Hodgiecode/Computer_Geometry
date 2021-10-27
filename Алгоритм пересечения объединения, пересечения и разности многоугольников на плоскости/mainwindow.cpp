#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawwidget.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->draw_widget->SetTaskPtr(&current_task);
    ui->draw_widget->SetDefaultPoints();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_1_clicked()
{
    // инициализация задачи
    ui->draw_widget->Difference1();
}

void MainWindow::on_pushButton_2_clicked()
{
    // инициализация задачи
    ui->draw_widget->Difference2();
}

void MainWindow::on_pushButton_clicked()
{
    // инициализация задачи
    ui->draw_widget->Intersection();
}

void MainWindow::on_pushButton_3_clicked()
{
    // инициализация задачи
    ui->draw_widget->Union();
}

