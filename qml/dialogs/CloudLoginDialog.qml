import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    property string username
    property string password

    id: dialog

    canAccept: usernameField.acceptableInput

    Column {
        width: parent.width

        DialogHeader {
            //% "Login"
            acceptText: qsTrId("id-login")
            //% "Cloud Login"
            title: qsTrId("id-cloud-login")
        }

        TextField {
            id: usernameField

            x: Theme.horizontalPageMargin
            width: parent.width - 2*x

            focus: true

            //% "Username"
            label: qsTrId("id-username")
            //% "Enter username"
            placeholderText: qsTrId("id-enter-username")

            inputMethodHints: Qt.ImhEmailCharactersOnly
            validator: RegExpValidator {
                regExp: /[^@ \t\r\n]+@[^@ \t\r\n]+\.[^@ \t\r\n]+/
            }

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: passwordField.focus = true
        }

        PasswordField {
            id: passwordField

            x: Theme.horizontalPageMargin
            width: parent.width - 2*x

            //% "Password"
            label: qsTrId("id-password")
            //% "Enter password"
            placeholderText: qsTrId("id-enter-password")

            validator: RegExpValidator {
                regExp: /(^$)|(\s+$)/
            }

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
