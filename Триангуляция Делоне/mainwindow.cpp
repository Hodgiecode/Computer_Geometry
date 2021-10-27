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

