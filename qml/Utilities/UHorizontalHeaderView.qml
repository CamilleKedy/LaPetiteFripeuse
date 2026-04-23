import QtQuick
import QtQuick.Controls

HorizontalHeaderView {
    id: horizontalHeader

    property int order: Qt.AscendingOrder
    property string arrowStr: order === Qt.AscendingOrder ? " \u2983" : " \u4060"
    property int sortedColumn
    signal headerClicked(int column)

    // model: []
    clip: true
    boundsBehavior: Flickable.StopAtBounds
    boundsMovement: Flickable.StopAtBounds
    delegate: Label {
        id: label
        text: model.display
        font.pixelSize: charter.minLabelFontSize
        padding: 12
        Rectangle {
            anchors.fill: parent
            z: -1
        }
        Image {
            visible: sortedColumn === model.column
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            source: order === Qt.AscendingOrder ? "qrc:/icons/caret-down-svgrepo-com-black.svg"
                            : "qrc:/icons/caret-up-svgrepo-com-black.svg"
            sourceSize.width: charter.minLabelFontSize
            sourceSize.height: charter.minLabelFontSize
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
            onClicked: {
                console.log(model.sortedColumn);
                headerClicked(model.column);
            }
        }
    }
}
