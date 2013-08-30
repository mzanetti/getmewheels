import QtQuick 2.0
import Ubuntu.Components 0.1
import GetMeWheels 1.0
import Ubuntu.Components.Popups 0.1


Page {
    id: itemList
    anchors.fill: parent

    property alias model: proxyModel.model
    property alias onlyBooked: proxyModel.onlyBooked

    GmwProxyModel {
        id: proxyModel
    }


    ListView {
        id: listView
        anchors.fill: parent
        model: proxyModel
        clip: true

        delegate:   Item {
            id: listItem
            height: 88
            width: parent.width

            MouseArea {
                id: mouseArea
                anchors.fill: listItem

                function dialogAccepted() {

                    var mapPage = pageStack.find(function(page) {
                        return page.name === "mapwindow";
                    });
                    mapPage.routeTo(gmwitem);
                    pageStack.pop();
                }

                onClicked: {
                    var component = Qt.createComponent("ItemDetailsSheet.qml")
                    PopupUtils.open(component, itemList, { model: [gmwitem] });
                }
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
                        color: "#848684"
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
    }
}
