import QtQuick 2.5
//import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

ApplicationWindow{
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property string currentWorld: ""

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Open world..."
                onTriggered: dialogChooseWorld.visible = true
            }
            MenuItem {
                text: "Close"
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: "Room"
            MenuItem {
                text: "Join/Create..."
                onTriggered: dialogRoomJoin.visible = true
            }
            MenuItem {
                text: "Leave..."
                onTriggered: dialogRoomLeave.visible = true
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
                    checked ? HcaClient.connected=true : HcaClient.connected=false
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

    StackView {
        id: stackView
        anchors.fill: contentItem
        initialItem: Item {
            id: mainView
            anchors.fill: parent
            anchors.centerIn: parent

            /*ColumnLayout{
                anchors.centerIn: parent
                anchors.fill: parent
                spacing: 10
                /*Button{
                    text: "Contacts"
                    onClicked: console.warn("contacts pushed")
                }
                Button{
                    text: "Rooms"
                    onClicked: stackView.push(roomsView)
                }*/

                ListView {
                    id: listWorldsTemp

                    anchors.fill: parent
                    model: wlModel

                    delegate:
                        Rectangle {

                        Component.onCompleted: console.warn("Name: " + modelData.name)

                        color: Qt.rgba(0.5,0.1,0.1,0.5)

                        width: 180; height: 40
                        Column {
                            Text { text: '<b>Name:</b> ' + name}
                            Text { text: '<b>Size:</b> ' + model.modelData.size }
                        }
                    }
               // }
            }
        }

    }

    /*Item{
       id: roomsView
       anchors.fill: parent

       ListView{
           id: listViewRooms
           anchors.fill: parent
           model: worldsListModel  //from c++
           delegate: Rectangle{
               height: 25
               width: parent.width
               Text{
                   anchors.left: parent.left
                   text: name
               }
               Text{
                   anchors.right: parent.right
                   text: size + " clients"
               }
           }
       }
    }*/

    Dialog {
        id: dialogRoomJoin
        modality: Qt.WindowModal
        title: "Insert room name!"

        standardButtons: StandardButton.Ok | StandardButton.Cancel

        onAccepted: {
            console.warn("Accepted: ", textRoomName.text);
            HcaClient.joinRoom(textRoomName.text, currentWorld);
        }


        ColumnLayout {
            width: parent ? parent.width : 100
            Label {
                text: "Insert room name to join/create: "
                Layout.columnSpan: 2
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                TextField {
                    id: textRoomName
                }
                Label {
                    text: "Choose the name wisely!"
                    Layout.alignment: Qt.AlignBaseline | Qt.AlignLeft
                }
            }

            ListView {
                id: listRooms
                width: parent.width
                model: roomsModel
                delegate:
                    Item {

                    width: 180; height: 40
                    Column {
                        Text { text: '<b>Name:</b> ' + name }
                        Text { text: '<b>Size:</b> ' + size }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            dialogChooseWorld.visible = false;
                            HcaClient.joinRoom(name, currentWorld)
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: dialogRoomLeave
        modality: Qt.WindowModal
        title: "Insert room name!"

        standardButtons: StandardButton.Ok | StandardButton.Cancel

        onAccepted: {
            console.warn("Accepted: ", textRoomLeave.text);
            HcaClient.leaveRoom(textRoomLeave.text);
        }


        ColumnLayout {
            width: parent ? parent.width : 100
            Label {
                text: "Insert room name to join/create: "
                Layout.columnSpan: 2
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                TextField {
                    id: textRoomLeave
                }
                Label {
                    text: "Choose the name wisely!"
                    Layout.alignment: Qt.AlignBaseline | Qt.AlignLeft
                }
            }
        }
    }

    Dialog {
        id: dialogChooseWorld
        modality: Qt.WindowModal
        title: "Chose your world!"

        ListView {
            id: listWorlds
            width: parent.width
            model: wlModel
            delegate:
                Item {

                Component.onCompleted: console.log("Name: " + name)

                width: 180; height: 40
                Column {
                    Text { text: '<b>Name:</b> ' + name }
                    Text { text: '<b>Size:</b> ' + size }
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        dialogChooseWorld.visible = false;
                        currentWorld = name
                        HcaClient.sendGetRoomsList(name)
                    }
                }
            }
        }
    }
}
