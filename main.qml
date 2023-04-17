import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import SanityCheckerState 1.0

ApplicationWindow {
    id: root
    width: 500
    height: 250
    maximumWidth: width
    maximumHeight: height
    minimumWidth: width
    minimumHeight: height
    visible: true
    title: qsTr("Симуляция сработки верхней уставки")
    readonly property int textInputWidth: 150
    readonly property int textInputHeight: 30
    readonly property int buttonWidth: 150
    readonly property int buttonHeight: 30

    RowLayout {
        x: (root.width-width)/2
        y: (root.height-height)/2

        spacing: 10
        ColumnLayout {
            InputField {
                id: fieldBA
                placeHolder: "BA"
                validator: RegExpValidator{
                    regExp: /\d+(?:\.\d+)?/
                }
                width: textInputWidth
                height: textInputHeight
            }

            InputField {
                id: fieldRMV
                placeHolder: "RMV"
                validator: RegExpValidator{
                    regExp: /\d+(?:\.\d+)?/
                }
                width: textInputWidth
                height: textInputHeight
            }

            InputField {
                id: fieldRDV
                placeHolder: "RDV, мс"
                validator: IntValidator{}
                width: textInputWidth
                height: textInputHeight
            }

            Button{
                width: buttonWidth
                height: buttonHeight
                text: "Установить параметры"
                onPressed: {
                    if(fieldBA.text !== "") {
                        sanityChecker.BA = parseFloat(fieldBA.text);
                    }

                    if(fieldRMV.text !== "") {
                        sanityChecker.RMV = parseFloat(fieldRMV.text);
                    }

                    if(fieldRDV.text !== "") {
                        sanityChecker.RDV = parseInt(fieldRDV.text);
                    }
                }
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignBottom
            Label {
                id: stateLabel
                color: "white"
                Component.onCompleted: {
                    sanityChecker.currentStateChanged.connect(function(){
                        switch(sanityChecker.currentState) {
                        case SanityCheckerState.PASSIVE:
                            stateLabel.color = "#43A047";
                            stateLabel.text = "PASSIVE";
                        break;
                        case SanityCheckerState.ACTIVE:
                            stateLabel.color = "#C62828";
                            stateLabel.text = "ACTIVE";
                        break;
                        }
                    });
                }
            }

            InputField {
                id: fieldValue
                placeHolder: "Значение"
                validator: RegExpValidator{
                    regExp: /\d+(?:\.\d+)?/
                }
                width: textInputWidth
                height: textInputHeight
            }
            Button{
                width: buttonWidth
                height: buttonHeight
                text: "Отправить значение"
                onPressed: {
                    if(fieldValue.text !== "") {
                        sanityChecker.checkAndSetState(parseFloat(fieldValue.text));
                    }
                }
            }
        }
    }
}
