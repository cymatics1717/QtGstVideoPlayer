#ifndef GSTMEDIASOURCE_H
#define GSTMEDIASOURCE_H

#include <QImage>
#include <QObject>
#include <QPixmap>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <QDebug>
#include <QQuickImageProvider>

class myImageProvider : public QQuickImageProvider
{
public:
    myImageProvider():QQuickImageProvider(QQuickImageProvider::Pixmap){}

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize){
        return img;
    }
    QPixmap img;
};

class gstMediaSource : public QObject
{
    Q_OBJECT
public:
    explicit gstMediaSource(QObject *parent = nullptr);
    ~gstMediaSource();
    int init(QString cmd, int argc, char *argv[],bool atonce = true);
    void run();
    int deInit();

    myImageProvider *imageProvider;
signals:
//    void incoming();
    void incoming(QPixmap image);
//    void endOfStream();
public slots:
    void test(double fromQML);
    void getFrame();
    void endOfStream();

private:
    QString cmd;
    GstState status;
    GstElement *pipeline;
    GstAppSink *appsink;

};

#endif // GSTMEDIASOURCE_H
