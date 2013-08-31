import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import GetMeWheels 1.0
import QtLocation 5.0

Page {
    id: mainPage
    property string name: "mapwindow"
    title: "Map"
//    property alias tracking: map.tracking

    function zoomToCurrentPosition() {
        map.zoomLevel = 16;
        map.tracking = true;
    }

    function routeTo(item) {
        map.routeTo(item);
    }

    tools: ToolbarActions {
        Action {
            iconSource: "/usr/share/icons/ubuntu-mobile/actions/scalable/settings.svg"
            text: "Settings"
            onTriggered: {
                PopupUtils.open(Qt.resolvedUrl("SettingsSheet.qml"), mainPage);
            }
        }
    }

//    ToolBarLayout {
//        id: commonTools
//        visible: true
//        ToolIcon {
//            platformIconId: "toolbar-view-menu"
//            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
//        }
//        ToolIcon {
//            platformIconId: "toolbar-refresh"
//            onClicked: {
//                gmwModel.clearVehicles();
//                gmwEngine.refreshVehicles(false);
//            }
//        }
//        ToolIcon {
//            id: mapListToggle
//            states: [
//                State {
//                    name: "map"; when: pageStack.depth === 1
//                    PropertyChanges { target: mapListToggle; iconId: "icon-m-toolbar-list"+ (theme.inverted ? "-white" : "") }
//                },
//                State {
//                    name: "list"; when: pageStack.depth === 2
//                    PropertyChanges { target: mapListToggle; iconSource: "images/icon-m-toolbar-map" + (theme.inverted ? "-inverted" : "") + ".png" }
//                }
//            ]
//            onClicked: {
//                myMenu.close();
//                if(state == "map") {
//                    var component = Qt.createComponent("ItemList.qml")
//                    if (component.status == Component.Ready) {
//                        pageStack.push(component, {model: gmwModel});
//                    } else {
//                        console.log("Error loading component:", component.errorString());
//                    }
//                } else {
//                    pageStack.pop();
//                }

//            }
//        }
//    }

//    Menu {
//        id: myMenu
//        visualParent: pageStack
//        MenuLayout {
//            MenuItem {
//                text: qsTr("Settings");
//                onClicked: {
//                    var component = Qt.createComponent("SettingsSheet.qml")
//                    if (component.status === Component.Ready) {
//                        var settingsSheet = component.createObject(mainPage)
//                        settingsSheet.open();
//                    } else {
//                        console.log("Error loading component:", component.errorString());
//                    }

//                }
//            }
//            MenuItem {
//                text: qsTr("Refresh all")
//                onClicked: {
//                    gmwModel.clearAll();
//                    gmwEngine.refreshStationary(false);
//                    gmwEngine.refreshVehicles(false);
//                }
//            }
//            MenuItem {
//                text: qsTr("About")
//                onClicked: aboutDialog.open();
//            }
//        }
//    }

    PositionSource {
        id: positionSource
        updateInterval: 1000
        active: true
        onPositionChanged: {
            var coord = positionSource.position.coordinate;
            gmwModel.currentPositionChanged(coord)
            console.log("Coordinate:", coord.longitude, coord.latitude);
        }
    }
    Timer {
        interval: 2000
        running: true
        repeat: true
        onTriggered: {
            var coord = positionSource.position.coordinate;
            console.log("Coordinate:", coord.longitude, coord.latitude);
        }
    }

    Map {
        id: map
        anchors.fill: parent

        // TODO: init with location from engine
        center {
            latitude: 48.468
            longitude: 9.94117
        }

        plugin : Plugin {
          name : "osm"
        }

        property var oldCenter
        property int oneGridUnitInMeters
        readonly property int calculationStep: 4 //gus

        Component.onCompleted: {
            oldCenter = QtLocation.coordinate(center.latitude, center.longitude);
            oneGridUnitInMeters = calculateGuMeters();
        }

        onZoomLevelChanged: {
            oneGridUnitInMeters = calculateGuMeters();
        }

        onCenterChanged: {
            if (oneGridUnitInMeters == 0) {
                oneGridUnitInMeters = calculateGuMeters();
            }

            if (map.center.distanceTo(oldCenter) > oneGridUnitInMeters * calculationStep) {
                print("moved by %1 gu".arg(calculationStep));
                map.oldCenter = QtLocation.coordinate(center.latitude, center.longitude);
            }
        }

        function calculateGuMeters() {
            var centerInPixels = map.toScreenPosition(center);
            print("centerInPxels", centerInPixels.x, centerInPixels.y)
            centerInPixels.x += units.gu(1);
            var offsetPoint = map.toCoordinate(centerInPixels);
            var distance = offsetPoint.distanceTo(center);
            print("distance is:", distance)
            return distance;
        }

        GmwProxyModel {
            id: mapModel
            model: gmwModel
            zoomLevel: map.zoomLevel
            thinningEnabled: true
        }

        function isOnScreen(gmwitem, mapCenter) {
            // TODO: adjust gmwitem.location calculationStep * units.gu(1) towards the map center to make items appear/disappear outside the visible area
            var screenCoords = map.toScreenPosition(gmwitem.location);
            return screenCoords.x > 0;
        }

        MapItemView {
            model: mapModel

            delegate: MapQuickItem {
                id: itemDelegate
                property bool onScreen: map.isOnScreen(gmwitem, map.oldCenter)
                sourceItem: Loader {
                    id: delegateLoader
                    sourceComponent: itemDelegate.onScreen ? delegateComponent : undefined
                }

                Component {
                    id: delegateComponent
                    Image {
                        id: itemImage
                        height: units.gu(4)
                        width: units.gu(4)
                        source: if (itemType == GmwItem.TypeVehicle) {
                                    if (gmwitem.engineType == GmwVehicle.EngineTypeED) {
                                        return "images/car_ed_map.png";
                                    } else {
                                        return "images/car_map.png";
                                    }
                                } else if (itemType == GmwItem.TypeGasStation){
                                    return "images/gas_map.png";
                                } else {
                                     if (gmwitem.chargingPole) {
                                         return "images/parking_cp_map.png"
                                     } else {
                                         return "images/parking_map.png"
                                     }
                                }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
//                                var component = Qt.createComponent("ItemDetailsSheet.qml")
//                                print(component.error)
                                PopupUtils.open(Qt.resolvedUrl("ItemDetailsSheet.qml"), itemDelegate, { gmwItem: gmwitem });
                                //PopupUtils.open(testSheet, itemImage)
                            }
                        }

                    }
                }
                coordinate: gmwitem.location
                z: 1

                anchorPoint.x: delegateLoader.width / 2
                anchorPoint.y: delegateLoader.height
            }
        }

        MapQuickItem {
            sourceItem: Image {
                width: units.gu(3)
                height: units.gu(3)
                source: "images/location_mark.png"

            }
            coordinate: positionSource.position.coordinate
            z: 3
            anchorPoint.x: width / 2
            anchorPoint.y: height / 2

        }

        MapCircle {
            radius: Math.max(positionSource.position.horizontalAccuracy, positionSource.position.verticalAccuracy)
            color: Qt.rgba(255, 0, 0, .1)
            center: positionSource.position.coordinate
            border.width: units.dp(1)
            border.color: Qt.rgba(255, 0, 0, 0.3)
            z: 0
        }

        zoomLevel: 14
