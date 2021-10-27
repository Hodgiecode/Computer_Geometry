#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_pushButton_clicked(){
    ui->draw_widget->LagrangeSpline();
}

void MainWindow::on_pushButton_3_clicked(){
    ui->draw_widget->CubicSpline();
}

void MainWindow::on_pushButton_2_clicked(){
    ui->draw_widget->HermiteSpline();
}
