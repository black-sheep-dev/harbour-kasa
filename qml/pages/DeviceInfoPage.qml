import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

import "../components/"

Page {
    property var device

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("DeviceSettingsPage.qml"), {device: device})
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: DeviceManager.refresh()
            }
        }

        Column {
            id: column
            width:parent.width

            PageHeader {
                title: qsTr("Device Info")
            }

            SectionHeader {
                text: qsTr("General")
                font.pixelSize: Theme.fontSizeLarge
            }

            InfoItem {
                name: qsTr("Name")
                value: device.name
            }

            InfoItem {
                name: qsTr("Description")
                value: device.deviceName
            }

            InfoItem {
                name: qsTr("Model")
                value: device.deviceModel
            }

            InfoItem {
                name: qsTr("Device ID")
                value: device.deviceID
            }

            InfoItem {
                name: qsTr("HW Version")
                value: device.hardwareVersion
            }

            InfoItem {
                name: qsTr("Firmware")
                value: device.firmwareVersion
            }

            // ------------------------------------------------------------------------------------

            SectionHeader {
                text: qsTr("Network")
                font.pixelSize: Theme.fontSizeLarge
            }

            InfoItem {
                name: qsTr("Hostname")
                value: device.hostname
            }

            InfoItem {
                name: qsTr("MAC")
                value: device.macAddress
            }

            InfoItem {
                name: qsTr("RSSI")
                value: device.rssi + " dBm"
            }

            // ------------------------------------------------------------------------------------

            SectionHeader {
                text: qsTr("Energy")
                font.pixelSize: Theme.fontSizeLarge
                visible: device.features & Device.FeatureEnergy
            }

            InfoItem {
                name: qsTr("Current")
                value: Number(device.current).toLocaleString(Qt.locale()) + " A"
                visible: device.features & Device.FeatureEnergy
            }

            InfoItem {
                name: qsTr("Power")
                value: Number(device.power).toLocaleString(Qt.locale()) + " W"
                visible: device.features & Device.FeatureEnergy
            }

            InfoItem {
                name: qsTr("Voltage")
                value: Number(device.voltage).toLocaleString(Qt.locale()) + " V"
                visible: device.features & Device.FeatureEnergy
            }

            InfoItem {
                name: qsTr("Total")
                value: Number(device.totalConsumption).toLocaleString(Qt.locale()) + " kWh"
                visible: device.features & Device.FeatureEnergy
            }
        }
    }

    onStatusChanged: if (status == PageStatus.Activating) DeviceManager.refresh(device.hostname);
}
