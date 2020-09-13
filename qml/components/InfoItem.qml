import QtQuick 2.0
import Sailfish.Silica 1.0

Row {
    property string name
    property string value

    width: parent.width - 2 * x
    x: Theme.horizontalPageMargin
    height: Theme.itemSizeExtraSmall

    Label {
        width: parent.width * 0.3
        anchors.verticalCenter: parent.verticalCenter
        color: Theme.highlightColor

        text: name
        font.pixelSize: Theme.fontSizeSmall
    }

    Label {
        anchors.verticalCenter: parent.verticalCenter
        text: value
        font.pixelSize: Theme.fontSizeSmall
        wrapMode: Label.WordWrap
    }
}



