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
                //% "Settings"
                title: qsTrId("id-settings")
            }

            TextSwitch {
                id: debugSwitch

                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                //% "Debug API Traffic"
                text: qsTrId("id-debug-api-traffic")
                //% "This option will log all traffic between devices and this app into Documents folder."
                description: qsTrId("id-debug-info-1")
                             + "\n"
                            //% "The data will help to investigate errors or to improve this app."
                             + qsTrId("id-debug-info-2")

                Component.onCompleted: checked = DeviceManager.debug
                onCheckedChanged: DeviceManager.debug = checked
            }
        }
    }
}

