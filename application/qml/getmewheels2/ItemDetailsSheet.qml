import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import GetMeWheels 1.0

Sheet {
    id: settingsSheet
    acceptButtonText: "Go to"
    rejectButtonText: "Close"

    signal goTo(variant item)

    onAccepted: goTo(listView.currentItem.gmwItem);

    property alias model: listView.model
    property alias currentItem: listView.currentItem

    content: Item {
        anchors.fill: parent
        PageIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
//            width: parent.width
            height: 20
            visible: totalPages > 1

            id: indicator
            objectName: "pageIndicatorObject"
        }

        ListView {
            id: listView
            anchors { left: parent.left; top: parent.top; right: parent.right; bottom: indicator.top }
            orientation: ListView.Horizontal
            highlightRangeMode: ListView.StrictlyEnforceRange
            preferredHighlightBegin: 0; preferredHighlightEnd: 0
            highlightFollowsCurrentItem: true
            snapMode: ListView.SnapOneItem
            cacheBuffer: width * 2
            interactive: count > 1

            onModelChanged: indicator.totalPages = count;
            onCurrentIndexChanged: indicator.currentPage = currentIndex + 1;

            delegate: Item {
                width: listView.width
                height: listView.height
                property QtObject gmwItem: model.modelData
                Column {
                    id: contentColumn
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 20

                    Row {
                        width: parent.width
                        height: nameLabel.height
                        spacing: 10
                        Image {
                            id: iconImage
                            source: itemTypeToImageSource(gmwItem.itemType, gmwItem.engineType, gmwItem.chargingPole)
                            anchors.verticalCenter: parent.verticalCenter

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
                            width: parent.width - iconImage.width - parent.spacing
                            anchors.verticalCenter: parent.verticalCenter

                            Label {
                                id: nameLabel
                                width: parent.width
                                text: gmwItem.name
                                font.pixelSize: 40
                                wrapMode: Text.WordWrap
                            }
                            Label {
                                text: gmwItem.engineType === GmwVehicle.EngineTypeED ? "Electric Drive" : gmwItem.chargingPole ? "Electric Drive Charging Pole" : ""
                                visible: text.length > 0
                            }
                        }
                    }

                    SectionHeader {
                        width: parent.width
                        headerText: "Location"
                    }

                    Label {
                        text: gmwItem.address
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                    Row {
                        spacing: 10
                        Label {
                            text: "Distance:"
                        }

                        Label {
                            text: gmwItem.distance === -1 ? "Waiting for GPS..." : gmwItem.distance
                        }
                    }

                    SectionHeader {
                        width: parent.width
                        headerText: "State"
                        visible: gmwItem.itemType === GmwItem.TypeVehicle
                    }

                    Grid {
                        id: vehicleGrid
                        spacing: 10
                        visible: gmwItem.itemType === GmwItem.TypeVehicle
                        columns: 2

                        Label {
                            height: fuelRow.height
                            text: gmwItem.engineType === GmwVehicle.EngineTypeED ? "Battery level:" : "Fuel level:"
                            verticalAlignment: Text.AlignVCenter
                        }

                        Row {
                            id: fuelRow
                            spacing: 10
                            Image {
                                property string typeString: gmwItem.engineType === GmwVehicle.EngineTypeED ? "battery" : "fuel"
                                source: gmwItem.fuelLevel > 25 ? "images/" + typeString + "_green.png" : gmwItem.fuelLevel > 15 ? "images/" + typeString + "_orange.png" : "images/" + typeString + "_red.png"
                            }

                            Label {
                                height: parent.height
                                text: gmwItem.fuelLevel + " %"
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        function stateToIconSource(state) {
                            switch(state) {
                            case GmwVehicle.StateUnacceptable:
                                return "image://theme/icon-m-messaging-smiley-cry";
                            case GmwVehicle.StateIssuesExist:
                                return "image://theme/icon-m-messaging-smiley-sad";
                            case GmwVehicle.StateSatisfied:
                                return "image://theme/icon-m-messaging-smiley-happy";
                            case GmwVehicle.StateGood:
                                return "image://theme/icon-m-messaging-smiley-happy";
                            case GmwVehicle.StateExcellent:
                                return "image://theme/icon-m-messaging-smiley-very-happy";
                            }
                            return "image://theme/icon-m-invitation-pending";
                        }

                        Label {
                            text: "Interior State:"
                            height: stateImage.height
                            verticalAlignment: Text.AlignVCenter
                        }

                        Image {
                            id: stateImage
                            source: vehicleGrid.stateToIconSource(gmwItem.interiorState)
                        }
                        Label {
                            text: "Exterior State:"
                            height: stateImage.height
                            verticalAlignment: Text.AlignVCenter
                        }

                        Image {
                            source: vehicleGrid.stateToIconSource(gmwItem.exteriorState)
                        }
                    }

                    SectionHeader {
                        width: parent.width
                        headerText: "Availability"
                        visible: gmwItem.itemType == GmwItem.TypeParkingSpot || gmwItem.itemType == GmwItem.TypeVehicle
                    }


                    Row {
                        width: parent.width
                        visible: gmwItem.itemType === GmwItem.TypeVehicle
                        spacing: 10
                        Image {
                            id: availabilityIcon
                            visible: gmwItem.booking.valid
                            width: 48
                            height: 48
                            property int timeLeft: gmwItem.booking.timeLeft
                            source: timeLeft > 900 ? "image://theme/icon-m-common-presence-online" : (timeLeft > 0 ?"image://theme/icon-m-common-presence-away" : "image://theme/icon-m-common-presence-busy")
                        }
                        Label {
                            width: parent.width - availabilityIcon.width - 10
                            anchors.verticalCenter: parent.verticalCenter
                            text: gmwItem.booking.text
                            wrapMode: Text.WordWrap
                        }
                    }
                    Button {
                        width: parent.width
                        visible: gmwItem.itemType == GmwItem.TypeVehicle
                        text: gmwItem.booking.valid && !gmwItem.booking.expired ? "Cancel" : "Book"
                        enabled: gmwEngine.defaultAccountName.length > 0;
                        onClicked: {
                            if(gmwItem.booking.valid && !gmwItem.booking.expired) {
                                createBookingDialog.state = "cancel"
                                createBookingDialog.gmwItem = gmwItem;
                                createBookingDialog.open();
                            } else {
                                createBookingDialog.state = "book"
                                createBookingDialog.gmwItem = gmwItem;
                                createBookingDialog.open();
                            }
                        }
                    }

                    Row {
                        spacing: 10
                        visible: gmwItem.itemType === GmwItem.TypeParkingSpot
                        Image {
                            id: parkingFreeIcon
                            source: gmwItem.capacityTotal - gmwItem.capacityUsed === 0 ? "image://theme/icon-m-common-presence-busy" : "image://theme/icon-m-common-presence-online"
                        }

                        Label {
                            text: (gmwItem.capacityTotal - gmwItem.capacityUsed) + " free / " + gmwItem.capacityTotal + " total"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

            }
        }
    }

    Dialog {
        id: createBookingDialog
        width: parent.width
        property QtObject gmwItem

        states: [
            State {
                name: "book"
                PropertyChanges { target: createBookingHeaderLabel; text: "Create booking?" }
                PropertyChanges { target: createBookingTextLabel; text: "The booking will be valid for 30 minutes from now. Cancelling or missing a booked car is associated with additional costs."}
            },
            State {
                name: "cancel"
                PropertyChanges { target: createBookingHeaderLabel; text: "Cancel booking?" }
                PropertyChanges { target: createBookingTextLabel; text: "Cancelling or missing a booking is associated with additional costs."}
            }
        ]
        title: Column {
            width: parent.width
            Label {
                id: createBookingHeaderLabel
                width: parent.width
                font.pixelSize: 40
                color: "white"
            }
        }

        content: Column {
            width: parent.width
            Label {
                id: createBookingTextLabel
                width: parent.width
                wrapMode: Text.WordWrap
                color: "white"
            }
            Item {
                width: parent.width
                height: 50
            }
        }

        buttons {
            ButtonRow {
                width: parent.width
                Button {
                    id: yesButton
                    text: "yes";
                    onClicked: {
                        if(createBookingDialog.state == "book") {
                            if(gmwEngine.createBooking(createBookingDialog.gmwItem)) {
                                infoBanner.text = "Car booked successfully";
                                infoBanner.show();
                            } else {
                                infoBanner.text = "Failed to create booking: " + gmwEngine.error();
                                infoBanner.show();
                            }
                        } else if(createBookingDialog.state == "cancel") {
                            if(gmwEngine.cancelBooking(createBookingDialog.gmwItem)) {
                                infoBanner.text = "Booking cancelled successfully";
                                infoBanner.show();
                            } else {
                                infoBanner.text = "Failed to cancel booking: " + gmwEngine.error();
                                infoBanner.show();
                            }

                        }
                        createBookingDialog.close();
                    }
                }
                Button {
                    id: noButton
                    text: "no";
                    onClicked: createBookingDialog.close();
                }
            }
        }
    }

    InfoBanner {
        id: infoBanner
        timerShowTime: 5000
    }
}
