import QtQuick 1.1
import com.nokia.symbian 1.1
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
        ToolButton {
            iconSource: "toolbar-view-menu"
//            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
        ToolButton {
            iconSource: "toolbar-refresh"
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
        ToolButton {
            id: mapListToggle
//            anchors.right: (parent === undefined) ? undefined : parent.right
            states: [
                State {
                    name: "map"; when: pageStack.depth === 1
                    PropertyChanges { target: mapListToggle; iconSource: "toolbar-list" }
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
            MenuItem { text: qsTr("Settings"); onClicked: pageStack.push(settingsSheet) }
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
            MenuItem {
                text: qsTr("Exit")
                onClicked: Qt.quit();
            }
        }
    }

    SettingsSheet {
        id: settingsSheet

        onAccepted: {
            pageStack.pop();
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
            height: 350
            width: parent.width
            Label {
                id: text
                font.pixelSize: 22
                anchors.centerIn: parent
                width: parent.width - 20
                color: "white"
                text: "Copyright\n Michael Zanetti\n  michael_zanetti@gmx.net\n Christian Fetzer\n  fetzerch@googlemail.com\n\nThis product uses the car2go API but is not endorsed or certified by car2go.\nThe use of this application requires a data connection which may be associated with additional costs."
                wrapMode: Text.WordWrap
            }
        }

        buttons {
            Button {
                id: closeButton
                text: "close"; onClicked: aboutDialog.close()
                width: parent.width*2/3
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }
    }

}
