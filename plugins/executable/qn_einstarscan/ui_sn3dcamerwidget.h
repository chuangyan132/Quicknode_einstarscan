/********************************************************************************
** Form generated from reading UI file 'sn3dcamerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SN3DCAMERWIDGET_H
#define UI_SN3DCAMERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sn3DCamerWidget
{
public:
    QVBoxLayout *verticalLayout_3;
    QLabel *label_img;
    QWidget *widget_back;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *labelImage;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *Sn3DCamerWidget)
    {
        if (Sn3DCamerWidget->objectName().isEmpty())
            Sn3DCamerWidget->setObjectName(QString::fromUtf8("Sn3DCamerWidget"));
        Sn3DCamerWidget->resize(620, 420);
        Sn3DCamerWidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_3 = new QVBoxLayout(Sn3DCamerWidget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_img = new QLabel(Sn3DCamerWidget);
        label_img->setObjectName(QString::fromUtf8("label_img"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_img->sizePolicy().hasHeightForWidth());
        label_img->setSizePolicy(sizePolicy);
        label_img->setMaximumSize(QSize(9999, 0));

        verticalLayout_3->addWidget(label_img);

        widget_back = new QWidget(Sn3DCamerWidget);
        widget_back->setObjectName(QString::fromUtf8("widget_back"));
        widget_back->setStyleSheet(QString::fromUtf8("#widget_back{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(8, 8, 8, 255), stop:1 rgba(32, 32, 32, 90));\n"
" border-width: 1px;\n"
"     border-style: solid;\n"
"     border-color: #434343;\n"
"\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(widget_back);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(193, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelImage = new QLabel(widget_back);
        labelImage->setObjectName(QString::fromUtf8("labelImage"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelImage->sizePolicy().hasHeightForWidth());
        labelImage->setSizePolicy(sizePolicy1);
        labelImage->setMinimumSize(QSize(100, 100));
        labelImage->setMaximumSize(QSize(100, 100));
        labelImage->setStyleSheet(QString::fromUtf8("border-image: url(:/images/notconnect_normal.png);"));
        labelImage->setScaledContents(false);

        horizontalLayout->addWidget(labelImage);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(widget_back);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(16777215, 50));
        label_2->setSizeIncrement(QSize(0, 50));
        label_2->setBaseSize(QSize(0, 50));
        QFont font;
        font.setPointSize(20);
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(231, 231, 231);"));

        verticalLayout->addWidget(label_2);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalSpacer_4 = new QSpacerItem(193, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout_3->addWidget(widget_back);


        retranslateUi(Sn3DCamerWidget);

        QMetaObject::connectSlotsByName(Sn3DCamerWidget);
    } // setupUi

    void retranslateUi(QWidget *Sn3DCamerWidget)
    {
        Sn3DCamerWidget->setWindowTitle(QApplication::translate("Sn3DCamerWidget", "Sn3DCamerWidget", nullptr));
        label_img->setText(QApplication::translate("Sn3DCamerWidget", "TextLabel", nullptr));
        labelImage->setText(QString());
        label_2->setText(QApplication::translate("Sn3DCamerWidget", "Not connected", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Sn3DCamerWidget: public Ui_Sn3DCamerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SN3DCAMERWIDGET_H
