import QtQuick 2.5
import QtQuick.Window 2.2
import QtQml 2.2
Window {
    visible: true
    width: 800
    height: 600

    Image {
        id:media
        anchors.fill: parent
    }
    Connections {
        target: source
        onIncoming: {
            media.source=""
            media.source= "image://source/source"
        }
    }
}
