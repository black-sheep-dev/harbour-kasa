import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

Page {
    property string hostname

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width
            spacing: Theme.paddingSmall

            PageHeader {
                //% "Send command"
                title: qsTrId("id-send-command")
            }

            TextArea {
                id: textArea
                width: parent.width
                height: page.height / 2
            }

            ButtonLayout {
                width: parent.width

                Button {
                    enabled: textArea.text.length > 0
                    //% "Send"
                    text: qsTrId("id-send");

                    onClicked: {
                        DeviceManager.sendCmd(hostname, textArea.text)
                    }
                }
                Button {
                    enabled: textArea.text.length > 0
                    //% "Clear"
                    text: qsTrId("id-clear");

                    onClicked: textArea.text = ""
                }
            }


        }
    }
}
