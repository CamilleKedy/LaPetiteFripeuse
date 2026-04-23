import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: pane
    property Item content
    property Menu optionsMenu
    property string title: qsTr("Default Title")
    property bool showOptions: false

    padding: 5
    background: Rectangle {
        color: "white"
        radius: 3
    }

    MouseArea {
        id: msArea
        visible: showOptions
        width: 20
        height: 20
        z: 1
        anchors.rightMargin: 10
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.top: parent.top
        hoverEnabled: true
        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
        onClicked: {
            optionsMenu.x = x - optionsMenu.width + width;
            optionsMenu.y = y + height;
            optionsMenu.visible = !optionsMenu.visible;
        }

        Image {
            anchors.centerIn: parent
            source: "qrc:/icons/options-horizontal-svgrepo-com-black.svg"
            sourceSize.width: 20
            sourceSize.height: 20
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Label {
            text: qsTr(pane.title)
            font.bold: true
            Layout.fillWidth: true
        }

        LayoutItemProxy {
            target: pane.content
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
