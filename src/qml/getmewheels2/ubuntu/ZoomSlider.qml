import QtQuick 2.0
import Ubuntu.Components 0.1

UbuntuShape {
    id: root
    color: "#22000000"
    implicitHeight: units.gu(5)
    height: implicitHeight

    property double radiusFactor: 1
    property int maxValue
    property int minValue
    property int value
    property bool pressed: scrollMouseArea.pressed

    onValueChanged: print("value is now", value)

    MouseArea {
        anchors.fill: parent
        onClicked: {
            print("yummi! Mouse presses taste delicious");
        }
    }

    Row {
        anchors.fill: parent
        anchors.margins: units.gu(0.5)
        spacing: units.gu(0.5)
        UbuntuShape {
            height: parent.height
            width: height
            color: "#55000000"
            Image {
                anchors.fill: parent
                anchors.margins: units.gu(1)
                source: "images/remove.svg"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(value > minValue) {
                        root.value -= 1;
                    }
                }
            }
        }

        Item {
            id: scrollArea
            width: parent.width - 2*parent.height - 2*parent.spacing
            height: parent.height
//            color: "#55FFFFFF"
            UbuntuShape {
                id: button
                color: "#55000000"
                height: parent.height
                width: height
                x: (parent.width - button.width) * (root.value - root.minValue) / (root.maxValue - root.minValue)
            }
            MouseArea {
                id: scrollMouseArea
                anchors.fill: parent
                onMouseXChanged: {
                    // x : width = (value - minValue) : (maxValue - minValue)
                    var value = mouseX * (root.maxValue - root.minValue) / width
                    print("value is", value, mouseX)
                    value = Math.max(value, minValue);
                    value = Math.min(value, maxValue);
                    //if(root.value != Math.round(value)) {
                        root.value = value;
                    //}

                }
            }
        }

        UbuntuShape {
            height: parent.height
            width: height
            color: "#55000000"
            Image {
                anchors.fill: parent
                anchors.margins: units.gu(1)
                source: "images/add.svg"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(value < maxValue) {
                        root.value += 1;
                    }
                }
            }
        }

    }
}
