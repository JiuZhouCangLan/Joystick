#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->joyStick, &JoyStick::posChanged, this, [this](QPoint pos) {
        QString str("X: %1\nY: %2");
        ui->label->setText(str.arg(pos.x()).arg(pos.y()));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

