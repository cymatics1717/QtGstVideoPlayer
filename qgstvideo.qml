import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    id: root
    visible: true
    width: 800
    height: 600
    color: "#333333"

    Image {
        id: media
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit

        focus: true
        Keys.onPressed: {
            console.log('pressed Key '+ event.text);
            if (event.key === Qt.Key_Return || event.key === Qt.Key_F) {
                root.visibility === Window.FullScreen ? root.showNormal() : root.showFullScreen()
            } else if (event.key === Qt.Key_Q) {
                Qt.quit();
            }
        }
    }

    Connections {
        target: source

        onIncoming: {
            var cnt = Math.random()
//            console.log("-------",++cnt,image)
//            source.test(cnt)
//            media.source=""
            media.source= "image://pool/"+cnt
        }
    }
    NumberAnimation {
        id: animation
        target: media
        property: "rotation"
        from: 0
        to: 360
        duration: 1000
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            animation.running = true;
        }
    }

}
