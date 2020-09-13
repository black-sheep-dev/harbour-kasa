import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width

            PageHeader {
                title: qsTr("Settings")
            }

            TextField {
                width: parent.width
                label: qsTr("Update Interval in msecs")
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator {bottom: 1000}
                text: "1000"

            }
        }
    }
}

