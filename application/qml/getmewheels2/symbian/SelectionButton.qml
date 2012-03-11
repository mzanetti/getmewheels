import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: subTitleButton

    property alias titleText: title.text
    property alias subTitleText: subTitle.text

    signal clicked

    height: 80
    width: parent.width

    Rectangle {
        id: background
        anchors.fill: parent
        visible: mouseArea.pressed
        color: "white"
        opacity: .2
    }

    MouseArea {
        id: mouseArea
        anchors.fill: background
        onClicked: {
            subTitleButton.clicked();
        }
    }

    Label {
        id: title

        anchors {
            left: parent.left
            leftMargin: 10
            rightMargin: 10
            topMargin: 10
            top: parent.top
        }

        font.bold: true
        text: "-"
    }

    Label {
        id: subTitle
        anchors {
            top:  title.bottom
            topMargin: 10
            left: parent.left
            leftMargin: 10
            rightMargin: 10
            right: icon.left
        }
        text: ""
    }

    Rectangle {
        id: icon

        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }

        height: 8
        width: 8
        rotation: 45
        color: "white"
        Rectangle {
            anchors.centerIn: parent
            width: 10
            height: 2
            color: "black"
            rotation: 45 + 90
        }
    }
}
