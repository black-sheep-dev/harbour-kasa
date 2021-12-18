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
                //% "Factory Reset"
                text: qsTrId("id-factory-reset")
                //enabled: expertSwitch.checked
                enabled: false
                //% "Starting factory reset"
                onClicked: remorse.execute(qsTrId("id-starting-factory-reset"), function() {DeviceManager.reset(device.hostname)})
            }
            MenuItem {
                //% "Reset energy statistic data"
                text: qsTrId("id-reset-energy-data")
                enabled: expertSwitch.checked
                //% "Starting statistic reset"
                onClicked: remorse.execute(qsTrId("id-starting-statistic-reset"), function() {DeviceManager.resetEnergyStat(device.hostname)})
            }
            MenuItem {
                enabled: cloudServerUrlField.acceptableInput && macAddressField.acceptableInput

                //% "Save"
                text: qsTrId("id-save")
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
            spacing: Theme.paddingMedium

            PageHeader {
                //% "Expert Settings"
                title: qsTrId("id-expert-settings")
            }

            TextSwitch {
                id: expertSwitch

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                checked: false
                //% "Enable expert settings"
                text: qsTrId("id-enable-expert-settings")
                //% "Only enable expert settings when you know what you are doing! You can brick your device! I have warned you!"
                description: qsTrId("id-enable-expert-settings-info")
            }

            SectionHeader {
                //% "Hardware"
                text: qsTrId("id-hardware")
                font.pixelSize: Theme.fontSizeMedium
                visible: expertSwitch.checked
            }

            TextField {
                id: macAddressField

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                readOnly: !expertSwitch.checked
                visible: expertSwitch.checked

                //% "MAC address"
                label: qsTrId("id-mac-address")
                //% "Enter MAC address"
                placeholderText: qsTrId("id-enter-mac-address")
                text: device.macAddress

                validator: RegExpValidator {
                    regExp: /^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$/
                }
            }

            SectionHeader {
                //% "Cloud"
                text: qsTrId("id-cloud")
                font.pixelSize: Theme.fontSizeMedium

                visible: expertSwitch.checked
            }

            TextField {
                id: cloudServerUrlField

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                readOnly: !expertSwitch.checked
                visible: expertSwitch.checked

                //% "Cloud Server URL"
                label: qsTrId("id-cloud-server-url")
                //% "Enter Cloud Server URL"
                placeholderText: qsTrId("id-enter-cloud-server-url")
                text: device.cloudServer

                inputMethodHints: Qt.ImhUrlCharactersOnly
                validator: RegExpValidator {
                    regExp: /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,8}(:[0-9]{1,5})?(\/.*)?$|^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$|[a-zA-Z0-9-_]{1,}/gm
                }
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                visible: expertSwitch.checked

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.highlightColor
                //% "Changing the cloud server url will prevent the device from sending data home (not yours). Hello privacy!"
                text: qsTrId("id-cloud-url-info")
            }
            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                visible: expertSwitch.checked

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.highlightColor
                //% "The default url is 'devs.tplinkcloud.com'"
                text: qsTrId("id-cloud-default-url-info")
            }
        }
    }
}
