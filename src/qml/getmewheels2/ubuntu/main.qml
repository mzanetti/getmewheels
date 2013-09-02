import QtQuick 2.0
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components 0.1
import QtLocation 5.0
import GetMeWheels 1.0

MainView {
    id: appWindow

    headerColor: "#0249b3"
    backgroundColor: "#0365bf"
    footerColor: "#06bce4"

     //

/*    tools: ToolbarActions {
        Action {
            text: "Settings"
            onTriggered: {
                var component = Qt.createComponent("SettingsSheet.qml");
                PopupUtils.open(component);
            }
        }
        Action {
            text: "Refresh"
            onTriggered: {
                gmwModel.clearVehicles();
                gmwEngine.refreshVehicles(false);
            }
        }
    }
*/

    Tabs {
        anchors.fill: parent

        Tab {
            title: "Map"
            id: mapTab
            page: MainPage {}
        }

        Tab {
            title: "List"
            page: ItemList {
                model: gmwModel
                onlyBooked: false
            }
        }
    }

    GmwEngine {
        id: gmwEngine

        onLocationNameChanged: print("location changed to", locationName)
    }

    GmwModel {
        id: gmwModel

        engine: gmwEngine
    }

    PositionSource {
        id: positionSource
        updateInterval: 1000
        active: true
    }



//    Dialog {
//        id: aboutDialog

//        title: Item {
//            height: 200
//            width: parent.width
//            Image {
//                id: icon
//                anchors.top: parent.top
//                anchors.horizontalCenter: parent.horizontalCenter
//                source: "/usr/share/icons/hicolor/80x80/apps/getmewheels280.png"
//            }
//            Text {
//                anchors.top: icon.bottom
//                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.topMargin: 20
//                color: "white"
//                font.pixelSize: 32
//                text: "GetMeWheels 1.2"
//            }

//        }

//        content:Item {
//            id: name
//            height: 200
//            width: parent.width
//            Label {
//                id: text
//                font.pixelSize: 22
//                anchors.centerIn: parent
//                color: "white"
//                text: "Copyright\n Michael Zanetti\n  michael_zanetti@gmx.net\n Christian Fetzer\n  fetzerch@googlemail.com"
//            }
//        }

//        buttons {
//            Button {
//                id: closeButton
//                text: "close"; onClicked: aboutDialog.close()
//            }
//            Button {
//                id: donateButton
//                anchors.top: closeButton.bottom
//                anchors.topMargin: 10
//                text: "donate";

//                onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=UPA7TFA73GKFY")
//            }
//            Button {
//                anchors.top: donateButton.bottom
//                anchors.topMargin: 10
//                text: "flattr";
//                onClicked: Qt.openUrlExternally("https://flattr.com/thing/465960/GetMeWheels")
//            }
//        }
//    }

}
