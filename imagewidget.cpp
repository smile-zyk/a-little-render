#include "imagewidget.h"
#include "ui_imagewidget.h"

imageWidget::imageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageWidget)
{
    ui->setupUi(this);
}

void imageWidget::DisplayResult(QImage m)
{
    img=m;
    update();
}

void imageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0,0,img);
}

imageWidget::~imageWidget()
{
    delete ui;
}
