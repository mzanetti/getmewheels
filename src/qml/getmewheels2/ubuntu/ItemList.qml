import QtQuick 2.0
import Ubuntu.Components 0.1
import GetMeWheels 1.0
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1


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

        delegate: Subtitled {
            id: listItem
            text: itemAddress
            subText: itemDistanceString + " - " + itemName

            icon: itemTypeToImageSource(itemType, itemEngineType, itemParkingCP);

            function itemTypeToImageSource(itemType, engineType, parkingCP) {
                switch(itemType) {
                case GmwItem.TypeGasStation:
                    return Qt.resolvedUrl("images/gas.svg");
                case GmwItem.TypeVehicle:
                    return engineType === GmwVehicle.EngineTypeED ? Qt.resolvedUrl("images/car_green.svg") : Qt.resolvedUrl("images/car_blue.svg");
                case GmwItem.TypeParkingSpot:
                    return parkingCP ? Qt.resolvedUrl("images/parking_green.svg") : Qt.resolvedUrl("images/parking_blue.svg")
                }
            }

            onClicked: {
                var component = Qt.createComponent("ItemDetailsSheet.qml")
                PopupUtils.open(component, itemList, { model: [gmwitem] });
            }

            function dialogAccepted() {

                var mapPage = pageStack.find(function(page) {
                    return page.name === "mapwindow";
                });
                mapPage.routeTo(gmwitem);
                pageStack.pop();
            }
        }
    }
}
