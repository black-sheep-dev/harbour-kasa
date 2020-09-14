import QtQuick 2.0
import QtQuick 2.0
import Sailfish.Silica 1.0

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
                text: qsTr("Factory Reset")
                //enabled: expertSwitch.checked
                enabled: false
                onClicked: remorse.execute(qsTr("Starting factory reset"), function() {DeviceManager.reset(device.hostname)})
            }
            MenuItem {
                text: qsTr("Reset energy statistic data")
                enabled: expertSwitch.checked
                onClicked: remorse.execute(qsTr("Starting statistic reset"), function() {DeviceManager.resetEnergyStat(device.hostname)})
            }
            MenuItem {
                text: qsTr("Save")
                onClicked: {
                    DeviceManager.setCloudServer(device.hostname, cloudServerUrlField.text)
                    DeviceManager.setDeviceMacAddress(device.hostname, macAddressField.text)
                }
            }
        }

        RemorsePopup { id: remorse }

        Column {
            id: column
            width:parent.width

            PageHeader {
                title: qsTr("Expert Settings")
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
                visible: expertSwitch.checked
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
