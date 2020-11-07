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

            TextSwitch {
                id: debugSwitch

                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                text: qsTr("Debug API Traffic")
                description: qsTr("This option will log all traffic between devices and this app into Documents folder.")
                             + "\n"
                             + qsTr("The data will help to investigate errors or to improve this app.")

                Component.onCompleted: checked = DeviceManager.debug
                onCheckedChanged: DeviceManager.debug = checked
            }
        }
    }
}

