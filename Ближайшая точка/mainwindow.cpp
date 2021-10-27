#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->draw_widget->SetTaskPtr(&current_task);

    if (ui->draw_widget->first_algo_btn_clicked == 0 && ui->draw_widget->second_algo_btn_clicked==0){
        ui->draw_widget->SetDefaultPoints();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_1_clicked()
{
    // инициализация задачи
    ui->draw_widget->RunFirstAlgo();
    QString v1 = QString::fromUtf8(ui->draw_widget->time_algo_one.c_str());
    QString v2 = QString::fromUtf8(ui->draw_widget->dist_algo_one.c_str());

    ui->lineEdit->setText(v1);
    ui->lineEdit_2->setText(v2);
}

void MainWindow::on_pushButton_2_clicked()
{
    // инициализация задачи
    ui->draw_widget->RunSecondAlgo();
    QString v1 = QString::fromUtf8(ui->draw_widget->time_algo_two.c_str());
    QString v2 = QString::fromUtf8(ui->draw_widget->dist_algo_two.c_str());

    ui->lineEdit_3->setText(v1);
    ui->lineEdit_4->setText(v2);

}


void MainWindow::on_pushButton_3_clicked()
{
    // инициализация задачи
    ui->draw_widget->first_algo_btn_clicked = 0;
    ui->draw_widget->second_algo_btn_clicked = 0;
    ui->draw_widget->Clear();
    ui->draw_widget->SetDefaultPoints();
    ui->draw_widget->repaint();
}
