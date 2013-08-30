import QtQuick 1.1
import com.nokia.symbian 1.1
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
        pageStack.pop();
    }

    GmwMap {
        id: map
        anchors.fill: parent
        //zoomLevel: 1 //14
        model: gmwModel
        tracking: true
//        visible: pageStack.count() === 1

        onItemsClicked: {
            rumbleEffect.start();
            print("yeeha: " + items);
            detailsSheet.model = items;
            pageStack.push(detailsSheet)
        }

        onRoutingFailed: {
            infoBanner.text = qsTr("Routing failed: Service Temporarily Unavailable");
            infoBanner.open();
        }

        onGpsAvailableChanged: {
            appWindow.gpsAvailable = gpsAvailable;
        }
    }

    ItemDetailsSheet {
        id: detailsSheet

        onGoTo: {
            console.log("routing to " + item);
            map.routeTo(item);
            pageStack.pop();
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

//    Rectangle {
//        id: sliderBackground
//        height: 380
//        width: 60
//        anchors.right: parent.right
//        anchors.rightMargin: 20
//        anchors.verticalCenter: parent.verticalCenter
//        color: "grey"
//        opacity: .5
//        radius: 15
//        border.color: "black"
//        border.width: 2
//    }

    Column {
        height: 380
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10

        ZoomSlider {
            height: parent.height - centerButton.height - parent.spacing * 3
            value: map.zoomLevel
            width: 36
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
            source: "images/location_mark.png"
            width: 36
            height: 36
            MouseArea {
                enabled: map.gpsAvailable
                anchors.fill: parent
                anchors.margins: -10
                onClicked: {
                    rumbleEffect.start();
                    mainPage.zoomToCurrentPosition();
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
//        timerShowTime: 5000
    }


}
