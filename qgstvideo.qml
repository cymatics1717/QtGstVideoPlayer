import QtQuick 2.5
import QtQuick.Window 2.2
import QtQml 2.2
Window {
    visible: true
    width: 800
    height: 600

    Image {
        id: media
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
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
}
