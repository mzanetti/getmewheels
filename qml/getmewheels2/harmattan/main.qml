import QtQuick 1.1
import com.nokia.meego 1.0
import GetMeWheels 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage

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

    MainPage {
        id: mainPage
        orientationLock: PageOrientation.LockPortrait
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
//            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
        ToolIcon {
            platformIconId: "toolbar-refresh"
//            anchors.horizontalCenter: (parent === undefined) ? undefined : parent.horizontalCenter
            onClicked: {
                gmwModel.clearVehicles();
                gmwEngine.refreshVehicles(false);
            }
        }
//        ToolIcon {
//            iconSource: "/usr/share/maps/images/icon_aroundme.png"
////            anchors.right: (parent === undefined) ? undefined : parent.right
//            onClicked: mainPage.zoomToCurrentPosition();
//        }
        ToolIcon {
            id: mapListToggle
//            anchors.right: (parent === undefined) ? undefined : parent.right
            states: [
                State {
                    name: "map"; when: pageStack.depth === 1
                    PropertyChanges { target: mapListToggle; iconId: "icon-m-toolbar-list" }
                },
                State {
                    name: "list"; when: pageStack.depth === 2
                    PropertyChanges { target: mapListToggle; iconSource: "images/icon-m-toolbar-map.png" }
                }
            ]
            onClicked: {
                myMenu.close();
                if(state == "map") {
                    var component = Qt.createComponent("ItemList.qml")
                    if (component.status == Component.Ready) {
                        pageStack.push(component, {model: gmwModel});
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }
                } else {
                    pageStack.pop();
                }

            }
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
            gmwEngine.locationName = locationName;
            gmwEngine.defaultAccountName = accountName;
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
                text: "GetMeWheels 1.1"
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
