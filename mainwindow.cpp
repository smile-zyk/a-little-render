#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),r(700,700)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->image_widget->setFixedSize(700,700);
    ui->tabWidget->setFixedSize(500,700);
    connect(this,&MainWindow::startDisplay,ui->image_widget,&imageWidget::DisplayResult);
    connect(ui->actionOpen,&QAction::triggered,[=]()
    {
       QString path=QFileDialog::getOpenFileName(this,"打开模型文件","/","(*.obj)");
       r.SetModelPath(path.toStdString());
       r.Render();
       emit startDisplay(r.img);
    });
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange)
    {              if(!isMinimized())
        {
            setAttribute(Qt::WA_Mapped);
        }
    }          QWidget::changeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

