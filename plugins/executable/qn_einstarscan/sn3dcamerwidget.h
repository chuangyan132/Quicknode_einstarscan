#ifndef SN3DCAMERWIDGET_H
#define SN3DCAMERWIDGET_H

#include <QWidget>
#include <QPixmap>
namespace Ui {
class Sn3DCamerWidget;
}

class Sn3DCamerWidget : public QWidget
{
    Q_OBJECT

public:
    Sn3DCamerWidget(QWidget *parent = 0);
    ~Sn3DCamerWidget();
	void updateImage(QPixmap&pixmap);

private:
    Ui::Sn3DCamerWidget *ui;
};

#endif // SN3DCAMERWIDGET_H
