#ifndef WIDGETCONTENT_H
#define WIDGETCONTENT_H

#include <QWidget>

namespace Ui {
class WidgetContent;
}

class WidgetContent : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetContent(QWidget *parent = 0);
    ~WidgetContent();

private:
    Ui::WidgetContent *ui;
};

#endif // WIDGETCONTENT_H
