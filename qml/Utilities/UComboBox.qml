import QtQuick
import QtQuick.Controls

ComboBox {
    property real delegateWidth: 100
    property real delegateHeight: 100

    id: cbox
    font.pixelSize: charter.minInputFontSize
    background: Rectangle {
        id: bkg
        radius: 3
        color: Qt.rgba(1,1,1,0.06)
        border.color: Qt.rgba(1,0,0,1)
        implicitWidth: delegateWidth
    }
    // model: []
    delegate: ItemDelegate {
        id: cboxDelegate
        text: typeof modelData !== "undefined" ? qsTr(modelData.text) : qsTr(model.text)
        font.pixelSize: charter.minInputFontSize
        width: delegateWidth
        height: delegateHeight
        highlighted: cbox.highlightedIndex === index
    }
    popup.y: y + height

    onAccepted: {
        // if (find(editText) === -1)
        //     model.append({text: editText})
    }
}
