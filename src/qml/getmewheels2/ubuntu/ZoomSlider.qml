import QtQuick 2.0
import Ubuntu.Components 0.1

Rectangle {
    id: root
    color: "black"
    border.color: "white"
    border.width: units.dp(1)
    opacity: .8
    radius: width / 2 * radiusFactor

    property double radiusFactor: 1
    property int maxValue
    property int minValue
    property int value

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
        anchors.topMargin: units.gu(0.5)
        anchors.horizontalCenter: parent.horizontalCenter
        text: "+"
        fontSize: "large"
        MouseArea {
            anchors.fill: parent
            anchors.margins: -units.gu(2)
            onClicked: {
//                rumbleEffect.start();
                if(value < maxValue) {
                    root.value += 1;
                }
            }
        }
    }
    Label {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: units.gu(0.5)
        anchors.horizontalCenter: parent.horizontalCenter
        text: "-"
        fontSize: "large"
        MouseArea {
            anchors.fill: parent
            anchors.margins: -units.gu(2)
            onClicked: {
//                rumbleEffect.start();
                if(value > minValue) {
                    root.value -= 1;
                }
            }
        }
    }

    Rectangle {
        id: scrollArea
        anchors.fill: parent
        anchors.topMargin: units.gu(3)
        anchors.bottomMargin: units.gu(3)
        anchors.leftMargin: units.dp(3)
        anchors.rightMargin: units.dp(3)
        radius: width / 2 * root.radiusFactor
        color: "white"
        border.color: "black"
        border.width: units.dp(1)
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
            anchors.leftMargin: -units.gu(1)
            anchors.rightMargin: -units.gu(1)
            onMouseYChanged: {
                var value = maxValue - ((mouseY-y) * (maxValue-minValue) / height)
                value = Math.max(value, minValue);
                value = Math.min(value, maxValue);
                if(root.value != Math.round(value)) {
                    root.value = value;
                }

            }
        }
    }


}
