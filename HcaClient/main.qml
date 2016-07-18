import QtQuick 2.5
//import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

ApplicationWindow{
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { text: "Open..." }
            MenuItem {
                text: "Close"
                onTriggered: Qt.quit()
            }
        }
    }

    toolBar:ToolBar {
        id: myToolbar
        RowLayout {
            anchors.fill: parent
            Button {
                id: buttonQuit
                text: "Quit"
                onClicked: Qt.quit()
            }

            Item { Layout.fillWidth: true }
            CheckBox {
                text: "Connected"
                checked: false
                Layout.alignment: Qt.AlignRight
                onCheckedChanged: {
                    checked ? HcaClient.connected=true : HcaClient.connected=true
                }
            }
        }
    }

    statusBar: StatusBar {
        id: myStatusbar
        RowLayout {
            anchors.fill: parent
            Label { text: HcaClient.connected ? "Connected" : "Not connected" }
        }
    }

    ColumnLayout {
        id: columnLayout1

        anchors.fill: contentItem

        Button {
            id: buttonLogin
            text: qsTr("Login")
            onClicked: HcaClient.sendLogin();
        }
    }
}
