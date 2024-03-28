#ifndef MENUACTION_H
#define MENUACTION_H

#include <QWidget>

namespace Ui {
class menuAction;
}

class menuAction : public QWidget
{
    Q_OBJECT

public:
    menuAction(QWidget *parent = 0);
    ~menuAction();
    void setImageEnabled(bool enabled);
public slots:
	void on_rb_normal_clicked(bool checked);
	void on_rb_dark_clicked(bool checked);
	void on_rb_ref_clicked(bool checked);

	void on_hs_laser_valueChanged(int value);
	void on_hs_led_valueChanged(int value);
	void on_hs_laserquality_valueChanged(int value);

signals:
    void sigImageBrightChanged(int iValue);
	void sigLaserChanged(int iValue);
	void sigLEDChanged(int iValue);
	void sigLaserQualityChanged(int iValue);
	void sigLaserObjectType(int objType);
private:
    Ui::menuAction *ui;
};

#endif // MENUACTION_H
