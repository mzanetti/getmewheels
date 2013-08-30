import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1
import GetMeWheels 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage
    property bool gpsAvailable: false

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

    HapticsEffect {
        id: rumbleEffect
        attackIntensity: 0.0
        attackTime: 250
        intensity: 1.0
        duration: 75
        fadeTime: 250
        fadeIntensity: 0.0
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
            height: 350
            width: parent.width
            Label {
                id: text
                font.pixelSize: 22
                anchors.centerIn: parent
                width: parent.width - 20
                color: "white"
                text: qsTr("Copyright") + "\n Michael Zanetti\n  michael_zanetti@gmx.net\n Christian Fetzer\n  fetzerch@googlemail.com\n\n" + qsTr("This product uses the car2go API but is not endorsed or certified by car2go.") + "\n" + qsTr("The use of this application requires a data connection which may be associated with additional costs.")
                wrapMode: Text.WordWrap
            }
        }

        buttons {
            Button {
                id: closeButton
                text: qsTr("Close"); onClicked: aboutDialog.close()
                width: parent.width*2/3
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }
    }

}
