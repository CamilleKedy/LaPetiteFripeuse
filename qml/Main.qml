import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

ApplicationWindow {
    id: app
    width: 1280
    height: 720
    visible: true
    // visibility: "Maximized"
    title: qsTr("La Petite Fripeuse")

    // Theming (switch to Universal if you prefer a Fluent look)
    Material.theme: Material.Light
    Material.primary: Qt.rgba(0,0,1,1)
    // Material.accent: Material.Indigo
    // Material.foreground: "#eaeef3"
    // Material.background: "#101317"

    QtObject {
        id: charter
        readonly property color deeppink: "deeppink"

        readonly property int pageTitleSize: 20
        readonly property bool pageTitleBold: true
        readonly property int graphTitleSize: 15

        readonly property int minInputFontSize: 12
        readonly property int maxInputFontSize: 15

        readonly property int minLabelFontSize: 12
        readonly property int maxLabelFontSize: 15
    }

    AppWindow
    {
        anchors.fill: parent
    }
}
