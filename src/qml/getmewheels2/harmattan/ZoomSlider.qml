// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

Rectangle {
    id: root
    color: "black"
    border.color: "white"
    border.width: 2
    opacity: .8
    radius: width / 2 * radiusFactor

    property double radiusFactor: 1
    property int maxValue
    property int minValue
    property int value
    signal valueChanged(int value)

    onValueChanged: {
        //button.y = y + (maxValue - value) * (scrollArea.height-button.height) / (maxValue - minValue)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            print("yummi! Mouse presses taste delicious");
        }
    }

    Label {
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        text: "+"
        font.pixelSize: 32
        MouseArea {
            anchors.fill: parent
            anchors.margins: -20
            onClicked: {
//                rumbleEffect.start();
                if(value < maxValue) {
                    valueChanged(root.value + 1);
                }
            }
        }
    }
    Label {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        text: "-"
        font.pixelSize: 32
        MouseArea {
            anchors.fill: parent
            anchors.margins: -20
            onClicked: {
//                rumbleEffect.start();
                if(value > minValue) {
                    valueChanged(root.value - 1);
                }
            }
        }
    }

    Rectangle {
        id: scrollArea
        anchors.fill: parent
        anchors.topMargin: 50
        anchors.bottomMargin: 50
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        radius: width / 2 * root.radiusFactor
        color: "white"
        border.color: "black"
        border.width: 2
        opacity: .8


        Rectangle {
            id: button
            color: "black"
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: width
            radius: width / 2 * root.radiusFactor
            y: root.y + (root.maxValue - root.value) * (scrollArea.height-button.height) / (root.maxValue - root.minValue)
        }

        MouseArea {
            anchors.fill: parent
            anchors.leftMargin: -10
            anchors.rightMargin: -10
            onMouseYChanged: {
//                print("button moved", mouseY)

                var value = maxValue - ((mouseY-y) * (maxValue-minValue) / height)
                value = Math.max(value, minValue);
                value = Math.min(value, maxValue);
                if(root.value != Math.round(value)) {
                    //root.value = value;
                    valueChanged(value)
//                    print("value is", root.value);
//                    button.y = y + (maxValue - root.value) * (height-button.height) / (maxValue-minValue)
                }

            }
        }
    }


}
