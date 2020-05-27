#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include "rasterizer.h"
#include "imagewidget.h"
#include <QImage>
#include <QFileDialog>
#include <QAction>
#include <QString>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Rasterizer r;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changeEvent(QEvent *event);
signals:
    void startDisplay(QImage);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
