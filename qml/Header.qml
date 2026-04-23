import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    id: header
    property bool sideBarOpened: true
    height: 56
    background: Rectangle {
        color: "deeppink"
    }

    RowLayout {
        anchors.fill: parent
        spacing: 16
        ToolButton {
            text: sideBarOpened ? qsTr("Open") : qsTr("Close")
            onClicked: {
                sideBarOpened = !sideBarOpened
            }
        }
        TextField {
            id: searchField
            placeholderText: qsTr("Search...")
            background: Rectangle {
                radius: 6
                color: Qt.rgba(1,1,1,0.06)
                // border.color: Qt.rgba(1,0,0,1)
                implicitWidth: 300
            }
        }
        Rectangle {
            color: Qt.rgba(0,0,0,0.0)
            Layout.fillWidth: true
        }
        ToolButton {
            text: qsTr("Notifications")
        }
        ToolButton {
            text: qsTr("Settings")
        }
        // add user avatar or actions here
    }
}
