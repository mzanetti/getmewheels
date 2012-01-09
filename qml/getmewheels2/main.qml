import QtQuick 1.1
import com.nokia.meego 1.0
import GetMeWheels 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage

//    Component.onCompleted: {
//        theme.inverted = true;
//    }

    Settings {
        id: gmwSettings
    }

    Car2goEngine {
        id: gmwEngine
        location: gmwSettings.location
    }

    GmwModel {
        id: gmwModel

        engine: gmwEngine
    }

    MainPage {
        id: mainPage
        orientationLock: PageOrientation.LockPortrait
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
        ToolIcon {
            platformIconId: "toolbar-refresh"
            anchors.horizontalCenter: (parent === undefined) ? undefined : parent.horizontalCenter
            onClicked: {
                gmwModel.clearVehicles();
                gmwEngine.refreshVehicles(false);
            }
        }
        ToolIcon {
            iconSource: "/usr/share/maps/images/icon_aroundme.png"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: mainPage.zoomToCurrentPosition();
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Settings"); onClicked: settingsSheet.open() }
            MenuItem {
                text: qsTr("Refresh all")
                onClicked: {
                    gmwModel.clearAll();
                    gmwEngine.refreshStationary(false);
                    gmwEngine.refreshVehicles(false);
                }
            }
            MenuItem {
                text: qsTr("About")
                onClicked: aboutDialog.open();
            }
        }
    }

    SettingsSheet {
        id: settingsSheet

        onAccepted: {
            mainPage.tracking= false;
            gmwModel.clearAll();
            gmwEngine.refreshStationary(false);
            gmwEngine.refreshVehicles(false);
        }
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
                text: "GetMeWheels 1.0"
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
                text: "Copyright\n Michael Zanetti\n michael_zanetti@gmx.net\n\nChristian Fetzer\nfetzerch@googlemail.com"
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
        }
    }

}
