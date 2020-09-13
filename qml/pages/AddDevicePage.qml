import QtQuick 2.0
import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

Dialog {
    Column {
        width: parent.width

        DialogHeader {}

        TextField {
            id: hostnameField
            width: parent.width
            placeholderText: qsTr("Enter hostname")
            label: qsTr("Hostname")
        }
    }

    onDone: {
        if (result == DialogResult.Accepted)
            DeviceManager.addDevice(hostnameField.text)
    }
}

