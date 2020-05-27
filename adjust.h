#ifndef ADJUST_H
#define ADJUST_H

#include <QWidget>

namespace Ui {
class Adjust;
}

class Adjust : public QWidget
{
    Q_OBJECT

public:
    explicit Adjust(QWidget *parent = nullptr);
    ~Adjust();
    int Get_X_Value();
    int Get_Y_Value();
    int Get_Z_Value();

private:
    Ui::Adjust *ui;
};

#endif // ADJUST_H
