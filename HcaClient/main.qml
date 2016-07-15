import QtQuick 2.5
import QtQuick.Window 2.2
import QtWebSockets 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    WebSocket {
        id: socket
        url: "ws://localhost:8081"
        onTextMessageReceived: {
            txt.text = txt.text + "\nReceived message: " + message
        }
        onStatusChanged: if (socket.status == WebSocket.Error) {
                             console.log("Error: " + socket.errorString)
                         } else if (socket.status == WebSocket.Open) {
                             socket.sendTextMessage('{{"ra":"0"}')
                         } else if (socket.status == WebSocket.Closed) {
                             txt.text += "\nSocket closed"
                         }
        active: false
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            //Qt.quit();
            socket.active = !socket.active
        }
    }

    Text {
        id: txt
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}
