#include "gstmediasource.h"

#include <opencv2/opencv.hpp>
#include <QDebug>
gstMediaSource::gstMediaSource(QObject *parent) : QObject(parent),
    status(GST_STATE_NULL),pipeline(NULL),appsink(NULL),imageProvider(new myImageProvider)
{

}

gstMediaSource::~gstMediaSource()
{
    deInit();
}

static GstFlowReturn new_preroll(GstAppSink *appsink, gpointer data)
{
    g_print ("Got preroll!\n");
    //    gst_app_sink_pull_preroll(appsink);

    return GST_FLOW_OK;
}

static GstFlowReturn new_sample(GstAppSink *appsink, gpointer data)
{
//    g_print("%s:%d\n",__FUNCTION__,__LINE__);
    static int framecount = 0;
    framecount++;

    static int width=0, height=0 ;


    GstSample *sample = gst_app_sink_pull_sample(appsink);
    GstCaps *caps = gst_sample_get_caps(sample);
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    static GstStructure *s;
    const GstStructure *info = gst_sample_get_info(sample);
    if(framecount==1)
    {
        g_print ("%s\n", gst_caps_to_string(caps));
        if(!caps)
        {
            g_print("Could not get image info from filter caps");
            return GST_FLOW_CUSTOM_ERROR_1;
        }

        s = gst_caps_get_structure(caps,0);
        gboolean res = gst_structure_get_int(s, "width", &width);
        res |= gst_structure_get_int(s, "height", &height);
        if(!res)
        {
            g_print("Could not get image width and height from filter caps");
            return GST_FLOW_CUSTOM_ERROR_2;
        }
        g_print("Image size: %d\t%d\n",width,height);
    }

    GstMapInfo map;
    gst_buffer_map (buffer, &map, GST_MAP_READ);
//    g_print("%s:%d\t%p %dx%d\n",__FUNCTION__,__LINE__,map.data,width,height);

    gstMediaSource *self = static_cast<gstMediaSource*>(data);
    if(self){
        QImage img = QImage(map.data,width,height,QImage::Format_RGB888);
//        img.save( "snapshot.jpg");
        self->imageProvider->img = QPixmap::fromImage(img);
        emit self->incoming(self->imageProvider->img);

//        emit self->incoming();
    }

//    cv::Mat frame(cv::Size(width, height), CV_8UC3, (char*)map.data, cv::Mat::AUTO_STEP);
//    cv::Mat edges;
//    cv::cvtColor(frame, edges, CV_RGB2GRAY);
//    cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
//    cv::Canny(edges, edges, 0, 10, 3);
//    cv::imshow("stream", edges);
//    cv::cvtColor(frame,frame,CV_BGR2RGB);
//    imshow("origin", frame);

    gst_buffer_unmap(buffer, &map);

    gst_sample_unref (sample);
    return GST_FLOW_OK;
}

static gboolean my_bus_callback (GstBus *bus, GstMessage *message, gpointer data)
{
    g_print ("Got %s message from %s\n", GST_MESSAGE_TYPE_NAME (message), GST_OBJECT_NAME (message->src));
    switch (GST_MESSAGE_TYPE (message))
    {
    case GST_MESSAGE_ERROR:
    {
        GError *err;
        gchar *debug;

        gst_message_parse_error (message, &err, &debug);
        g_print ("Error from %s: %s\n", GST_OBJECT_NAME (message->src), err->message);
        g_error_free (err);
        g_free (debug);
        break;
    }
    case GST_MESSAGE_EOS:
    {
        gstMediaSource *self = static_cast<gstMediaSource*>(data);
        if(self){
            self->endOfStream();
        }
    }
        //quit_flag = 1;
        break;
    case GST_MESSAGE_STATE_CHANGED:
        GstState oldstate, newstate;
        gst_message_parse_state_changed(message, &oldstate, &newstate, NULL);
        g_print ("Element %s changed state from %s to %s.\n",
                 GST_OBJECT_NAME (message->src),
                 gst_element_state_get_name (oldstate),
                 gst_element_state_get_name (newstate));
        break;
    default:
        /* unhandled message */
        break;
    }

    return TRUE;
}

int gstMediaSource::init(QString cmd, int argc, char *argv[],bool atonce)
{
    this->cmd = cmd;

    GError *error = NULL;
    bool ans = gst_init_check(&argc, &argv,&error);
    if(ans){
        g_print("initing GStreamer version: %s",gst_version_string());
//        g_print("%s\n", cv::getBuildInformation().c_str());

        gst_init (&argc, &argv);

        pipeline = gst_parse_launch (qPrintable(cmd), &error);
        if (error)
        {
            g_print ("could not construct pipeline: %s\n", error->message);
            g_error_free (error);
        } else {
            appsink  = GST_APP_SINK(gst_bin_get_by_name (GST_BIN (pipeline), "sink"));
            GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PAUSED);
            switch(ret)
            {
                case GST_STATE_CHANGE_FAILURE:
                    g_print ("failed to play the file\n");
                case GST_STATE_CHANGE_NO_PREROLL:
                    g_print ("live source detected\n");
//                    live_flag = 1;
                    break;
                default:
                    break;
            }

            gst_app_sink_set_emit_signals(appsink, true);
            gst_app_sink_set_drop(appsink, true);
            gst_app_sink_set_max_buffers(appsink, 1);
            GstAppSinkCallbacks callbacks = { NULL, new_preroll, new_sample };
            gst_app_sink_set_callbacks (appsink, &callbacks, this,NULL);

            GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
            guint bus_watch_id = gst_bus_add_watch (bus, my_bus_callback, this);
            gst_object_unref (bus);

            if(atonce){
                run();
            }
        }

    } else {
        g_print ("could not init GSteamer: %s\n", error->message);
        g_error_free (error);
    }
}

void gstMediaSource::run()
{
    if(pipeline){
        if(status == GST_STATE_NULL){
            g_print ("running!\n");
            status = GST_STATE_PLAYING;
        } else if(status == GST_STATE_PLAYING){
            g_print ("pausing!\n");
            status = GST_STATE_NULL;
        }
        gst_element_set_state (pipeline, status);
    }
}

int gstMediaSource::deInit()
{
    if(pipeline){
        g_print ("%s:bye!\n",__FUNCTION__);
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (pipeline);
    }
}

void gstMediaSource::test(double fromQML)
{
    qDebug() << fromQML;
}

void gstMediaSource::getFrame()
{
    if(appsink && !gst_app_sink_is_eos(appsink) ) {
        g_main_context_iteration(NULL,false);
    }
}

void gstMediaSource::endOfStream()
{
    if(pipeline){
        g_print ("%s:over!\n",__FUNCTION__);
        gst_element_set_state (pipeline, GST_STATE_NULL);
    }
}
