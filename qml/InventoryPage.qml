import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ArticleModel 1.0

// import ".."

Page {
    id: page
    property alias pageTitle: pageTitle.text

    background: Rectangle {
        color: Qt.rgba(0,0,0,0)
    }

    header: Label {
        id: pageTitle
        text: qsTr("Default page header")
        font.pixelSize: charter.pageTitleSize
        font.bold: charter.pageTitleBold
    }

    StackLayout {
        anchors.topMargin: 10
        anchors.fill: parent

        InventoryTile {
            title: qsTr(pageTitle.text)
        }
    }
}
