#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>


MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),source(new gstMediaSource(this))
{
    ui->setupUi(this);
    setCentralWidget(ui->label);
    installEventFilter(this);

//#define CAPS "video/x-raw,width=640,pixel-aspect-ratio=1/1"
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


//    source->init(cmd,argc,argv,false);
    source->init(cmd,argc,argv);
    statusBar()->showMessage(cmd);
    connect(source,SIGNAL(incoming(QPixmap)),ui->label,SLOT(setPixmap(QPixmap)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::KeyPress){
        QKeyEvent *event = static_cast<QKeyEvent *>(e);
        if(event){
//            qDebug() << event->text();
//            return true;
            switch (event->key()) {
            case Qt::Key_Q:
                qApp->quit();
                break;
            case Qt::Key_Return:
            {
                static int cnt =0;
                if(++cnt%2==1){
                    showFullScreen();
                } else {
                    showNormal();
                }
            }
                break;
            default:
                break;
            }
        }
    } else if(e->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *event = static_cast<QMouseEvent *>(e);
        if(event/*&&event->button()==Qt::RightButton*/){
            source->run();
        }
    }

    return QObject::eventFilter(obj,e);
}

