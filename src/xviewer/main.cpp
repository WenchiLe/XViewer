#include "xviewer.h"
#include <QtWidgets/QApplication>
#include "xcamera_config.h"
#include <QDebug>
#include <QDir>
#include "xcamera_record.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XViewer w;
    w.show();
    return a.exec();
}
