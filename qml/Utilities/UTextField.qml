import QtQuick
import QtQuick.Controls

TextField {
    id: txtField
    property string placeholder: qsTr("Placeholder...")
    property alias backgroundWidth: rectangle.implicitWidth

    placeholderText: focus ? "" : (text.length ? "" : placeholder)
    font.pixelSize: charter.minInputFontSize
    background: Rectangle {
        id: rectangle
        radius: 3
        color: Qt.rgba(1,1,1,0.06)
        border.color: Qt.rgba(1,0,0,1)
        implicitWidth: 100
    }
}
