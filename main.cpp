#include "gstmediasource.h"
#include "mainwindow.h"
#include <QStatusBar>
#include <QApplication>
#include <QtQuick>
#include <QDebug>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#define CAPS "video/x-raw,format=RGB"
//    QString cmd = "udpsrc port=8554 ! "
//                  "jpegparse ! "
//                  "jpegdec ! "
//                  "videoconvert  ! "
////                  "videoscale ! "
//            "clockoverlay halignment=right valignment=top shaded-background=true time-format=\"%Y-%m-%d %H:%M:%S\"! "
//            "appsink name=sink sync=true caps=\"" CAPS "\"";

    QString cmd = "udpsrc port=8554 caps=\"application/x-rtp,encoding-name=JPEG,payload=26\" ! "
                  "rtpjpegdepay ! jpegparse ! queue ! jpegdec !  videoconvert  ! "
                  "clockoverlay halignment=right valignment=top shaded-background=true time-format=\"%Y-%m-%d %H:%M:%S\"! "
                  "appsink name=sink sync=true caps=\"" CAPS "\"";

//    QString cmd = "tcpclientsrc host=10.111.161.43 port=8899 ! "
////                    "jpegparse ! "
//                    "jpegdec ! "
//                    "videoconvert  ! "
////                    "videoscale ! "
//                    "appsink name=sink sync=true caps=\"" CAPS "\"";


//        QString cmd =
//            "uridecodebin uri=file:///home/wayne/Documents/sintel_trailer-480p.webm ! "
////          "uridecodebin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm ! "
//            "videoconvert ! "
//            "videoscale ! "
//            "appsink name=sink   sync=true caps=\"" CAPS "\"";


    gstMediaSource source;
    source.init(cmd,argc,argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("source",&source);
    engine.addImageProvider("pool",source.imageProvider);
    engine.load(QUrl("qrc:/qgstvideo.qml"));

//    MainWindow w(argc,argv);
//    w.statusBar()->showMessage(cmd);
//    QObject::connect(&source,SIGNAL(incoming(QPixmap)),w.label(),SLOT(setPixmap(QPixmap)));
//    w.show();

    return a.exec();
}
