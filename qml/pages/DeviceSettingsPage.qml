import QtQuick 2.0
import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

Page {
    property var device: DeviceManager.deviceListModel().deviceAt(index)

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("Restart")
                onClicked: DeviceManager.restart(device.hostname)
            }
        }

        Column {
            id: column
            width:parent.width

            PageHeader {
                title: qsTr("Device Settings")
            }

            SectionHeader {
                text: qsTr("Hardware")
                font.pixelSize: Theme.fontSizeMedium
            }

            Row {
                width: parent.width - 2 * x
                x: Theme.horizontalPageMargin
                height: Theme.itemSizeMedium

                Label {
                    width: parent.width - ledSwitch.width
                    text: qsTr("Turn LED on/off")

                    anchors.verticalCenter: parent.verticalCenter
                }

                Item {
                    width: Theme.paddingMedium
                    height: 1
                }

                Switch {
                    id: ledSwitch
                    checked: device.ledOn

                    onClicked: {
                        DeviceManager.toggleLED(device.hostname)
                    }

                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
