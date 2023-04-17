import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#EEEEEE"
    property string text: input.text
    property string placeHolder: ""
    property var validator

    TextInput {
       id: input
       anchors.fill: parent
       color: "black"
       verticalAlignment: Text.AlignVCenter
       validator: parent.validator

       MouseArea {
           anchors.fill: parent
           cursorShape: Qt.IBeamCursor
           onPressed: parent.focus = true
       }
    }
    Text {
        anchors.fill: parent
        text: placeHolder
        color: "#9E9E9E"
        visible: !input.text
        verticalAlignment: Text.AlignVCenter
    }
}
