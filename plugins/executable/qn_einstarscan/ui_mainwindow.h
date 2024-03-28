/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextBrowser>
#include "sn3dcamerwidget.h"

#include <QQuickWidget>
#include "lighthub.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton_Start;
    QPushButton *pushButton_End;
    QWidget *widget;
	QTextBrowser *textBrowser;
	QHBoxLayout *horizontalLayout;
    Sn3DCamerWidget *camera_left;
    Sn3DCamerWidget *camera_right;
    Sn3DCamerWidget *camera_color;
    QMenuBar *menubar;
    QStatusBar *statusbar;
	LightHub *hub;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1258, /*597*/900);

        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton_Start = new QPushButton(centralwidget);
        pushButton_Start->setObjectName(QString::fromUtf8("pushButton_Start"));
        pushButton_Start->setGeometry(QRect(420, 30, 75, 23));
        pushButton_End = new QPushButton(centralwidget);
        pushButton_End->setObjectName(QString::fromUtf8("pushButton_End"));
        pushButton_End->setGeometry(QRect(560, 30, 75, 23));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 80, 1251, 461));

		textBrowser = new QTextBrowser(centralwidget);
		textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
		QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
		sizePolicy3.setHorizontalStretch(0);
		sizePolicy3.setVerticalStretch(0);
		sizePolicy3.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
		textBrowser->setSizePolicy(sizePolicy3);
		textBrowser->setStyleSheet(QString::fromUtf8("background-color: rgb(80, 80, 80);\n"
			"border-color: rgb(80, 80, 80);"));
		textBrowser->setFrameShape(QFrame::NoFrame);
		textBrowser->setFrameShadow(QFrame::Plain);
		textBrowser->setLineWidth(0);
		textBrowser->setGeometry(QRect(0, 560, 1251, 400));

		QQuickWidget *view = new QQuickWidget(widget);
		hub = new LightHub();
		view->setAttribute(Qt::WA_AlwaysShowToolTips);
		view->setClearColor(Qt::transparent);
		view->engine()->rootContext()->setContextProperty("$hub", hub);
		view->setSource(QUrl(QStringLiteral("qrc:/Canvas.qml")));
		view->show();

        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        camera_left = new Sn3DCamerWidget(widget);
        camera_left->setObjectName(QString::fromUtf8("camera_left"));
		camera_left->setVisible(false);
        horizontalLayout->addWidget(camera_left);

        camera_right = new Sn3DCamerWidget(widget);
        camera_right->setObjectName(QString::fromUtf8("camera_right"));
		camera_right->setVisible(false);
        horizontalLayout->addWidget(camera_right);

        camera_color = new Sn3DCamerWidget(widget);
        camera_color->setObjectName(QString::fromUtf8("camera_color"));

        //horizontalLayout->addWidget(camera_color);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1258, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_Start->setText(QApplication::translate("MainWindow", "start", nullptr));
        pushButton_End->setText(QApplication::translate("MainWindow", "end", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
