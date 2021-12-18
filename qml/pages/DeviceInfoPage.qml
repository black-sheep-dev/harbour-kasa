import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

import "../components/"

Page {
    property Device device

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                //% "Settings"
                text: qsTrId("id-settings")
                onClicked: pageStack.push(Qt.resolvedUrl("DeviceSettingsPage.qml"), {device: device})
            }
            MenuItem {
                //% "Energy Consumption"
                text: qsTrId("id-enery-consumtion")
                onClicked: pageStack.push(Qt.resolvedUrl("EnergyConsumptionChartPage.qml"), {device: device})
                visible: device.features & Device.FeatureEnergy
            }

            MenuItem {
                //% "Refresh"
                text: qsTrId("id-refresh")
                onClicked: DeviceManager.refresh()
            }
        }

        Column {
            id: column
            width:parent.width
            spacing: Theme.paddingSmall

            PageHeader {
                //% "Device Info"
                title: qsTrId("id-device-info")
            }

            TextSwitch {
                id: onSwitch
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                //% "Turn on/off"
                text: qsTrId("id-turn-on-off")

                Component.onCompleted: checked = Device.on

                onCheckedChanged: {
                    device.on = checked
                    DeviceManager.toggleOn(device.hostname)
                }
            }

            Slider {
                visible: Device.deviceType === Device.KL110

                width: parent.width

                minimumValue: 0
                maximumValue: 100
                stepSize: 1

                //% "Brightness"
                label: qsTrId("id-brightness")
                valueText: value + " %"

                Component.onCompleted: value = device.brightness

                onPressedChanged: {
                    if (pressed)
                        return;

                    device.brightness = value
                    DeviceManager.setBrightness(device.hostname, value)

                }
            }

            SectionHeader {
                //% "General"
                text: qsTrId("id-general")
                font.pixelSize: Theme.fontSizeLarge
            }

            DetailItem {
                //% "Name"
                label: qsTrId("id-name")
                value: device.name
            }

            DetailItem {
                //% "Description"
                label: qsTrId("id-description")
                value: device.deviceName
            }

            DetailItem {
                //% "Model"
                label: qsTrId("id-model")
                value: device.deviceModel
            }

            DetailItem {
                //% "Device ID"
                label: qsTrId("id-device-id")
                value: device.deviceID
            }

            DetailItem {
                //% "HW Version"
                label: qsTrId("id-hw-version")
                value: device.hardwareVersion
            }

            DetailItem {
                //% "Firmware"
                label: qsTrId("id-firmware")
                value: device.firmwareVersion
            }

            // ------------------------------------------------------------------------------------

            SectionHeader {
                //% "Network"
                text: qsTrId("id-network")
                font.pixelSize: Theme.fontSizeLarge
            }

            DetailItem {
                //% "Hostname"
                label: qsTrId("id-hostname")
                value: device.hostname
            }

            DetailItem {
                //% "MAC"
                label: qsTrId("id-mac")
                value: device.macAddress
            }

            DetailItem {
                //% "RSSI"
                label: qsTrId("id-rssi")
                value: device.rssi + " dBm"
            }

            // ------------------------------------------------------------------------------------

            SectionHeader {
                //: Electric energy
                //% "Energy"
                text: qsTrId("id-energy")
                font.pixelSize: Theme.fontSizeLarge
                visible: device.features & Device.FeatureEnergy
            }

            DetailItem {
                //: Electric current
                //% "Current"
                label: qsTrId("id-current")
                value: Number(device.current).toLocaleString(Qt.locale()) + " A"
                visible: device.features & Device.FeatureEnergy
            }

            DetailItem {
                //: Electric power
                //% "Power"
                label: qsTrId("id-power")
                value: Number(device.power).toLocaleString(Qt.locale()) + " W"
                visible: device.features & Device.FeatureEnergy
            }

            DetailItem {
                //: Electric voltage
                //% "Voltage"
                label: qsTrId("id-voltage")
                value: Number(device.voltage).toLocaleString(Qt.locale()) + " V"
                visible: device.features & Device.FeatureEnergy
            }

            DetailItem {
                //% "Total"
                label: qsTrId("id-total")
                value: Number(device.totalConsumption).toLocaleString(Qt.locale()) + " kWh"
                visible: device.features & Device.FeatureEnergy
            }

            Item {
                width: 1
                height: Theme.paddingSmall
            }
        }
    }

    onStatusChanged: if (status == PageStatus.Activating) DeviceManager.refresh(device.hostname);
}
