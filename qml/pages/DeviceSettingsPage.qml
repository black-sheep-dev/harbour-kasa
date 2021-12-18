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
                //% "Expert Settings"
                text: qsTrId("id-expert-settings")
                onClicked: pageStack.push(Qt.resolvedUrl("DeviceExpertSettingsPage.qml"), {device: device})
            }
            MenuItem {
                //% "Restart Device"
                text: qsTrId("id-restart-device")
                onClicked: DeviceManager.restart(device.hostname)
            }
            MenuItem {
                //% "Save"
                text: qsTrId("id-save")
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
                //% "Device Settings"
                title: qsTrId("id-device-settings")
            }

            TextField {
                id: aliasField

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                //% "Device Alias"
                label: qsTrId("id-device-alias")
                //% "Enter device alias"
                placeholderText: qsTrId("id-enter-device-alias");
                text: device.name
            }

            SectionHeader {
                //% "Hardware"
                text: qsTrId("Hardware")
                font.pixelSize: Theme.fontSizeMedium
            }

            TextSwitch {
                id: ledSwitch

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                checked: device.ledOn

                //% "Turn LED on/off"
                text: qsTrId("id-toggle-led")

                onClicked: {
                    DeviceManager.toggleLED(device.hostname)
                }
            }

            SectionHeader {
                //% "Cloud"
                text: qsTrId("id-cloud")
                font.pixelSize: Theme.fontSizeMedium
            }

            TextSwitch {
                id: cloudBindSwitch

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                checked: device.cloudRegistration

                text: checked ?
                          //% "Device registered in cloud"
                          qsTrId("id-device-cloud-registered") :
                          //% "Device is not registered in cloud"
                          qsTrId("id-device-cloud-not-registered")

                //% "You can register or unregister the device from TP-Link cloud. For registration you need to provide username and password."
                description: qsTrId("id-register-info-text")

                onClicked: {
                    if (!checked) {
                        cloudBindSwitch.checked = true
                        remorseCloudBind.execute(cloudBindSwitch,
                                                        //% "Leaving cloud"
                                                        qsTrId("id-leaving-cloud"),
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

                //% "Username"
                label: qsTrId("id-username")
                //% "Enter username"
                placeholderText: qsTrId("id-enter-username")

                text: device.cloudUsername
            }
        }
    }
}
