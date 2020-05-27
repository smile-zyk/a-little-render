#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QPainter>
namespace Ui {
class imageWidget;
}

class imageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit imageWidget(QWidget *parent = nullptr);
    ~imageWidget();
    void DisplayResult(QImage);
    void paintEvent(QPaintEvent *event);
private:
    Ui::imageWidget *ui;
    QImage img;
};

#endif // IMAGEWIDGET_H
