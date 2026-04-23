import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels
import ArticleModel 1.0
import Qt5Compat.GraphicalEffects

import "../qml/Utilities"

Tile {
    id: tile
    title: qsTr("Default Article title...")
    content: ColumnLayout {
        RowLayout {
            Item {
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Filter: ")
                font.pixelSize: charter.minInputFontSize
            }

            UComboBox {
                id: cbox
                model: ["----", "Adornment", "Bag", "Cosmetic", "Garment", "Perfume", "Shoes"]
                delegateWidth: searchField.width * 3 / 4
                delegateHeight: searchField.height
                Layout.preferredHeight: searchField.height
            }

            UTextField {
                id: searchField
                backgroundWidth: tile.width / 5
                placeholder: qsTr("Search...")
            }
        }

        // UHorizontalHeaderView {
        //     id: horizontalHeader
        //     Layout.fillWidth: true
        //     syncView: tableView
        //     model: articleProxyModel
        // }

        // UTableView {
        //     id: tableView
        //     Layout.fillWidth: true
        //     Layout.fillHeight: true
        //     baseWidths: [100, 150, 650,    // id, pic, name
        //                 300, 0, 200,      // price, min price, qty
        //                 400, 0, 300,      // cat, subcat, supplier
        //                 0, 300, 300]      // cdate, mdate, action
        //     model: articleProxyModel
        //     delegate: DelegateChooser {

        //     }
        // }
    }
}
