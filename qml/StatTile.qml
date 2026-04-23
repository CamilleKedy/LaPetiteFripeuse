import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: pane
    property double mainStat: 99999999
    property bool isCurrency: false
    property string title: "Default Title"
    property var icon

    padding: 5
    background: Rectangle {
        color: "white"
        radius: 3
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: qsTr(pane.title)
                font.bold: true
            }

            RowLayout {
                Label {
                    text: {
                        let locale = Qt.locale("fr_CM")
                        return qsTr(mainStat.toLocaleString(locale, 'f', 0))
                    }
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    Layout.fillHeight: true
                }
                Label {
                    visible: pane.isCurrency
                    text: {
                        let locale = Qt.locale("fr_CM")
                        return locale.currencySymbol(Locale.CurrencySymbol)
                    }
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 10
                    Layout.fillHeight: true
                }
            }

            RowLayout {
                Label {
                    text: qsTr("2.3%") // TO DO: Adapt this
                    color: "green"
                }
                Label {
                    text: qsTr("better than last week") // TO DO: Adapt this
                    Layout.fillWidth: true
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                Layout.preferredWidth: 40
                Layout.preferredHeight: 40
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                radius: width / 2
                color: "red"

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/icons/money-finance-cash-payment-svgrepo-com.svg"
                    sourceSize.width: parent.width - 5
                    sourceSize.height: parent.height - 5
                }
                // Text {
                //     anchors.centerIn: parent
                //     text: "\u{1F69A}" // delivery truck emoji
                //     font.pixelSize: 28
                //     color: "white"
                // }
            }
            Rectangle {
                Layout.fillHeight: true
            }
        }
    }
}
