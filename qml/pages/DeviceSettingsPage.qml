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
            MenuItem {
                text: qsTr("Save")
                onClicked: {
                    DeviceManager.setCloudServer(device.hostname, cloudServerUrlField.text)
                    DeviceManager.setDeviceAlias(device.hostname, aliasField.text)
                    DeviceManager.setDeviceMacAddress(device.hostname, macAddressField.text)
                }
            }
        }

        Column {
            id: column
            width:parent.width

            PageHeader {
                title: qsTr("Device Settings")
            }

            TextField {
                id: aliasField
                width: parent.width
                label: qsTr("Device Alias")
                placeholderText: qsTr("Enter device alias");
                text: device.name
            }

            TextSwitch {
                id: expertSwitch
                checked: false
                text: qsTr("Enable expert settings")
                description: qsTr("Only enable expert settings when you know what you are doing! You can brick your device! I have warned you!")
            }

            SectionHeader {
                text: qsTr("Hardware")
                font.pixelSize: Theme.fontSizeMedium
            }

            TextSwitch {
                id: ledSwitch
                checked: device.ledOn

                text: qsTr("Turn LED on/off")

                onClicked: {
                    DeviceManager.toggleLED(device.hostname)
                }
            }

            Item {
                height: Theme.paddingMedium
                width: 1
            }

            TextField {
                id: macAddressField

                width: parent.width

                readOnly: !expertSwitch.checked
                visible: expertSwitch.checked

                label: qsTr("MAC address")
                placeholderText: qsTr("Enter MAC address")
                text: device.macAddress

                validator: RegExpValidator {
                    regExp: /^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$/
                }
            }

            SectionHeader {
                text: qsTr("Cloud")
                font.pixelSize: Theme.fontSizeMedium

                visible: expertSwitch.checked
            }

            TextSwitch {
                id: cloudBindSwitch
                checked: device.cloudRegistration

                visible: expertSwitch.checked
                enabled: device.cloudRegistration

                text: checked ? qsTr("Device registered in cloud") : qsTr("Device is not registered in cloud")
                description: qsTr("You can register or unregister the device from TP-Link cloud. For registration you need to provide username and password.")
            }

            Item {
                height: Theme.paddingMedium
                width: 1
            }


            TextField {
                id: cloudUsernameField
                width: parent.width

                readOnly: cloudBindSwitch.checked
                visible: expertSwitch.checked

                label: qsTr("Username")
                placeholderText: qsTr("Enter username")

                text: device.cloudUsername

                inputMethodHints: Qt.ImhEmailCharactersOnly
            }

            TextField {
                id: cloudPasswordField
                width: parent.width

                visible: !cloudBindSwitch.checked

                label: qsTr("Password")
                placeholderText: qsTr("Enter password")

                echoMode: TextInput.Password
            }

            TextField {
                id: cloudServerUrlField
                width: parent.width

                readOnly: !expertSwitch.checked
                visible: expertSwitch.checked

                label: qsTr("Cloud Server URL")
                placeholderText: qsTr("Enter Cloud Server URL")
                text: device.cloudServer

                inputMethodHints: Qt.ImhUrlCharactersOnly
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin

                visible: expertSwitch.checked

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                text: qsTr("Changing the cloud server url will prevent the device from sending data home (not yours). Hello privacy!")
            }
            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin

                visible: expertSwitch.checked

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                text: qsTr("The default url is 'devs.tplinkcloud.com'")
            }
        }
    }
}
