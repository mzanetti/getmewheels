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
        updateInterval: 10000
        active: true
        property bool gotPosition: false
        onPositionChanged: {
            gotPosition = true
            print("Bug tvoss as long as this show up more often than every 10 seconds.")
        }
    }

    // Workaround for positionSource updateInterval not working yet
    Timer {
        interval: 10000
        running: positionSource.active && positionSource.gotPosition
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            var coord = positionSource.position.coordinate;
            gmwModel.currentPositionChanged(coord)
            console.log("updating position:", coord.longitude, coord.latitude);
            if (map.tracking) {
                map.center = coord;
            }
        }
    }

    Map {
        id: map
        anchors.fill: parent

        property bool tracking: false

        // TODO: init with location from engine
        center {
            latitude: 48.468
            longitude: 9.94117
        }

        plugin : Plugin {
            name : "osm"
        }

        GmwProxyModel {
            id: mapModel
            model: gmwModel
            thinningEnabled: true
            //onlyBooked: true

        }
        Binding {
            target: mapModel
            property: "zoomLevel"
            value: map.zoomLevel
            when: !zoomSlider.pressed && !map.gesture.isPinchActive
        }

        MapItemView {
            model: mapModel

            delegate: MapQuickItem {
                id: itemDelegate
                coordinate: gmwitem.location
                anchorPoint.x: itemDelegate.width / 2
                anchorPoint.y: itemDelegate.height / 2
                z: 1

                sourceItem: UbuntuShape {
                    width: (itemType == GmwItem.TypeVehicle && gmwitem.booking.valid) ? units.gu(5) : units.gu(3.5)
                    height: width
                    radius: "medium"
                    image: Image {
                        anchors.fill: parent
                        source: itemType == GmwItem.TypeVehicle ?
                               (gmwitem.engineType == GmwVehicle.EngineTypeED ? "images/car_green.svg" : "images/car_blue.svg")
                             : (itemType == GmwItem.TypeParkingSpot ?
                                    (gmwitem.chargingPole ? "images/parking_green.svg" : "images/parking_blue.svg")
                                  : "images/gas.svg")
                    }
                    UbuntuShape {
                        color: "black"
                        width: Math.max(units.gu(1.5), hideCountLabel.implicitWidth + units.gu(.5))
                        height: units.gu(1.5)
                        visible: model.hideCount > 0
                        anchors {
                            top: parent.top
                            right: parent.right
                            margins: -units.gu(.5)
                        }
                        Label {
                            id: hideCountLabel
                            anchors.centerIn: parent
                            fontSize: "x-small"
                            text: model.hideCount + 1
                            color: "white"
                        }
                    }
                    UbuntuShape {
                        width: units.gu(1.5)
                        height: units.gu(1.5)
                        visible: itemType == GmwItem.TypeVehicle && gmwitem.booking.valid
                        anchors {
                            top: parent.top
                            left: parent.left
                            margins: -units.gu(.5)
                        }

                        color: itemType == GmwItem.TypeVehicle ? (gmwitem.booking.exired ? "red" : (gmwitem.booking.timeLeft > 10 ? "#9bd500" : "yellow")) : "transparent"
                    }
                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            PopupUtils.open(Qt.resolvedUrl("ItemDetailsSheet.qml"), itemDelegate, { gmwItem: gmwitem });
                        }
                    }
                }
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

    Row {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: units.gu(2)
        }
        spacing: units.gu(2)
        UbuntuShape {
            height: zoomSlider.height
            width: height
            color: "#44000000"
            Image {
                height: parent.height - units.gu(1)
                width: height
                anchors.centerIn: parent
                source: "images/location_mark.svg"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(positionSource.valid) {
                        mainPage.zoomToCurrentPosition();
                    } else {
                        print("Swallowing Mouse click");
                    }
                }
            }
        }

        ZoomSlider {
            id: zoomSlider
            width: parent.width - x
            maxValue: map.maximumZoomLevel
            minValue: map.minimumZoomLevel
            value: map.zoomLevel
            radiusFactor: 1
            onValueChanged: {
                map.zoomLevel = value;
            }
        }
    }

    //    InfoBanner {
    //        id: infoBanner
    //        timerShowTime: 5000
    //    }


}
