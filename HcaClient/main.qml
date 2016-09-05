import QtQuick 2.5
//import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "components" 1.0

ApplicationWindow{
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property string currentWorld: ""

    ListModel{
        id: worldsModel
        function addWorld(id, name, description, size){
            console.warn("Appending world " + name)
            worldsModel.append({
                                  "worldId": id,
                                  "worldName": name,
                                  "worldDescription": description,
                                  "worldSize": size
                              })
        }

        function reset(){
            clear();
            console.warn("Cleared worlds list");
        }
    }

    ListModel{
        id: roomsModel
        function addRoom(id, name, description, motd, size, count, avatar){
            roomsModel.append({
                                  "roomId": id,
                                  "roomName": name,
                                  "roomDescription": description,
                                  "roomMotd": motd,
                                  "roomSize": size,
                                  "roomCount": count,
                                  "roomAvatar": avatar
                              })
        }

        function reset(){
            clear();
            console.warn("Cleared worlds list");
        }
    }

    Connections{
        target: HcaClient
        onQmlWorldsListAdd: worldsModel.addWorld(id, name, description, size)
        onQmlWorldsListReset: worldsModel.reset()
        onQmlRoomsListAdd: roomsModel.addRoom(id, name, description, motd, size, count, avatar)
        onQmlRoomsListReset: roomsModel.reset()
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Open world..."
                onTriggered: dialogChooseWorld.visible = true
            }
            MenuItem {
                text: "Ping"
                onTriggered: HcaClient.sendPing()
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
                    checked ? HcaClient.active=true : HcaClient.active=false
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

    TabView {
        id: tabView
        anchors.fill: parent //contentItem
        onCurrentIndexChanged:{
            switch(currentIndex){
            case 0:
                console.warn("Selected worls tab");
                break;
            case 1:
                console.warn("Selected rooms tab");
                break;
            case 2:
                console.warn("Selected friends tab");
                break;
            case 3:
                console.warn("Selected profile tab");
                break;
            default:
                break;
            }
        }

        tabPosition: Qt.TopEdge

        Tab{  //currentIndex 0
            title: "Worlds"
            ListView{
                id: worldsListView
                model: worldsModel
                delegate: WorldsListDelegate{}
                anchors.fill: parent
            }
        }
        Tab{  //currentIndex 1
            title: "Rooms"
        }
        Tab{  //currentIndex 2
            title: "Friends"
        }
        Tab{  //currentIndex 3
            title: "Profile"
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

            /*ListView {
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
            }*/
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
