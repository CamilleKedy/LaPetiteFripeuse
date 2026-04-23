import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels

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

    Tile {
        anchors.topMargin: 10
        anchors.fill: parent
        title: qsTr(pageTitle.text)
        content: ColumnLayout {
            TextField {
                id: searchField
                placeholderText: qsTr("Search...")
                font.pixelSize: charter.minInputFontSize
                background: Rectangle {
                    radius: 3
                    color: Qt.rgba(1,1,1,0.06)
                    border.color: Qt.rgba(1,0,0,1)
                    implicitWidth: page.width / 5
                }
                Layout.alignment: Qt.AlignRight
            }

            HorizontalHeaderView {
                id: horizontalHeader
                Layout.fillWidth: true
                syncView: tableView
                model: ["checked", "amount", "fruitType", "fruitName", "fruitPrice"]
                clip: true
            }

            TableView {
                id: tableView
                Layout.fillWidth: true
                Layout.fillHeight: true
                columnSpacing: 1
                rowSpacing: 1
                boundsBehavior: Flickable.StopAtBounds

                model: TableModel {
                    TableModelColumn { display: "checked" }
                    TableModelColumn { display: "amount" }
                    TableModelColumn { display: "fruitType" }
                    TableModelColumn { display: "fruitName" }
                    TableModelColumn { display: "fruitPrice" }

                    // Each row is one type of fruit that can be ordered
                    rows: [
                        {
                            // Each property is one cell/column.
                            checked: false,
                            amount: 1,
                            fruitType: "Apple",
                            fruitName: "Granny Smith",
                            fruitPrice: 1.50
                        },
                        {
                            checked: true,
                            amount: 4,
                            fruitType: "Orange",
                            fruitName: "Navel",
                            fruitPrice: 2.50
                        },
                        {
                            checked: false,
                            amount: 1,
                            fruitType: "Banana",
                            fruitName: "Cavendish",
                            fruitPrice: 3.50
                        }
                    ]
                }
                delegate:  TextInput {
                    text: model.display
                    padding: 12
                    selectByMouse: true

                    onAccepted: model.display = text

                    Rectangle {
                        anchors.fill: parent
                        color: "#efefef"
                        z: -1
                    }
                }
            }
        }

    }
}
