import QtQuick
import QtGraphs
import QtQuick.Controls
import QtQuick.Layouts

Page {
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

    ColumnLayout {
        anchors.topMargin: 10
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Tile {
                Layout.fillWidth: true
                Layout.preferredHeight: 305
                title: qsTr("Recent movement")
                content: GraphsView {
                    id: chart
                    theme: GraphsTheme {
                        colorScheme: GraphsTheme.ColorScheme.Light
                        backgroundColor: Qt.rgba(0,0,0,0)
                        seriesColors: ["deeppink", "black"]
                        grid.mainColor: "#888888"
                        grid.mainWidth: 0.5
                        grid.subColor: Qt.rgba(0,0,0,0)
                    }
                    axisX: BarCategoryAxis  { // TO DO: Switch to AbstractAxis
                        categories: ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]
                        lineVisible: false
                    }
                    axisY: ValueAxis {
                        min: 0
                        max: 10
                        subTickCount: 4
                    }
                    LineSeries {
                        capStyle: Qt.FlatCap
                        pointDelegate: Rectangle {
                            width: 5
                            height: width
                            radius: width / 2
                            border.color: "deeppink"
                        }
                        XYPoint { x: 1; y: 3 }
                        XYPoint { x: 2; y: 5 }
                        XYPoint { x: 3; y: 5 }
                        XYPoint { x: 4; y: 7 }
                        XYPoint { x: 5; y: 2 }
                        XYPoint { x: 6; y: 5 }
                        XYPoint { x: 7; y: 9 }
                    }
                    SplineSeries {
                        capStyle: Qt.FlatCap
                        pointDelegate: Rectangle {
                            width: 5
                            height: width
                            radius: width / 2
                            border.color: "black"
                        }
                        XYPoint { x: 0.5; y: 3 }
                        XYPoint { x: 1.5; y: 5 }
                        XYPoint { x: 2.5; y: 5 }
                        XYPoint { x: 3.5; y: 7 }
                        XYPoint { x: 4.5; y: 2 }
                        XYPoint { x: 5.5; y: 5 }
                        XYPoint { x: 6.5; y: 9 }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true

                StatTile {
                    Layout.preferredWidth: (appWindow.width - sidebar.width * sidebar.position) / 5
                    Layout.preferredHeight: 150
                    isCurrency: true
                }
                StatTile {
                    Layout.preferredWidth: (appWindow.width - sidebar.width * sidebar.position) / 5
                    Layout.preferredHeight: 150
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Tile {
                Layout.preferredWidth: appWindow.width / 3
                Layout.preferredHeight: 305
                title: qsTr("Sales per category")
                content: GraphsView {
                    id: chart2
                    theme: GraphsTheme {
                        colorScheme: GraphsTheme.ColorScheme.Light
                        backgroundColor: Qt.rgba(0,0,0,0)
                        seriesColors: ["deeppink", "black"]
                        grid.mainColor: "#888888"
                        grid.mainWidth: 0.5
                        grid.subColor: Qt.rgba(0,0,0,0)
                    }
                    axisX: BarCategoryAxis {
                        categories: ["Adornment", "Bag", "Cosmetic", "Garment", "Perfume", "Shoes"]
                        lineVisible: false
                    }
                    axisY: ValueAxis {
                        min: 0
                        max: 10
                        subTickCount: 4
                    }
                    BarSeries {
                        BarSet {
                            values: [7, 6, 9, 8, 2, 1]
                        }
                        BarSet {
                            values: [9, 8, 6, 1, 0, 4]
                        }
                    }
                }
            }

            Tile {
                Layout.fillWidth: true
                Layout.preferredHeight: 305
                title: qsTr("Last sales")
                content: ScrollView {
                    width: 180; height: 200
                    clip: true
                    ListView {
                        id: view1
                        model: ListModel {
                            ListElement {
                                name: "Yellow dress"
                                price: "26000"
                            }
                            ListElement {
                                name: "Hugo Boss perfume"
                                price: "75000"
                            }
                            ListElement {
                                name: "Red bag"
                                price: "7500"
                            }
                            ListElement {
                                name: "Yellow dress"
                                price: "26000"
                            }
                            ListElement {
                                name: "Hugo Boss perfume"
                                price: "75000"
                            }
                            ListElement {
                                name: "Red bag"
                                price: "7500"
                            }
                        }
                        delegate: Rectangle {
                            required property string name
                            required property string price
                            color: "red"
                            border.color: "black"
                            width: view1.width
                            height: view1.height / 4
                            Text {
                                text: name + ": " + price
                            }

                        }
                    }
                }
            }
        }

        // add summary cards, KPI widgets, recent activity here
    }
}

