import QtQuick 2.0
import Ubuntu.Components 0.1
import GetMeWheels 1.0
import Ubuntu.Components.Popups 0.1

Popover {
    id: itemDetailsSheet

    signal goTo(variant item)

    property variant gmwItem

    Connections {
        target: gmwEngine
        onLocationNameChanged: {
            reject();
        }
    }

    Item {
        anchors {
            left: parent.left
            right: parent.right
            margins: units.gu(1)
        }
        height: contentColumn.height + units.gu(2)

        Column {
            id: contentColumn
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            spacing: units.gu(1)

            Row {
                width: parent.width
                height: nameLabel.height
                spacing: units.gu(1)
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
                        text: gmwItem.engineType === GmwVehicle.EngineTypeED ? qsTr("Electric Drive") : gmwItem.chargingPole ? qsTr("Electric Drive Charging Pole") : ""
                        visible: text.length > 0
                    }
                }
            }

            SectionHeader {
                width: parent.width
                headerText: qsTr("Location")
            }

            Label {
                text: gmwItem.address
                width: parent.width
                wrapMode: Text.WordWrap
            }
            Row {
                spacing: 10
                Label {
                    text: qsTr("Distance:")
                }

                Label {
                    text: gmwItem.distance === -1 ? qsTr("Waiting for GPS...") : gmwItem.distance
                }
            }

            SectionHeader {
                width: parent.width
                headerText: qsTr("State")
                visible: gmwItem.itemType === GmwItem.TypeVehicle
            }

            Grid {
                id: vehicleGrid
                spacing: 10
                visible: gmwItem.itemType === GmwItem.TypeVehicle
                columns: 2

                Label {
                    height: fuelRow.height
                    text: gmwItem.engineType === GmwVehicle.EngineTypeED ? qsTr("Battery level:") : qsTr("Fuel level:")
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
                    text: qsTr("Interior State:")
    //                            height: stateImage.height
                    verticalAlignment: Text.AlignVCenter
                }

                Image {
                    id: stateImage
                    source: vehicleGrid.stateToIconSource(gmwItem.interiorState)
                }
                Label {
                    text: qsTr("Exterior State:")
    //                            height: stateImage.height
                    verticalAlignment: Text.AlignVCenter
                }

                Image {
                    source: vehicleGrid.stateToIconSource(gmwItem.exteriorState)
                }
            }

            SectionHeader {
                width: parent.width
                headerText: qsTr("Availability")
                visible: gmwItem.itemType === GmwItem.TypeParkingSpot || gmwItem.itemType === GmwItem.TypeVehicle
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
                visible: gmwItem.itemType === GmwItem.TypeVehicle
                text: gmwItem.booking.valid && !gmwItem.booking.expired ? qsTr("Cancel") : qsTr("Book")
                onClicked: {
                    if(gmwEngine.defaultAccountName.length === 0) {
                        PopupUtils.open(createBookingDialog, itemDetailsSheet, {state: "error" })
                        return;
                    }

                    if(gmwItem.booking.valid && !gmwItem.booking.expired) {
                        PopupUtils.open(createBookingDialog, itemDetailsSheet, {state: "cancel", gmwItem: gmwItem })
                    } else {
                        PopupUtils.open(createBookingDialog, itemDetailsSheet, {state: "book", gmwItem: gmwItem })
                    }
                }

                Component {
                    id: createBookingDialog
                    CreateBookingDialog {
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
                    text: (gmwItem.capacityTotal - gmwItem.capacityUsed) + " " + qsTr("free") + " / " + gmwItem.capacityTotal + " " + qsTr("total")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

//    Dialog {
//        id: createBookingDialog
//        width: parent.width
//        property QtObject gmwItem

//        states: [
//            State {
//                name: "book"
//                PropertyChanges { target: createBookingHeaderLabel; text: qsTr("Create booking?") }
//                PropertyChanges { target: createBookingTextLabel; text: qsTr("The booking will be valid for 30 minutes from now.")}
//            },
//            State {
//                name: "cancel"
//                PropertyChanges { target: createBookingHeaderLabel; text: qsTr("Cancel booking") }
//                PropertyChanges { target: createBookingTextLabel; text: qsTr("Are you sure?")}
//            },
//            State {
//                name: "error"
//                PropertyChanges { target: createBookingHeaderLabel; text: qsTr("Not authorized") }
//                PropertyChanges { target: createBookingTextLabel; text: qsTr("To be able to create bookings for cars you need to authorize GetMeWheels and select the account which will be charged for the ride.") + "\n\n" + qsTr("Open settings now?")}
//            }
//        ]
//        title: Column {
//            width: parent.width
//            Label {
//                id: createBookingHeaderLabel
//                width: parent.width
//                font.pixelSize: 40
//                color: "white"
//            }
//        }

//        Column {
//            width: parent.width
//            Label {
//                id: createBookingTextLabel
//                width: parent.width
//                wrapMode: Text.WordWrap
//                color: "white"
//            }
//            Item {
//                width: parent.width
//                height: 50
//            }
//        }

//        buttons {
//            Row {
//                width: parent.width
//                Button {
//                    id: yesButton
//                    text: qsTr("Yes")
//                    onClicked: {
//                        if(createBookingDialog.state == "book") {
//                            if(gmwEngine.createBooking(createBookingDialog.gmwItem)) {
//                                infoBanner.text = qsTr("Car booked successfully");
//                                infoBanner.show();
//                            } else {
//                                infoBanner.text = qsTr("Failed to create booking: %1").arg(gmwEngine.error());
//                                infoBanner.show();
//                            }
//                        } else if(createBookingDialog.state == "cancel") {
//                            if(gmwEngine.cancelBooking(createBookingDialog.gmwItem)) {
//                                infoBanner.text = qsTr("Booking cancelled successfully");
//                                infoBanner.show();
//                            } else {
//                                infoBanner.text = qsTr("Failed to cancel booking: %1").arg(gmwEngine.error());
//                                infoBanner.show();
//                            }
//                        } else if(createBookingDialog.state == "error") {
//                            var component = Qt.createComponent("SettingsSheet.qml")
//                            if (component.status == Component.Ready) {
//                                var sheet = component.createObject(itemDetailsSheet);
//                                sheet.open();
//                            } else {
//                                console.log("Error loading component:", component.errorString());
//                            }
//                        }
//                        createBookingDialog.close();
//                    }
//                }
//                Button {
//                    id: noButton
//                    text: qsTr("No")
//                    onClicked: createBookingDialog.close();
//                }
//            }
//        }
//    }

}
