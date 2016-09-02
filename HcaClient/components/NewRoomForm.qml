import QtQuick 2.2
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Window {
    id: loginWindow
    property string username: login.text;
    property string password: password.text;
    property bool issave: savePassword.checked;

    flags: Qt.Dialog
    modality: Qt.WindowModal
    width: 400
    height: 160
    minimumHeight: 160
    minimumWidth: 400
    title: "Login to program"

    GridLayout {
        columns: 2
        anchors.fill: parent
        anchors.margins: 10
        rowSpacing: 10
        columnSpacing: 10

        Label {
            text: "Room name"
        }
        TextField {
            id: roomName
            text: "room name"
            Layout.fillWidth: true
        }

        Label {
            text: "Room Description"
        }
        TextField {
            id: description
            text: "description"
            Layout.fillWidth: true
        }

        Label {
            text: "Room motd"
        }
        TextField {
            id: motd
            text: "motd"
            Layout.fillWidth: true
        }

        Item {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            implicitHeight: button.height

            Button {
                id: button
                anchors.centerIn: parent
                text: "Enter"
                onClicked: {
                    loginWindow.close();
                }
            }
        }
    }
}
