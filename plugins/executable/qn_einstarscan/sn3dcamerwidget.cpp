#include "sn3dcamerwidget.h"
#include "ui_sn3dcamerwidget.h"

Sn3DCamerWidget::Sn3DCamerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sn3DCamerWidget)
{
    ui->setupUi(this);    
}

Sn3DCamerWidget::~Sn3DCamerWidget()
{
    delete ui;
}

void Sn3DCamerWidget::updateImage(QPixmap&pixmap)
{
	ui->widget_back->setMaximumHeight(0);
	ui->label_img->setPixmap(pixmap);
	ui->label_img->setMinimumHeight(this->height());
	ui->label_img->setMaximumHeight(this->height());
}
