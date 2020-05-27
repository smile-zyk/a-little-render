#include "adjust.h"
#include "ui_adjust.h"

Adjust::Adjust(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Adjust)
{
    ui->setupUi(this);
    void (QSpinBox:: *SpinBoxValueChanged)(int)=&QSpinBox::valueChanged;
    connect(ui->X_spinBox,SpinBoxValueChanged,ui->X_horizontalSlider,&QSlider::setValue);
    connect(ui->Y_spinBox,SpinBoxValueChanged,ui->Y_horizontalSlider,&QSlider::setValue);
    connect(ui->Z_spinBox,SpinBoxValueChanged,ui->Z_horizontalSlider,&QSlider::setValue);
    connect(ui->X_horizontalSlider,&QSlider::valueChanged,ui->X_spinBox,&QSpinBox::setValue);
    connect(ui->Y_horizontalSlider,&QSlider::valueChanged,ui->Y_spinBox,&QSpinBox::setValue);
    connect(ui->Z_horizontalSlider,&QSlider::valueChanged,ui->Z_spinBox,&QSpinBox::setValue);
}

Adjust::~Adjust()
{
    delete ui;
}

int Adjust::Get_X_Value()
{
    return ui->X_spinBox->value();
}

int Adjust::Get_Y_Value()
{
    return ui->Y_spinBox->value();
}

int Adjust::Get_Z_Value()
{
    return ui->Z_spinBox->value();
}

