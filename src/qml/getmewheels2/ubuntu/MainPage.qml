import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import GetMeWheels 1.0
import QtLocation 5.0

Page {
    id: mainPage
    title: "Map"

    property string name: "mapwindow"
    property alias positionCoordinate: positionMarker.coordinate
    property alias tracking: map.tracking

    function zoomToCurrentPosition() {
        map.zoomLevel = 16;
        map.tracking = true;
    }

    function routeTo(item) {
        map.routeTo(item);
    }

    tools: mainToolbar

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

    Map {
        id: map
        anchors.fill: parent

        property bool tracking: false

        onTrackingChanged: {
            if (tracking && positionSource.valid) {
                map.center = positionCoordinate
            }
        }

        center: gmwEngine.location.area.center

        Connections {
            target: gmwEngine
            onLocationChanged: {
                map.zoomLevel = 13
            }
        }

        Connections {
            target: map.gesture
            onPanFinished: {
                map.tracking = false

                // This is very unreliable for now as it doesn't get emitted when slowly dragging the map, only when flicking
                mapModel.visibleRect = QtLocation.rectangle(map.toCoordinate(Qt.point(0, 0)), map.toCoordinate(Qt.point(map.width, map.height)))
            }
        }

        // So lets use this as a workaround for now...
        onCenterChanged: {
            filterTimer.restart();
        }
        Timer {
            id: filterTimer
            repeat: false
            interval: 200

            onTriggered: {
                mapModel.visibleRect = QtLocation.rectangle(map.toCoordinate(Qt.point(0, 0)), map.toCoordinate(Qt.point(map.width, map.height)))
            }
        }

        plugin : Plugin {
            name : "osm"
        }

        GmwProxyModel {
            id: mapModel
            model: gmwModel
            thinningEnabled: true
            onlyBooked: showOnlyBookedCars
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
                            right: parent.right
                            margins: -units.gu(.5)
                        }

                        color: itemType == GmwItem.TypeVehicle ? (gmwitem.booking.exired ? "red" : (gmwitem.booking.timeLeft > 900 ? "#9bd500" : "orange")) : "transparent"
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
            id: positionMarker
            sourceItem: Image {
                width: units.gu(3)
                height: units.gu(3)
                source: "images/location_mark.png"
            }
            coordinate: positionSource.position.coordinate
            z: 3
            anchorPoint.x: width / 2
            anchorPoint.y: height / 2

            onCoordinateChanged: {
                if (map.tracking) {
                    map.center = coordinate;
                }
            }

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
            visible: positionSource.valid
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
}
