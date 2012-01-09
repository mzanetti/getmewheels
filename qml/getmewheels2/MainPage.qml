import QtQuick 1.1
import com.nokia.meego 1.0
import GetMeWheels 1.0
import QtMobility.location 1.1

Page {
    tools: commonTools
    property alias tracking: map.tracking

    function zoomToCurrentPosition() {
        map.zoomLevel = 16;
        map.tracking = true;
    }

    GmwMap {
        id: map
        anchors.fill: parent
        //zoomLevel: 1 //14
        model: gmwModel
        tracking: true

        onItemClicked: {
            print("yeeha: " + item);
            detailsSheet.gmwItem = item;
            detailsSheet.open();
        }
    }

    ItemDetailsSheet {
        id: detailsSheet

        onAccepted: {
            //            map.animatedPanTo(detailsSheet.gmwItem);
            map.routeTo(detailsSheet.gmwItem);
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
                if(dx > 0 || dy > 0) {
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

    Rectangle {
        id: sliderBackground
        height: 380
        width: 60
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        color: "grey"
        opacity: .5
        radius: 15
        border.color: "black"
        border.width: 2
    }

    Slider {
        anchors.centerIn: sliderBackground
        height: sliderBackground.height - 10
        orientation: Qt.Vertical
        value: map.zoomLevel
        minimumValue: map.minimumZoomLevel
        maximumValue: map.maximumZoomLevel
        stepSize: 1
        onValueChanged: {
            map.zoomLevel = value;
        }
    }

}
