import QtQuick 2.0
import QtQuick 2.0
import Sailfish.Silica 1.0

import "../dialogs/"

import org.nubecula.harbour.kasa 1.0

Page {
    property var device

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("Expert Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("DeviceExpertSettingsPage.qml"), {device: device})
            }
            MenuItem {
                text: qsTr("Restart Device")
                onClicked: DeviceManager.restart(device.hostname)
            }
            MenuItem {
                text: qsTr("Save")
                onClicked: {
                    DeviceManager.setDeviceAlias(device.hostname, aliasField.text)
                }
            }
        }

        RemorseItem { id: remorseCloudBind }

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Device Settings")
            }

            TextField {
                id: aliasField

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                label: qsTr("Device Alias")
                placeholderText: qsTr("Enter device alias");
                text: device.name
            }

            SectionHeader {
                text: qsTr("Hardware")
                font.pixelSize: Theme.fontSizeMedium
            }

            TextSwitch {
                id: ledSwitch

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                checked: device.ledOn

                text: qsTr("Turn LED on/off")

                onClicked: {
                    DeviceManager.toggleLED(device.hostname)
                }
            }

            SectionHeader {
                text: qsTr("Cloud")
                font.pixelSize: Theme.fontSizeMedium
            }

            TextSwitch {
                id: cloudBindSwitch

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                checked: device.cloudRegistration

                text: checked ? qsTr("Device registered in cloud") : qsTr("Device is not registered in cloud")
                description: qsTr("You can register or unregister the device from TP-Link cloud. For registration you need to provide username and password.")

                onClicked: {
                    if (!checked) {
                        cloudBindSwitch.checked = true
                        remorseCloudBind.execute(cloudBindSwitch,
                                                        qsTr("Leaving cloud"),
                                                        function () {
                                                            DeviceManager.unregisterDeviceFromCloud(device.hostname);
                                                            cloudBindSwitch.checked = false
                                                        })
                    } else {
                        var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/CloudLoginDialog.qml"))

                        dialog.accepted.connect(function () {
                            DeviceManager.registerDeviceOnCloud(device.hostname,
                                                                dialog.username,
                                                                dialog.password)
                        })
                    }
                }
            }

            TextField {
                id: cloudUsernameField

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                readOnly: true
                visible: cloudBindSwitch.checked

                label: qsTr("Username")
                placeholderText: qsTr("Enter username")

                text: device.cloudUsername
            }
        }
    }
}
