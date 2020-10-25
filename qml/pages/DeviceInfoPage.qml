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
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("DeviceSettingsPage.qml"), {device: device})
            }
            MenuItem {
                text: qsTr("Energy Consumption")
                onClicked: pageStack.push(Qt.resolvedUrl("EnergyConsumptionChartPage.qml"), {device: device})
                visible: device.features & Device.FeatureEnergy
            }

            MenuItem {
                text: qsTr("Refresh")
                onClicked: DeviceManager.refresh()
            }
        }

        Column {
            id: column
            width:parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Device Info")
            }

            SectionHeader {
                text: qsTr("General")
                font.pixelSize: Theme.fontSizeLarge
            }

            DetailItem {
                label: qsTr("Name")
                value: device.name
            }

            DetailItem {
                label: qsTr("Description")
                value: device.deviceName
            }

            DetailItem {
                label: qsTr("Model")
                value: device.deviceModel
            }

            DetailItem {
                label: qsTr("Device ID")
                value: device.deviceID
            }

            DetailItem {
                label: qsTr("HW Version")
                value: device.hardwareVersion
            }

            DetailItem {
                label: qsTr("Firmware")
                value: device.firmwareVersion
            }

            // ------------------------------------------------------------------------------------

            SectionHeader {
                text: qsTr("Network")
                font.pixelSize: Theme.fontSizeLarge
            }

            DetailItem {
                label: qsTr("Hostname")
                value: device.hostname
            }

            DetailItem {
                label: qsTr("MAC")
                value: device.macAddress
            }

            DetailItem {
                label: qsTr("RSSI")
                value: device.rssi + " dBm"
            }

            // ------------------------------------------------------------------------------------

            SectionHeader {
                text: qsTr("Energy")
                font.pixelSize: Theme.fontSizeLarge
                visible: device.features & Device.FeatureEnergy
            }

            DetailItem {
                label: qsTr("Current")
                value: Number(device.current).toLocaleString(Qt.locale()) + " A"
                visible: device.features & Device.FeatureEnergy
            }

            DetailItem {
                label: qsTr("Power")
                value: Number(device.power).toLocaleString(Qt.locale()) + " W"
                visible: device.features & Device.FeatureEnergy
            }

            DetailItem {
                label: qsTr("Voltage")
                value: Number(device.voltage).toLocaleString(Qt.locale()) + " V"
                visible: device.features & Device.FeatureEnergy
            }

            DetailItem {
                label: qsTr("Total")
                value: Number(device.totalConsumption).toLocaleString(Qt.locale()) + " kWh"
                visible: device.features & Device.FeatureEnergy
            }
        }
    }

    onStatusChanged: if (status == PageStatus.Activating) DeviceManager.refresh(device.hostname);
}
