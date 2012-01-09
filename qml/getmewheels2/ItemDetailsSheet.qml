import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import GetMeWheels 1.0

Sheet {
    id: settingsSheet
    acceptButtonText: "Go to"
    rejectButtonText: "Close"

    property QtObject gmwItem

    content: Column {
        id: contentColumn
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Label {
            text: gmwItem.name
            font.pixelSize: 40
            width: parent.width
            wrapMode: Text.WordWrap
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
            visible: gmwItem.itemType == GmwItem.TypeVehicle
        }

        Grid {
            id: vehicleGrid
            spacing: 10
            visible: gmwItem.itemType == GmwItem.TypeVehicle
            columns: 2

            Label {
                height: fuelRow.height
                text: "Fuel level:"
                verticalAlignment: Text.AlignVCenter
            }

            Row {
                id: fuelRow
                spacing: 10
                Image {
                    source: gmwItem.fuelLevel > 25 ? "images/fuel_green.png" : gmwItem.fuelLevel > 15 ? "images/fuel_orange.png" : "images/fuel_red.png"
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
            visible: gmwItem.itemType == GmwItem.TypeParkingLot || gmwItem.itemType == GmwItem.TypeVehicle
        }


        Row {
            width: parent.width
            visible: gmwItem.itemType == GmwItem.TypeVehicle
            spacing: 10
            Image {
                id: availabilityIcon
                source: gmwItem.booking.valid ? (gmwItem.booking.expired ? "image://theme/icon-m-common-presence-busy" : "image://theme/icon-m-common-presence-away") : "image://theme/icon-m-common-presence-online"
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
                    gmwEngine.cancelBooking(gmwItem);
                } else {
                    if(gmwEngine.createBooking(gmwItem)) {
                        okBanner.show();
                    } else {
                        errorBanner.errorText = gmwEngine.error();
                        errorBanner.show();
                    }
                }
            }
        }

        Row {
            spacing: 10
            visible: gmwItem.itemType === GmwItem.TypeParkingLot
            Image {
                id: parkingFreeIcon
                source: gmwItem.capacityTotal - gmwItem.capacityUsed === 0 ? "image://theme/icon-m-common-presence-busy" : "image://theme/icon-m-common-presence-online"
            }

            Label {
                text: (gmwItem.capacityTotal - gmwItem.capacityUsed) + " free / " + gmwItem.capacityTotal + " total"
            }
        }
    }

    InfoBanner {
        id: okBanner
        text: "Car booked successfully"
        timerShowTime: 5000
    }

    InfoBanner {
        id: errorBanner
        text: "Failed to create booking: " + errorText
        timerShowTime: 5000

        property string errorText
    }
}
