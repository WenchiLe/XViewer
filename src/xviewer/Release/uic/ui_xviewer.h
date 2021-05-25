/********************************************************************************
** Form generated from reading UI file 'xviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XVIEWER_H
#define UI_XVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "xcalendar.h"

QT_BEGIN_NAMESPACE

class Ui_XViewerClass
{
public:
    QWidget *head;
    QWidget *logo;
    QWidget *head_button;
    QPushButton *min;
    QPushButton *max;
    QPushButton *close;
    QPushButton *normal;
    QLabel *status;
    QPushButton *preview;
    QPushButton *playback;
    QWidget *body;
    QWidget *left;
    QListWidget *cam_list;
    QPushButton *add_cam;
    QPushButton *set_cam;
    QPushButton *del_cam;
    QWidget *cams;
    QWidget *playback_wid;
    XCalendar *cal;
    QListWidget *time_list;

    void setupUi(QWidget *XViewerClass)
    {
        if (XViewerClass->objectName().isEmpty())
            XViewerClass->setObjectName(QString::fromUtf8("XViewerClass"));
        XViewerClass->resize(800, 660);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/XViewer/img/webcam_icon_129436.ico"), QSize(), QIcon::Normal, QIcon::Off);
        XViewerClass->setWindowIcon(icon);
        XViewerClass->setStyleSheet(QString::fromUtf8("/* \346\214\211\351\222\256\346\240\267\345\274\217 */\n"
"QPushButton:hover\n"
"\n"
" { \n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 102, 184, 255), stop:0.495 rgba(39, 39, 39, 255), stop:0.505 rgba(39,39, 39, 255), stop:1 rgba(0, 102, 184, 255));\n"
"border: none;\n"
"border-radius:5px;\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";\n"
" }\n"
"QPushButton:!hover\n"
" { \n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 50, 150, 255), stop:0.495 rgba(0, 102, 184, 255), stop:0.505 rgba(0, 102, 184, 255), stop:1 rgba(0, 50, 150, 255));\n"
" border: none;\n"
"border-radius:5px;\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";\n"
" }\n"
"\n"
"\n"
"\n"
"/* \351\241\266\351\203\250\346\240\267\345\274\217 */\n"
"#XViewerClass {\n"
"    background-color: #333333;\n"
"    border-radius: 0px;\n"
"}\n"
"#head\n"
"{\n"
"	background-color:#3c3c3c;\n"
"}\n"
"\n"
"#logo\n"
"{\n"
""
                        "	border-image: url(:/XViewer/img/webcam_icon_129436.png);\n"
"}\n"
"\n"
"#close\n"
"{\n"
"	border-image: url(:/XViewer/img/application_exit_icon_181084.png);\n"
"}\n"
"\n"
"#max\n"
"{\n"
"	border-image: url(:/XViewer/img/window_maximize_icon_181228.png);\n"
"}\n"
"\n"
"#min\n"
"{\n"
"	border-image: url(:/XViewer/img/window_minimize_icon_181229.png);\n"
"}\n"
"\n"
"#normal\n"
"{\n"
"	\n"
"	border-image: url(:/XViewer/img/window_restore_icon_181231.png);\n"
"}\n"
"\n"
"/* body \346\240\267\345\274\217 */\n"
"#body\n"
"{\n"
"	background-color: #212121;\n"
"}\n"
"\n"
"#left\n"
"{\n"
"	background-color: #252525;\n"
"}\n"
"\n"
"#cams\n"
"{\n"
"	background-color: #1e1e1e;\n"
"}\n"
"\n"
"#status\n"
"{\n"
"	color: rgb(255, 255, 255);\n"
"	font: 75 9pt \"Adobe Arabic\";\n"
"}\n"
"\n"
"/*tab \350\217\234\345\215\225\346\214\211\351\222\256\345\210\207\346\215\242\346\240\267\345\274\217 */\n"
"\n"
"#preview\n"
"{\n"
"background-color: rgb(50, 50, 50);\n"
"}\n"
"#playback\n"
"{\n"
"background-color: rgb(50, 50, 50);\n"
"}"
                        "\n"
