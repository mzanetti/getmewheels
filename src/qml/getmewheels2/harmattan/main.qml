import QtQuick 1.1
import com.nokia.meego 1.0
//import QtMobility.feedback 1.1
import GetMeWheels 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage
    property bool gpsAvailable: false

    Component.onCompleted: {
        theme.inverted = true;
    }

//    Component.onCompleted: {
//        theme.inverted = true;
//    }

    GmwEngine {
        id: gmwEngine
    }

    GmwModel {
        id: gmwModel

        engine: gmwEngine
    }

//    HapticsEffect {
//        id: rumbleEffect
//        attackIntensity: 0.0
//        attackTime: 250
//        intensity: 1.0
//        duration: 75
//        fadeTime: 250
//        fadeIntensity: 0.0
//    }

    MainPage {
        id: mainPage
        orientationLock: PageOrientation.LockPortrait
    }


    Dialog {
        id: aboutDialog

        title: Item {
            height: 200
            width: parent.width
            Image {
                id: icon
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                source: "/usr/share/icons/hicolor/80x80/apps/getmewheels280.png"
            }
            Text {
                anchors.top: icon.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 20
                color: "white"
                font.pixelSize: 32
                text: "GetMeWheels 1.2"
            }

        }

        content:Item {
            id: name
            height: 200
            width: parent.width
            Label {
                id: text
                font.pixelSize: 22
                anchors.centerIn: parent
                color: "white"
                text: "Copyright\n Michael Zanetti\n  michael_zanetti@gmx.net\n Christian Fetzer\n  fetzerch@googlemail.com"
            }
        }

        buttons {
            Button {
                id: closeButton
                text: "close"; onClicked: aboutDialog.close()
            }
            Button {
                id: donateButton
                anchors.top: closeButton.bottom
                anchors.topMargin: 10
                text: "donate";

                onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=UPA7TFA73GKFY")
            }
            Button {
                anchors.top: donateButton.bottom
                anchors.topMargin: 10
                text: "flattr";
                onClicked: Qt.openUrlExternally("https://flattr.com/thing/465960/GetMeWheels")
            }
        }
    }

}
