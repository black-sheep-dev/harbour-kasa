import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        PullDownMenu {
            MenuItem {
                //% "About"
                text: qsTrId("id-about")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                //% "Settings"
                text: qsTrId("id-settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }

            MenuItem {
                //% "Add Device"
                text: qsTrId("id-add-device")
                onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/AddDeviceDialog.qml"))
            }
        }

        id: listView

        header: PageHeader {
            //% "Devices"
            title: qsTrId("id-devices")
        }

        anchors.fill: parent

        model: DeviceManager.deviceListModel()


        delegate: ListItem {
            id: delegate

            width: parent.width
            contentHeight: Theme.itemSizeExtraLarge

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: deviceIcon

                    height: parent.height - 2 * Theme.paddingSmall
                    width: parent.height - 2 * Theme.paddingSmall
                    anchors.verticalCenter: parent.verticalCenter

                    opacity: available ? 1.0 : 0.3

                    source: {
                        switch (device_type) {
                        case Device.HS100:
                        case Device.HS110:
                            return "qrc:///icons/IOT.SMARTPLUGSWITCH"

                        case Device.KL110:
                            return "qrc:///icons/IOT.SMARTBULB"

                        default:
                            return "qrc:///icons/IOT.SMARTPLUGSWITCH"
                        }
                    }
                }

                Item {
                    id: spacer

                    width:Theme.paddingMedium
                    height:1

                }

                Column {
                    id: data

                    width: parent.width - deviceIcon.width - activateSwitch.width

                    anchors.verticalCenter: deviceIcon.verticalCenter

                    Label{
                        id: text
                        width: parent.width
                        elide: Text.ElideRight
                        text: name
                        color: pressed ? Theme.secondaryHighlightColor:Theme.highlightColor
                        font.pixelSize: Theme.fontSizeLarge
                    }
                    Label{
                        //% "Device offline or network error"
                        text: available ? hostname : qsTrId("id-device-offline-or-error");
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium

                    }
                }

                IconButton {
                    visible: !available
                    icon.source: "image://theme/icon-m-refresh"

                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: DeviceManager.getSystemInfo(hostname)
                 }

                Switch {
                    id: activateSwitch
                    checked: on

                    visible: available

                    onClicked: {
                        checked: on
                        DeviceManager.toggleOn(hostname)
                    }

                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            menu: ContextMenu {
                MenuItem {
                    //% "Delete"
                    text: qsTrId("id-delete")
                    onClicked: delegate.remorseDelete(function() {DeviceManager.removeDevice(model.hostname)})
                }
                MenuItem {
                    visible: DeviceManager.debug
                    //% "Send commands"
                    text: qsTrId("id-send-commands")
                    onClicked: pageStack.push(Qt.resolvedUrl("SendCommandPage.qml"), {hostname: hostname})
                }
            }

            onClicked: {
                if (available)
                    pageStack.push(Qt.resolvedUrl("DeviceInfoPage.qml"), {device: DeviceManager.deviceListModel().deviceAt(index)}) 
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            //% "No devices available"
            text: qsTrId("id-no-devices-available")
            //% "Pull down to add a new device"
            hintText: qsTrId("id-no-devices-hint")
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: if (status == PageStatus.Activating) DeviceManager.refresh();
}
