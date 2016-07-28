import QtQuick 2.5

Item {
    id: root
    visible: true
    height: 640
    width: 480

    signal clicked(int id)

    ListView {
        id: listViewWorlds

        anchors.fill: parent
        //model: wlModel
        model: tempModel

        delegate:
            Item {
                id: container
                width: ListView.view.width; height: 60; anchors.leftMargin: 10; anchors.rightMargin: 10

                Rectangle {
                    id: content
                    anchors.centerIn: parent; width: container.width - 40; height: container.height - 10
                    color: "transparent"
                    antialiasing: true
                    radius: 10

                    Rectangle { anchors.fill: parent; anchors.margins: 3; color: "#91AA9D"; antialiasing: true; radius: 8 }
                }

                Text {
                    id: label
                    anchors.centerIn: content
                    text: "List element " + (index + 1)
                    color: "#193441"
                    font.pixelSize: 14
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        container.ListView.view.currentIndex = index
                        container.forceActiveFocus()
                        console.log("Clicked");
                    }
                }

            /*Rectangle {
            id: listViewWorldsDelegate

            //Component.onCompleted: console.warn("Name: " + modelData.name + ", " + modelData.id)

            color: Qt.rgba(Math.random(),Math.random(),Math.random(),0.1)

            width: root.width;
            height: 40
            Column {
                Text { text: '<b>Name:</b> ' + name}
                //Text { text: '<b>Size:</b> ' + model.modelData.size }
                Text { text: '<b>Size:</b> ' + size }
            }

            MouseArea{
                id: clickArea
                anchors.fill: parent
                z: 1
                hoverEnabled: true
                onClicked: {
                    console.log("Clicked: ")
                    //root.clicked(model_id)
                }
            }*/
        }
    }

    ListModel {
        id: tempModel
        ListElement{
            name: "Fake world 1"
            description: "Description for fake world 1"
            model_id: 0
            size: 10
        }
        ListElement{
            name: "Fake world 2"
            description: "Description for fake world 2"
            model_id: 1
            size: 1
        }
    }
}
