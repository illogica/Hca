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
        RowLayout {
            anchors.fill: parent
            Button {
                id: buttonQuit
                text: "Quit"
                onClicked: Qt.quit()
            }

            Item { Layout.fillWidth: true }
            CheckBox {
                text: "Enabled"
                checked: true
                Layout.alignment: Qt.AlignRight
            }
        }
    }

    statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            Label { text: "Status bar" }
        }
    }
}
