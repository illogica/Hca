import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle{
    id: root
    border.color: "brown"
    border.width: 2
    radius: 2
    antialiasing: true
    height: 40
    width: 360
    color: Qt.rgba(Math.random(),Math.random(),Math.random(),0.1)
    Row{
        Image {
            id: imageAvatar
            height: root.height
            width: height
            fillMode: Image.PreserveAspectFit
            source: "qrc:/emojione/" + roomAvatar + ".svg"
            sourceSize.width: imageAvatar.width
            sourceSize.height: imageAvatar.height
            mipmap: true
            smooth: true
        }

        Column{

            Row{
                Label{
                    //anchors.left: parent.left
                    text: 'Name: <b>' +roomName + "</b>, "
                }
                Label{
                    //anchors.right: parent.right
                    text: roomSize + " clients"
                }
            }

            Label{
                text: roomDescription
            }
        }
    }
}