//        tracking: true

//        onItemsClicked: {
//            print("yeeha: " + items);
//            detailsSheet.model = items;
//            detailsSheet.open();
//        }

//        onRoutingFailed: {
//            infoBanner.text = "Routing failed: Service Temporarily Unavailable";
//            infoBanner.show();
//        }

//        onGpsAvailableChanged: {
//            appWindow.gpsAvailable = gpsAvailable;
//        }

    }

//    ItemDetailsSheet {
//        id: detailsSheet

//        onGoTo: {
//            map.routeTo(item);
//        }
//    }

//    PinchArea {
//        id: pincharea

//        property double __oldZoom

//        anchors.fill: parent

//        function calcZoomDelta(zoom, percent) {
//            return Math.round(zoom + Math.log(percent)/Math.log(2))
//        }

//        onPinchStarted: {
//            print("pinch started")
//            __oldZoom = map.zoomLevel
//        }

//        onPinchUpdated: {
//            print("pinch updated")
//            var newZoomLevel = calcZoomDelta(__oldZoom, pinch.scale);
//            if(map.zoomLevel != newZoomLevel) {
//                map.zoomLevel = newZoomLevel;
//            }
//        }

//        onPinchFinished: {
//            print("pinch finished")
//            var newZoomLevel = calcZoomDelta(__oldZoom, pinch.scale);
//            if(map.zoomLevel != newZoomLevel) {
//                map.zoomLevel = newZoomLevel;
//            }
//        }
//    }

//    MouseArea {
//        id: mousearea

//        property bool __isPanning: false
//        property int __lastX: -1
//        property int __lastY: -1
//        property bool __panned: false

//        anchors.fill : parent

//        onPressed: {
//            map.tracking = false
//            __isPanning = true
//            __lastX = mouse.x
//            __lastY = mouse.y
//            __panned = false
//        }

//        onReleased: {
//            __isPanning = false
//            if(!__panned) {
//                map.clicked(mouseX, mouseY)
//            }
//        }

//        onPositionChanged: {
//            if (__isPanning) {
//                var dx = mouse.x - __lastX
//                var dy = mouse.y - __lastY
//                if(Math.abs(dx) > 5 || Math.abs(dy) > 5) {
//                    __panned = true
//                    map.pan(-dx, -dy)
//                    __lastX = mouse.x
//                    __lastY = mouse.y
//                }
//            }
//        }

//        onCanceled: {
//            __isPanning = false;
//        }
//    }

    Column {
        height: units.gu(30)
        anchors.right: parent.right
        anchors.rightMargin: units.gu(2)
        anchors.verticalCenter: parent.verticalCenter
        spacing: units.gu(1)

        ZoomSlider {
            height: parent.height - centerButton.height - parent.spacing * 3
            value: map.zoomLevel
            width: units.gu(3)
            maxValue: map.maximumZoomLevel
            minValue: map.minimumZoomLevel
            radiusFactor: 1
            onValueChanged: {
                map.zoomLevel = value;
            }
        }

        Image {
            id: centerButton
            anchors.horizontalCenter: parent.horizontalCenter
            source: "images/location_mark_big.png"
            width: units.gu(3)
            height: units.gu(3)
            MouseArea {
                anchors.fill: parent
                anchors.margins: -units.gu(1)
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
                opacity: (positionSource.position.latitudeValid && positionSource.position.longitudeValid) ? 0 : .5
                color: "black"
                anchors.centerIn: parent
                height: parent.height
                width: parent.width
                radius: parent.width / 2
            }
        }
    }

//    InfoBanner {
//        id: infoBanner
//        timerShowTime: 5000
//    }


    Component {
        id: testSheet
        Popover {

        }
    }

}
