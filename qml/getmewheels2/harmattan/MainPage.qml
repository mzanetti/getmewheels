import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import GetMeWheels 1.0
import QtMobility.location 1.1

Page {
    id: mainPage
    tools: commonTools
    property string name: "mapwindow"
    property alias tracking: map.tracking

    function zoomToCurrentPosition() {
        map.zoomLevel = 16;
        map.tracking = true;
    }

    function routeTo(item) {
        map.routeTo(item);
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
        ToolIcon {
            platformIconId: "toolbar-refresh"
            onClicked: {
                gmwModel.clearVehicles();
                gmwEngine.refreshVehicles(false);
            }
        }
        ToolIcon {
            id: mapListToggle
            states: [
                State {
                    name: "map"; when: pageStack.depth === 1
                    PropertyChanges { target: mapListToggle; iconId: "icon-m-toolbar-list"+ (theme.inverted ? "-white" : "") }
                },
                State {
                    name: "list"; when: pageStack.depth === 2
                    PropertyChanges { target: mapListToggle; iconSource: "images/icon-m-toolbar-map" + (theme.inverted ? "-inverted" : "") + ".png" }
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
            MenuItem {
                text: qsTr("Settings");
                onClicked: {
                    var component = Qt.createComponent("SettingsSheet.qml")
                    if (component.status === Component.Ready) {
                        var settingsSheet = component.createObject(mainPage)
                        settingsSheet.open();
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }

                }
            }
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

    GmwMap {
        id: map
        anchors.fill: parent
        //zoomLevel: 1 //14
        model: gmwModel
        tracking: true
        visible: true // pageStack.count() === 1

        onItemsClicked: {
            print("yeeha: " + items);
            detailsSheet.model = items;
            detailsSheet.open();
        }

        onRoutingFailed: {
            infoBanner.text = "Routing failed: Service Temporarily Unavailable";
            infoBanner.show();
        }

        onGpsAvailableChanged: {
            appWindow.gpsAvailable = gpsAvailable;
        }

    }

    ItemDetailsSheet {
        id: detailsSheet

        onGoTo: {
            map.routeTo(item);
        }
    }

    PinchArea {
        id: pincharea

        property double __oldZoom

        anchors.fill: parent

        function calcZoomDelta(zoom, percent) {
            return Math.round(zoom + Math.log(percent)/Math.log(2))
        }

        onPinchStarted: {
            print("pinch started")
            __oldZoom = map.zoomLevel
        }

        onPinchUpdated: {
            print("pinch updated")
            var newZoomLevel = calcZoomDelta(__oldZoom, pinch.scale);
            if(map.zoomLevel != newZoomLevel) {
                map.zoomLevel = newZoomLevel;
            }
        }

        onPinchFinished: {
            print("pinch finished")
            var newZoomLevel = calcZoomDelta(__oldZoom, pinch.scale);
            if(map.zoomLevel != newZoomLevel) {
                map.zoomLevel = newZoomLevel;
            }
        }
    }

    MouseArea {
        id: mousearea

        property bool __isPanning: false
        property int __lastX: -1
        property int __lastY: -1
        property bool __panned: false

        anchors.fill : parent

        onPressed: {
            map.tracking = false
            __isPanning = true
            __lastX = mouse.x
            __lastY = mouse.y
            __panned = false
        }

        onReleased: {
            __isPanning = false
            if(!__panned) {
                map.clicked(mouseX, mouseY)
            }
        }

        onPositionChanged: {
            if (__isPanning) {
                var dx = mouse.x - __lastX
                var dy = mouse.y - __lastY
                if(Math.abs(dx) > 5 || Math.abs(dy) > 5) {
                    __panned = true
                    map.pan(-dx, -dy)
                    __lastX = mouse.x
                    __lastY = mouse.y
                }
            }
        }

        onCanceled: {
            __isPanning = false;
        }
    }

    Column {
        height: 380
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10

        ZoomSlider {
            height: parent.height - centerButton.height - parent.spacing * 3
            value: map.zoomLevel
            width: 42
            maxValue: map.maximumZoomLevel
            minValue: 10
            radiusFactor: 1
            onValueChanged: {
                map.zoomLevel = value;
            }
        }

        Image {
            id: centerButton
            anchors.horizontalCenter: parent.horizontalCenter
            source: "images/location_mark_big.png"
            width: 42
            height: 42
            MouseArea {
                anchors.fill: parent
                anchors.margins: -10
                onClicked: {
                    if(map.gpsAvailable) {
//                        rumbleEffect.start();
                        mainPage.zoomToCurrentPosition();
                    } else {
                        print("Swallowing Mouse click");
                    }
                }
            }
            Rectangle {
                opacity: map.gpsAvailable ? 0 : .5
                color: "black"
                anchors.centerIn: parent
                height: parent.height
                width: parent.width
                radius: parent.width / 2
            }
        }
    }

    InfoBanner {
        id: infoBanner
        timerShowTime: 5000
    }


}
