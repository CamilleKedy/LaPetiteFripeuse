import QtQuick
import QtQuick.Controls

Drawer {
    id: drawer
    property string activeButtonName: "Dashboard"
    height: parent.height
    width: parent.width
    edge: Qt.LeftEdge
    background: Rectangle {
        color: "deeppink"
        Rectangle {
            id: mainBackground
            anchors.bottom: parent.bottom
            width: parent.width
            height: parent.height - headerBar.height
            color: "white"
        }
    }



    ListView {
        anchors.bottom: parent.bottom
        height: parent.height - headerBar.height
        width: parent.width
        model: ListModel {
            ListElement { name: "Dashboard"; page: 0 }
            ListElement { name: "Orders"; page: 1 }
            ListElement { name: "Inventory"; page: 2 }
            ListElement { name: "Analytics"; page: 3 }
            // ListElement { name: "Customers"; page: 4 }
        }
        delegate: ItemDelegate {
            text: name
            width: parent.width
            onClicked: {
                contentStack.currentIndex = page
                activeButtonName = name
            }
        }
    }
}
