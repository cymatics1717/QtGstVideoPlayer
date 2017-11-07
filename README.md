
gstreamer udp streaming with Qt client
--------------------
# UDP

## server:

gst-launch filesrc location=./sintel_trailer-480p.webm ! decodebin ! queue ! videoconvert ! jpegenc ! queue ! multipartmux ! udpsink host=127.0.0.1 port=8554

gst-launch v4l2src ! decodebin ! queue ! videoconvert ! jpegenc ! queue ! multipartmux ! udpsink host=127.0.0.1 port=8554

## client

gst-launch udpsrc port=8554 ! multipartdemux ! jpegparse ! jpegdec ! videoconvert ! autovideosink

--------------------
# RTP payload

## server:

gst-launch --gst-debug=4 filesrc location=./sintel_trailer-480p.webm ! decodebin ! queue ! videoconvert ! jpegenc ! queue ! rtpjpegpay ! udpsink host=127.0.0.1 port=8554

gst-launch --gst-debug=4 v4l2src device=/dev/video0 ! "image/jpeg,width=800, height=600,framerate=30/1" ! rtpjpegpay ! udpsink host=127.0.0.1 port=8554

gst-launch --gst-debug=4 v4l2src device=/dev/video0 ! rtpjpegpay ! udpsink host=127.0.0.1 port=8554

## client

gst-launch --gst-debug=4 udpsrc port=8554 caps="application/x-rtp,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegparse ! queue max-size-time=0 max-size-buffers=0 ! jpegdec ! autovideosink

gst-launch --gst-debug=4 udpsrc port=8554 caps="application/x-rtp,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegparse ! queue ! jpegdec ! autovideosink


