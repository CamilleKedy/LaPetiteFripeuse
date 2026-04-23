import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: appWindow
    background: Rectangle {
        color: "deeppink"
        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: contentStack.height * 0.9
            color: "#eeeeee"
        }
    }

    RowLayout {
        anchors.fill: parent

        SideBar {
            id: sidebar
            width: 240
            visible: true
            modal: false
            interactive: false
            dim: false
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: sidebar.visible ? sidebar.position * sidebar.width : 0

            Header {
                id: headerBar
                Layout.fillWidth: true
                onSideBarOpenedChanged: sidebar.visible = !sidebar.visible
                // transform: Translate {
                //     x: sidebar.position * sidebar.width
                // }
            }

            StackLayout {
                id: contentStack
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: app.width / 20
                Layout.rightMargin: app.width / 20
                Layout.topMargin: 10
                Layout.bottomMargin: 5

                DashboardPage { id: dashboardPage; pageTitle: qsTr("Welcome back, Camille!") }

                OrdersPage { id: ordersPage; pageTitle: sidebar.activeButtonName }

                InventoryPage { id: inventoryPage; pageTitle: sidebar.activeButtonName }

                Rectangle {
                    color: "black"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    StatTile {
                        id: st1
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 5
                        anchors.topMargin: 5
                        width: (appWindow.width - sidebar.width * sidebar.position) / 5
                        height: 150
                    }
                    StatTile {
                        id: st2
                        anchors.left: st1.right
                        anchors.top: parent.top
                        anchors.leftMargin: 5
                        anchors.topMargin: 5
                        width: (appWindow.width - sidebar.width * sidebar.position) / 5
                        height: 150
                    }
                    StatTile {
                        id: st3
                        anchors.left: st2.right
                        anchors.top: parent.top
                        anchors.leftMargin: 5
                        anchors.topMargin: 5
                        width: (appWindow.width - sidebar.width * sidebar.position) / 5
                        height: 150
                    }
                }

                // future pages: UsersPage, ProductsPage, SettingsPage etc.
            }
        }
    }
}
