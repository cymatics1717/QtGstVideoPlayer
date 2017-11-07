#ifndef GSTMEDIASOURCE_H
#define GSTMEDIASOURCE_H

#include <QImage>
#include <QObject>
#include <QPixmap>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

class gstMediaSource : public QObject
{
    Q_OBJECT
public:
    explicit gstMediaSource(QObject *parent = nullptr);
    ~gstMediaSource();
    int init(QString cmd, int argc, char *argv[],bool atonce = true);
    void run();
    int deInit();

signals:
    void incoming(QPixmap image);
public slots:
    void getFrame();
private:
    QString cmd;
    GstState status;
    GstElement *pipeline;
    GstAppSink *appsink;
};

#endif // GSTMEDIASOURCE_H
