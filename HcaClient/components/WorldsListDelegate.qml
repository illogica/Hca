import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle{
    id: root
    border.color: "grey"
    border.width: 2
    radius: 5
    antialiasing: true
    height: 40
    width: 360
    color: Qt.rgba(Math.random(),Math.random(),Math.random(),0.1)

    Column{
        Row{
            Label{
                //anchors.left: parent.left
                text: 'Name: <b>' +worldName + "</b>, "
            }
            Label{
                //anchors.right: parent.right
                text: worldSize + " rooms"
            }
        }

        Label{
            text: worldDescription
        }
    }
}

