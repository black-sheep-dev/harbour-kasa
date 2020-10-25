import QtQuick 2.0
import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

Dialog {
    id: dialog

    canAccept: hostnameField.acceptableInput

    Column {
        width: parent.width

        DialogHeader {
            acceptText: qsTr("Add")
            title: qsTr("Add new device")
        }

        TextField {
            id: hostnameField
            width: parent.width
            placeholderText: qsTr("Enter hostname")
            label: qsTr("Hostname")

            focus: true
            inputMethodHints: Qt.ImhUrlCharactersOnly
            validator: RegExpValidator {
                regExp: /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,8}(:[0-9]{1,5})?(\/.*)?$|^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$|[a-zA-Z0-9-_]{1,}/gm
            }
        }
    }

    onDone: {
        if (result == DialogResult.Accepted)
            DeviceManager.addDevice(hostnameField.text)
    }
}