"\n"
"#preview:checked\n"
"{\n"
"background-color: rgb(0, 102, 184);\n"
"}\n"
"#playback:checked\n"
"{\n"
"background-color: rgb(0, 102, 184);\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
""));
        head = new QWidget(XViewerClass);
        head->setObjectName(QString::fromUtf8("head"));
        head->setGeometry(QRect(0, 0, 800, 60));
        head->setMaximumSize(QSize(16777215, 60));
        logo = new QWidget(head);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(0, 0, 60, 60));
        head_button = new QWidget(head);
        head_button->setObjectName(QString::fromUtf8("head_button"));
        head_button->setGeometry(QRect(650, 0, 150, 60));
        head_button->setMaximumSize(QSize(150, 16777215));
        min = new QPushButton(head_button);
        min->setObjectName(QString::fromUtf8("min"));
        min->setGeometry(QRect(27, 20, 20, 20));
        min->setFlat(true);
        max = new QPushButton(head_button);
        max->setObjectName(QString::fromUtf8("max"));
        max->setEnabled(true);
        max->setGeometry(QRect(65, 20, 20, 20));
        max->setFlat(true);
        close = new QPushButton(head_button);
        close->setObjectName(QString::fromUtf8("close"));
        close->setGeometry(QRect(101, 20, 20, 20));
        close->setFlat(true);
        normal = new QPushButton(head_button);
        normal->setObjectName(QString::fromUtf8("normal"));
        normal->setEnabled(true);
        normal->setGeometry(QRect(65, 20, 20, 20));
        normal->setFlat(true);
        normal->raise();
        min->raise();
        max->raise();
        close->raise();
        status = new QLabel(head);
        status->setObjectName(QString::fromUtf8("status"));
        status->setGeometry(QRect(440, 20, 251, 31));
        preview = new QPushButton(head);
        preview->setObjectName(QString::fromUtf8("preview"));
        preview->setGeometry(QRect(210, 30, 90, 30));
        preview->setCheckable(true);
        preview->setChecked(true);
        preview->setAutoExclusive(true);
        playback = new QPushButton(head);
        playback->setObjectName(QString::fromUtf8("playback"));
        playback->setGeometry(QRect(310, 30, 90, 30));
        playback->setCheckable(true);
        playback->setAutoExclusive(true);
        head_button->raise();
        logo->raise();
        status->raise();
        preview->raise();
        playback->raise();
        body = new QWidget(XViewerClass);
        body->setObjectName(QString::fromUtf8("body"));
        body->setGeometry(QRect(0, 70, 800, 600));
        left = new QWidget(body);
        left->setObjectName(QString::fromUtf8("left"));
        left->setGeometry(QRect(10, 10, 200, 600));
        left->setMaximumSize(QSize(200, 16777215));
        cam_list = new QListWidget(left);
        cam_list->setObjectName(QString::fromUtf8("cam_list"));
        cam_list->setGeometry(QRect(0, 40, 200, 1000));
        cam_list->setDragEnabled(true);
        cam_list->setIconSize(QSize(50, 50));
        add_cam = new QPushButton(left);
        add_cam->setObjectName(QString::fromUtf8("add_cam"));
        add_cam->setGeometry(QRect(5, 5, 50, 30));
        set_cam = new QPushButton(left);
        set_cam->setObjectName(QString::fromUtf8("set_cam"));
        set_cam->setGeometry(QRect(75, 5, 50, 30));
        del_cam = new QPushButton(left);
        del_cam->setObjectName(QString::fromUtf8("del_cam"));
        del_cam->setGeometry(QRect(145, 5, 50, 30));
        cams = new QWidget(body);
        cams->setObjectName(QString::fromUtf8("cams"));
        cams->setGeometry(QRect(220, 10, 411, 411));
        playback_wid = new QWidget(body);
        playback_wid->setObjectName(QString::fromUtf8("playback_wid"));
        playback_wid->setGeometry(QRect(209, 10, 600, 800));
        cal = new XCalendar(playback_wid);
        cal->setObjectName(QString::fromUtf8("cal"));
        cal->setGeometry(QRect(160, 0, 400, 300));
        time_list = new QListWidget(playback_wid);
        time_list->setObjectName(QString::fromUtf8("time_list"));
        time_list->setGeometry(QRect(0, 0, 150, 800));
        time_list->raise();
        cal->raise();

        retranslateUi(XViewerClass);
        QObject::connect(close, SIGNAL(clicked()), XViewerClass, SLOT(close()));
        QObject::connect(min, SIGNAL(clicked()), XViewerClass, SLOT(showMinimized()));
        QObject::connect(normal, SIGNAL(clicked()), XViewerClass, SLOT(NormalWindow()));
        QObject::connect(max, SIGNAL(clicked()), XViewerClass, SLOT(MaxWindow()));
        QObject::connect(add_cam, SIGNAL(clicked()), XViewerClass, SLOT(AddCam()));
        QObject::connect(set_cam, SIGNAL(clicked()), XViewerClass, SLOT(SetCam()));
        QObject::connect(del_cam, SIGNAL(clicked()), XViewerClass, SLOT(DelCam()));
        QObject::connect(preview, SIGNAL(clicked()), XViewerClass, SLOT(Preview()));
        QObject::connect(playback, SIGNAL(clicked()), XViewerClass, SLOT(Playback()));
        QObject::connect(cam_list, SIGNAL(clicked(QModelIndex)), XViewerClass, SLOT(SelectCamera(QModelIndex)));
        QObject::connect(cal, SIGNAL(clicked(QDate)), XViewerClass, SLOT(SelectDate(QDate)));
        QObject::connect(time_list, SIGNAL(activated(QModelIndex)), XViewerClass, SLOT(PlayVideo(QModelIndex)));

        QMetaObject::connectSlotsByName(XViewerClass);
    } // setupUi

    void retranslateUi(QWidget *XViewerClass)
    {
        XViewerClass->setWindowTitle(QApplication::translate("XViewerClass", "XViewer", nullptr));
        min->setText(QString());
        max->setText(QString());
        close->setText(QString());
        normal->setText(QString());
        status->setText(QApplication::translate("XViewerClass", "\347\233\221\346\216\247\344\270\255\343\200\202\343\200\202\343\200\202", nullptr));
        preview->setText(QApplication::translate("XViewerClass", "\351\242\204\350\247\210", nullptr));
        playback->setText(QApplication::translate("XViewerClass", "\345\233\236\346\224\276", nullptr));
        add_cam->setText(QApplication::translate("XViewerClass", "\346\226\260\345\242\236", nullptr));
        set_cam->setText(QApplication::translate("XViewerClass", "\344\277\256\346\224\271", nullptr));
        del_cam->setText(QApplication::translate("XViewerClass", "\345\210\240\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XViewerClass: public Ui_XViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XVIEWER_H
