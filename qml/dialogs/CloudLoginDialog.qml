import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    property string username
    property string password

    id: dialog

    Column {
        width: parent.width

        DialogHeader { }

        TextField {
            id: usernameField
            width: parent.width

            focus: true

            label: qsTr("Username")
            placeholderText: qsTr("Enter username")

            inputMethodHints: Qt.ImhEmailCharactersOnly

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: passwordField.focus = true
        }

        PasswordField {
            id: passwordField

            width: parent.width

            label: qsTr("Password")
            placeholderText: qsTr("Enter password")

            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: dialog.accept()
        }
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            username = usernameField.text
            password = passwordField.text
        }
    }
}
