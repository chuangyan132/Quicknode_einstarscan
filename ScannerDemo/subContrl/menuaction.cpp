#include "menuaction.h"
#include "ui_menuaction.h"
#include "qdebug.h"

menuAction::menuAction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menuAction)
{
    ui->setupUi(this);
    connect(ui->slider_Image, &QSlider::valueChanged, [this](int iValue){
        ui->spin_Image->setValue(iValue);
        emit sigImageBrightChanged(iValue);
        qDebug() << "slider changed";
    });
    connect(ui->spin_Image, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int iValue){
        ui->slider_Image->setValue(iValue);
        qDebug() << "spin changed";
    });
}

menuAction::~menuAction()
{
    delete ui;
}

void menuAction::on_rb_normal_clicked(bool checked)
{
	if (checked)
	{
		emit sigLaserObjectType(0);
	}
}

void menuAction::on_rb_dark_clicked(bool checked)
{
	if (checked)
	{
		emit sigLaserObjectType(1);
	}
}

void menuAction::on_rb_ref_clicked(bool checked)
{
	if (checked)
	{
		emit sigLaserObjectType(2);
	}
}

void menuAction::on_hs_laser_valueChanged(int value)
{
	emit sigLaserChanged(value);
}

void menuAction::on_hs_led_valueChanged(int value)
{
	emit sigLEDChanged(value);
}

void menuAction::on_hs_laserquality_valueChanged(int value)
{
	emit sigLaserQualityChanged(value);
}

void menuAction::setImageEnabled(bool enabled)
{
    ui->slider_Image->setEnabled(enabled);
    ui->spin_Image->setEnabled(enabled);
}
