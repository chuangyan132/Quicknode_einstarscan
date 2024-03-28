#include "sn3dstyle.h"
#include "qapplication.h"

Sn3DStyle::Sn3DStyle(QObject* parent)
    :QObject(parent)
{
}


Sn3DStyle::~Sn3DStyle()
{
}

void Sn3DStyle::setStyleSheet()
{
    QString styleSheet =
        "QPushButton"
        "{"
        "color: rgb(255, 255, 255);"
        "padding :6 6 6 6;"
        "border-image: url(:/images/btn_leftnormal.png) 0  354 0  0 ;"
        "}"
        "QPushButton:hover"
        "{"
        "border-image: url(:/images/btn_leftnormal.png) 0  236 0  118;"
        "}"
        "QPushButton:pressed"
        "{"
        "border-image: url(:/images/btn_leftnormal.png) 0 118 0 236;"
        "}"
        "QPushButton:disabled"
        "{"
        "border-image: url(:/images/btn_leftnormal.png) 0 0 0 354;"
        "}"
        "QFrame"
        "{"
        "color:rgb(255, 255, 255);"
        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(20, 20, 20, 255), stop:1 rgba(24, 24, 24, 255));"
        "}"
		"QRadioButton{"
	"color:#9b9b9b;"
	"}"
		"QRadioButton::indicator:unchecked{"
	"color: #ffffff;"
		"border-image:url(:/images/ridio_norml.png);"
	"}"
		"QRadioButton::indicator:checked{"
	"color:#ffffff;"
		"border-image:url(:/images/rido_checked.png);"
	"}"
        "QCheckBox"
        "{"
        "color:rgb(255, 255, 255);"
        "}"
        ;
    qApp->setStyleSheet(styleSheet);
}
