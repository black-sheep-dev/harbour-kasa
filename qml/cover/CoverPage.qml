import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

CoverBackground {
    property int id: 0
    property var device: DeviceManager.deviceListModel().deviceAt(id)

    Image {
        source: device.on ? "qrc:///icons/IOT.SMARTPLUGSWITCH_GREEN" : "qrc:///icons/IOT.SMARTPLUGSWITCH"
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        sourceSize.width: Theme.iconSizeExtraLarge * 2
        sourceSize.height: Theme.iconSizeExtraLarge * 2
        opacity: device.on ? 0.8 : 0.1
    }

    Label {
        x: Theme.horizontalPageMargin
        width: parent.width - 2 * Theme.horizontalPageMargin
        anchors.top: parent.top;
        anchors.topMargin: Theme.horizontalPageMargin
        text: device ? device.name : qsTr("No devices available")
        wrapMode: Label.WordWrap
    }

    CoverActionList {
        id: coverAction
        enabled: device ? true : false

        CoverAction {
            iconSource: "image://theme/icon-cover-previous"
            onTriggered: {
                id--

                if (id < 0)
                    id = DeviceManager.deviceListModel().rowCount() - 1

                device = DeviceManager.deviceListModel().deviceAt(id)
                DeviceManager.refresh(device.hostname)
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-location"
            onTriggered: DeviceManager.toggleOn(device.hostname)
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: {
                id++

                if (id >= DeviceManager.deviceListModel().rowCount())
                    id = 0

                device = DeviceManager.deviceListModel().deviceAt(id)
                DeviceManager.refresh(device.hostname)
            }
        }
    }

    onVisibleChanged: {
        id = 0
        device = DeviceManager.deviceListModel().deviceAt(id)
    }
}
