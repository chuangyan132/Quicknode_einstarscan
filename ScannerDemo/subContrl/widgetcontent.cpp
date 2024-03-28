#include "widgetcontent.h"
#include "ui_widgetcontent.h"

WidgetContent::WidgetContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetContent)
{
    ui->setupUi(this);
}

WidgetContent::~WidgetContent()
{
    delete ui;
}
