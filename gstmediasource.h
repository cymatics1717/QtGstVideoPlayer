#ifndef GSTMEDIASOURCE_H
#define GSTMEDIASOURCE_H

#include <QImage>
#include <QObject>
#include <QPixmap>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

#include <QQuickImageProvider>

class myImageProvider : public QQuickImageProvider
{
public:
    myImageProvider(ImageType type, Flags flags = Flags()):QQuickImageProvider(type,flags){}
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize){
        return img;
    }
    QImage img;
};

class gstMediaSource : public QObject
{
    Q_OBJECT
public:
    explicit gstMediaSource(QObject *parent = nullptr);
    ~gstMediaSource();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    int init(QString cmd, int argc, char *argv[],bool atonce = true);
    void run();
    int deInit();

    myImageProvider *imageProvider;

signals:
//    void incoming();
    void incoming(QPixmap image);
//    void endOfStream();
public slots:
    void getFrame();
    void endOfStream();

private:
    QString cmd;
    GstState status;
    GstElement *pipeline;
    GstAppSink *appsink;

};

#endif // GSTMEDIASOURCE_H
