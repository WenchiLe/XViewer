/********************************************************************************
** Form generated from reading UI file 'xplayvideo.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XPLAYVIDEO_H
#define UI_XPLAYVIDEO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XPlayVideo
{
public:
    QWidget *video;
    QLabel *label;
    QSlider *speed;
    QLabel *speedtxt;
    QSlider *pos;
    QPushButton *play;

    void setupUi(QWidget *XPlayVideo)
    {
        if (XPlayVideo->objectName().isEmpty())
            XPlayVideo->setObjectName(QString::fromUtf8("XPlayVideo"));
        XPlayVideo->resize(600, 510);
        XPlayVideo->setStyleSheet(QString::fromUtf8("#play\n"
"{\n"
"	border-image: url(:/XViewer/img/pausebutton_120631.png);\n"
"}"));
        video = new QWidget(XPlayVideo);
        video->setObjectName(QString::fromUtf8("video"));
        video->setGeometry(QRect(0, 30, 600, 400));
        label = new QLabel(XPlayVideo);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 5, 60, 20));
        speed = new QSlider(XPlayVideo);
        speed->setObjectName(QString::fromUtf8("speed"));
        speed->setGeometry(QRect(60, 5, 100, 20));
        speed->setMinimum(1);
        speed->setMaximum(20);
        speed->setPageStep(2);
        speed->setValue(10);
        speed->setOrientation(Qt::Horizontal);
        speedtxt = new QLabel(XPlayVideo);
        speedtxt->setObjectName(QString::fromUtf8("speedtxt"));
        speedtxt->setGeometry(QRect(170, 5, 16, 16));
        pos = new QSlider(XPlayVideo);
        pos->setObjectName(QString::fromUtf8("pos"));
        pos->setGeometry(QRect(0, 440, 600, 20));
        pos->setMinimum(1);
        pos->setMaximum(999);
        pos->setPageStep(100);
        pos->setOrientation(Qt::Horizontal);
        play = new QPushButton(XPlayVideo);
        play->setObjectName(QString::fromUtf8("play"));
        play->setGeometry(QRect(280, 465, 40, 40));

        retranslateUi(XPlayVideo);
        QObject::connect(speed, SIGNAL(sliderReleased()), XPlayVideo, SLOT(SetSpeed()));
        QObject::connect(pos, SIGNAL(sliderReleased()), XPlayVideo, SLOT(PlayPos()));
        QObject::connect(play, SIGNAL(clicked()), XPlayVideo, SLOT(Pause()));
        QObject::connect(pos, SIGNAL(sliderMoved(int)), XPlayVideo, SLOT(Move()));

        QMetaObject::connectSlotsByName(XPlayVideo);
    } // setupUi

    void retranslateUi(QWidget *XPlayVideo)
    {
        XPlayVideo->setWindowTitle(QApplication::translate("XPlayVideo", "XPlayVideo", nullptr));
        label->setText(QApplication::translate("XPlayVideo", "\346\222\255\346\224\276\351\200\237\345\272\246", nullptr));
        speedtxt->setText(QApplication::translate("XPlayVideo", "1", nullptr));
        play->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class XPlayVideo: public Ui_XPlayVideo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XPLAYVIDEO_H
