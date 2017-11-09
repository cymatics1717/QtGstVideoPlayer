
gstreamer udp streaming with Qt client
--------------------
# UDP

## server:

```
gst-launch filesrc location=./sintel_trailer-480p.webm ! decodebin ! queue ! videoconvert ! jpegenc ! queue ! multipartmux ! udpsink host=127.0.0.1 port=8554
```
```
gst-launch v4l2src ! decodebin ! queue ! videoconvert ! jpegenc ! queue ! multipartmux ! udpsink host=127.0.0.1 port=8554
```

## client
```
gst-launch udpsrc port=8554 ! multipartdemux ! jpegparse ! jpegdec ! videoconvert ! autovideosink
```
```
GST_DEBUG=4 ./QtGstVideoPlayer
```
--------------------
# RTP payload

## server:
```
gst-launch --gst-debug=4 filesrc location=./sintel_trailer-480p.webm ! decodebin ! queue ! videoconvert ! jpegenc ! queue ! rtpjpegpay ! udpsink host=127.0.0.1 port=8554
```
```
gst-launch --gst-debug=4 v4l2src device=/dev/video0 ! "image/jpeg,width=800, height=600,framerate=30/1" ! rtpjpegpay ! udpsink host=127.0.0.1 port=8554
```
```
gst-launch --gst-debug=4 v4l2src device=/dev/video0 ! rtpjpegpay ! udpsink host=127.0.0.1 port=8554
```
## client
```
gst-launch --gst-debug=4 udpsrc port=8554 caps="application/x-rtp,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegparse ! queue max-size-time=0 max-size-buffers=0 ! jpegdec ! autovideosink
```
```
gst-launch --gst-debug=4 udpsrc port=8554 caps="application/x-rtp,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegparse ! queue ! jpegdec ! autovideosink
```
```
GST_DEBUG=4 ./QtGstVideoPlayer
```

```
gst-discoverer-1.5 ./sintel_trailer-480p.webm

gst-launch filesrc location=./sintel_trailer-480p.webm ! decodebin ! jpegenc ! udpsink host=127.0.0.1 port=8554
gst-launch filesrc location=./sintel_trailer-480p.webm ! matroskademux ! vp8dec ! jpegenc !  udpsink host=127.0.0.1 port=8554
gst-launch udpsrc port=8554 ! jpegdec ! autovideosink

gst-launch v4l2src ! rtpjpegpay ! udpsink host=127.0.0.1 port=8554
gst-launch udpsrc port=8554 caps="application/x-rtp,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegparse ! jpegdec ! autovideosink


gst-launch filesrc location= ./sintel_trailer-480p.webm ! matroskademux name=d  \
d.audio_0 ! queue ! vorbisdec ! audioconvert ! queue ! autoaudiosink  \
d.video_0 ! queue ! vp8dec    ! videoconvert ! queue ! autovideosink


https://github.com/matthiasbock/gstreamer-phone/wiki/Streaming-H.264-via-RTP

gst-launch -v v4l2src ! videoconvert ! x264enc ! rtph264pay ! udpsink host=127.0.0.1 port=9078
gst-launch -v udpsrc port=9078 ! 'application/x-rtp,payload=96,encoding-name=H264' ! rtph264depay ! avdec_h264 ! autovideosink

```

