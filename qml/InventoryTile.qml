import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels
import ArticleModel 1.0
import Qt5Compat.GraphicalEffects

import "../qml/Utilities"

Tile {
    id: tile
    // anchors.topMargin: 10
    // anchors.fill: parent
    title: qsTr("Default Inventory title...")
    showOptions: true
    optionsMenu: Menu {
        id: contextMenu
        visible: false
        delegate: MenuItem {
            font.pixelSize: charter.minInputFontSize
            height: searchField.height
        }
        Action {
            text: qsTr("Add articles ...")
        }
        Action {
            text: qsTr("Selection mode")
        }
        Action {
            text: qsTr("Export data")
        }
    }
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
                model: categoryModel /*["----", "ADORNMENT", "BAG", "COSMETIC", "GARMENT", "PERFUME", "SHOES"]*/
                delegateWidth: searchField.width * 3 / 4
                delegateHeight: searchField.height
                Layout.preferredHeight: searchField.height
                textRole: "text"
                valueRole: "value"

                onActivated: {
                    articleProxyModel.categoryComboBoxCurrentValue = currentValue;
                }
            }

            UTextField {
                id: searchField
                backgroundWidth: tile.width / 5
                placeholder: qsTr("Search...")
                onTextChanged: articleProxyModel.searchTxtField = text;
            }
        }

        UHorizontalHeaderView {
            id: horizontalHeader
            Layout.fillWidth: true
            syncView: tableView
            onHeaderClicked: (column) => {
                sortedColumn = column;
                model.updateSorting(column);
            }
        }

        UTableView {
            id: tableView
            Layout.fillWidth: true
            Layout.fillHeight: true
            baseWidths: [100, 150, 400, 300,// id, pic, name, desc
                        250, 150, 100,      // price, min price, qty
                        250, 300, 400,      // cat, subcat, supplier
                        200, 200, 300]      // cdate, mdate, action
            Component.onCompleted: {
                // hiddenColumns.add(ArticleModel.DescriptionColumn);
                // hiddenColumns.add(ArticleModel.ModificationDateColumn);
                // forceLayout();
            }

            model: articleProxyModel
            delegate: DelegateChooser {
                // role: "display"
                DelegateChoice {
                    column: ArticleModel.IdColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.PictureColumn
                    delegate: Rectangle {
                        color: "#efefef"
                        Image {
                            id: articlePic
                            asynchronous: true
                            anchors.centerIn: parent
                            source: "file:///" + picPath + "/" + model.display
                            width: Math.min(parent.width, parent.height) - 5
                            height: Math.min(parent.width, parent.height) - 5
                            sourceSize.width: width
                            sourceSize.height: height
                            fillMode: Image.PreserveAspectCrop
                            layer.enabled: true
                            layer.smooth: true
                            layer.effect: OpacityMask {
                                maskSource: Rectangle {
                                    width: articlePic.width
                                    height: articlePic.height
                                    radius: width / 2
                                }
                            }
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.NameColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.DescriptionColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.PriceColumn
                    delegate: Label {
                        text: (model.display.lowestPrice === model.display.highestPrice) ?
                                  model.display.lowestPrice : model.display.lowestPrice + " - " + model.display.highestPrice
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.MinPriceColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.QuantityColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.CategoryColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.SubCategoryColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.SupplierColumn
                    delegate: Label {
                        text: model.display
                        elide: Text.ElideRight
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.CreationDateColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.ModificationDateColumn
                    delegate: Label {
                        text: model.display
                        font.pixelSize: charter.minLabelFontSize
                        padding: 12
                        Rectangle {
                            anchors.fill: parent
                            color: "#efefef"
                            z: -1
                        }
                    }
                }
                DelegateChoice {
                    column: ArticleModel.ActionColumn
                    delegate: Rectangle {
                        color: "#efefef"
                        Button {
                            anchors.centerIn: parent
                            width: parent.width * 3 / 4
                            height: parent.height * 4 / 5
                            text: qsTr("View")
                            font.pixelSize: charter.minLabelFontSize - 2
                            background: Rectangle {
                                id: bkg
                                color: "white"
                                border.color: "black"
                                border.width: 1
                            }
                            // onHoveredChanged: bkg.border.color = hovered ? "black" : charter.deeppink
                        }
                    }
                }
            }
        }
    }
}
