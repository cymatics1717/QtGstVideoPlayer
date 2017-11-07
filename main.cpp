#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

/*
gst-launch v4l2src ! decodebin ! clockoverlay halignment=right valignment=bottom shaded-background=true time-format="%Y-%m-%d %H:%M:%S" ! queue ! videoconvert ! jpegenc ! queue ! multipartmux ! udpsink host=127.0.0.1 port=8554



*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argc,argv);
    w.show();

    return a.exec();
}
