import QtQuick
import QtQuick.Controls

TableView {
    id: tableView
    required property var baseWidths
    property var hiddenColumns: new Set()

    clip: true
    columnSpacing: 1
    rowSpacing: 1
    boundsBehavior: Flickable.StopAtBounds
    boundsMovement: Flickable.StopAtBounds
    model: []
    columnWidthProvider: function(column) {
        if (hiddenColumns.has(column)) return 0;

        let activeTotal = 0;
        for (let i = 0; i < baseWidths.length; i++)
        {
            if (!hiddenColumns.has(i)) activeTotal += baseWidths[i];
        }

        if (activeTotal === 0) return 0;
        const scale = width / activeTotal;

        return Math.floor(baseWidths[column] * scale) - columnSpacing;
    }
}
