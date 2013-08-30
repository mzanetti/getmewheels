import QtQuick 1.1
import com.nokia.meego 1.0
import GetMeWheels 1.0


Page {
    id: itemList
    orientationLock: PageOrientation.LockPortrait

    property alias model: proxyModel.model

    tools: commonTools

    GmwProxyModel {
        id: proxyModel
    }

    Item {
        id: header
        width: parent.width
        height: 72

        ButtonRow {
            anchors.fill: parent

            platformStyle: TabButtonStyle { }
            TabButton {
                text: "All"
                onClicked: proxyModel.onlyBooked = false;
            }
            TabButton {
                text: "Booked"
                onClicked: proxyModel.onlyBooked = true;
            }
        }

    }

    ListView {
        id: listView
        anchors {left: parent.left; top: header.bottom; right: parent.right; bottom: parent.bottom }
        model: proxyModel
        clip: true

        delegate:   Item {
            id: listItem
            height: 88
            width: parent.width

            MouseArea {
                id: mouseArea
                anchors.fill: background

                function dialogAccepted() {

                    var mapPage = pageStack.find(function(page) {
                        return page.name === "mapwindow";
                    });
                    mapPage.routeTo(gmwitem);
                    pageStack.pop();
                }

                onClicked: {
                    var component = Qt.createComponent("ItemDetailsSheet.qml")
                    if (component.status === Component.Ready) {
                        var itemDetailsSheet = component.createObject(listView)
                        itemDetailsSheet.model = [gmwitem];
                        itemDetailsSheet.goTo.connect(dialogAccepted)
                        itemDetailsSheet.open();
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }
                }
            }

            BorderImage {
                id: background
                anchors.fill: parent
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }

            Row {
                id: itemRow
                anchors {left: parent.left; top: parent.top; right: parent.right }
                height: 88
                anchors.margins: 5
                spacing: 10

                Image {
                    id: image
                    source: itemTypeToImageSource(itemType, itemEngineType, itemParkingCP);
                    height: 64
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit

                    function itemTypeToImageSource(itemType, engineType, parkingCP) {
                        switch(itemType) {
                        case GmwItem.TypeGasStation:
                            return "images/gas.png";
                        case GmwItem.TypeVehicle:
                            return engineType === GmwVehicle.EngineTypeED ? "images/car_green.png" : "images/car_blue.png";
                        case GmwItem.TypeParkingSpot:
                            return parkingCP ? "images/parking_cp.png" : "images/parking.png"
                        }
                    }
                }

                Column {
                    width: itemRow.width - image.width - parent.spacing
                    anchors.verticalCenter: parent.verticalCenter
                    Label {
                        id: mainText
                        width: parent.width
                        text: itemAddress
                        font.weight: Font.Bold
                        font.pixelSize: 26
                        elide: Text.ElideRight
                    }
                    Label {
                        width: parent.width
                        text: itemDistanceString + " - " + itemName
                        color: theme.inverted ? "#7b797b" : "#848684"
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                    }
                }
            }

//            Image {
//                id: arrow
//                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
//                anchors.right: parent.right;
//                anchors.verticalCenter: parent.verticalCenter
//                visible: folderModel.isFolder(index)
//            }


        }
        ScrollDecorator {
            flickableItem: listView
        }

    }
}
