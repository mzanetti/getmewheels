import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.0
import GetMeWheels 1.0

Page {
    id: settingsSheet
    orientationLock: PageOrientation.LockPortrait
//    acceptButtonText: "Go to"
//    rejectButtonText: "Close"

    signal goTo(variant item)

    //onAccepted: goTo(listView.currentItem.gmwItem);

    property alias model: listView.model
    property alias currentItem: listView.currentItem

//        PageIndicator {
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.bottom: parent.bottom
////            width: parent.width
//            height: 20
//            visible: totalPages > 1

//            id: indicator
//            objectName: "pageIndicatorObject"
//        }
    Row {
        id: indicator
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        height: 5
        spacing: 5
        Repeater {
            model: settingsSheet.model
            Rectangle {
                color: "white"
                opacity: index === listView.currentIndex ? 1 : .6
                border.width: 1
                width: 5
                height: 5
            }
        }
    }

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "toolbar-back"
//            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: pageStack.pop();
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-play"
//            anchors.left: (parent === undefined) ? undefined : parent.left
            visible: appWindow.gpsAvailable
            onClicked: {
                settingsSheet.goTo(settingsSheet.currentItem.gmwItem);
            }
        }
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

//            onModelChanged: indicator.model = count;
//            onCurrentIndexChanged: indicator.currentPage = currentIndex + 1;

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
                                font.pixelSize: text.length > 14 ? 30 : (text.length > 10 ? 35 : 40)
                                wrapMode: Text.WordWrap
                                elide: Text.ElideRight
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
                                return "images/smiley_sad.png";
                            case GmwVehicle.StateIssuesExist:
                                return "images/smiley_sad.png";
                            case GmwVehicle.StateSatisfied:
                                return "images/smiley_happy.png";
                            case GmwVehicle.StateGood:
                                return "images/smiley_happy.png";
                            case GmwVehicle.StateExcellent:
                                return "images/smiley_happy.png";
                            }
                            return "images/smiley_unknown.png";
                        }

                        function stateToString(state) {
                            switch(state) {
                            case GmwVehicle.StateUnacceptable:
                                return qsTr("Bad");
                            case GmwVehicle.StateGood:
                                return qsTr("Good");
                            }
                            return qsTr("Unknown");
                        }

                        Label {
                            text: qsTr("Interior State:")
//                            height: stateImage.height
                            verticalAlignment: Text.AlignVCenter
                        }

                        Row {
                            spacing: 10

                            Image {
                                id: stateImage
                                source: vehicleGrid.stateToIconSource(gmwItem.interiorState)
                            }
                            Label {
                                text: vehicleGrid.stateToString(gmwItem.interiorState)
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        Label {
                            text: qsTr("Exterior State:")
//                            height: stateImage.height
                            verticalAlignment: Text.AlignVCenter
                        }

                        Row {
                            spacing: 10
                            Image {
                                source: vehicleGrid.stateToIconSource(gmwItem.exteriorState)
                            }
                            Label {
                                text: vehicleGrid.stateToString(gmwItem.exteriorState)
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }

                    SectionHeader {
                        width: parent.width
                        headerText: qsTr("Availability")
                        visible: gmwItem.itemType == GmwItem.TypeParkingSpot  || gmwItem.itemType == GmwItem.TypeVehicle
                    }


                    Row {
                        width: parent.width
                        visible: gmwItem.itemType === GmwItem.TypeVehicle
                        spacing: 10
                        Image {
                            id: availabilityIcon
                            visible: gmwItem.booking.valid
                            width: 32
                            height: 32
                            property int timeLeft: gmwItem.booking.timeLeft
                            source: timeLeft > 900 ? "images/bookingstate_green.png" : (timeLeft > 0 ?"images/bookingstate_yellow.png" : "images/bookingstate_red.png")
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
                        text: gmwItem.booking.valid && !gmwItem.booking.expired ? qsTr("Cancel") : qsTr("Book")
                        onClicked: {
                            if(gmwEngine.defaultAccountName.length === 0) {
                                createBookingDialog.state = "error"
                                createBookingDialog.open();
                                return;
                            }

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
//                        Image {
//                            id: parkingFreeIcon
//                            source: gmwItem.capacityTotal - gmwItem.capacityUsed === 0 ? "image://theme/icon-m-common-presence-busy" : "image://theme/icon-m-common-presence-online"
//                        }

                        Label {
                            text: (gmwItem.capacityTotal - gmwItem.capacityUsed) + " " + qsTr("free") + " / " + gmwItem.capacityTotal + " " + qsTr("total")
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

            }
        }


    Dialog {
        id: createBookingDialog
        width: parent.width
        height: 250
        property QtObject gmwItem

        states: [
            State {
                name: "book"
                PropertyChanges { target: createBookingHeaderLabel; text: qsTr("Create booking?") }
                PropertyChanges { target: createBookingTextLabel; text: qsTr("The booking will be valid for 30 minutes from now.")}
            },
            State {
                name: "cancel"
                PropertyChanges { target: createBookingHeaderLabel; text: qsTr("Cancel booking") }
                PropertyChanges { target: createBookingTextLabel; text: qsTr("Are you sure?")}
            },
            State {
                name: "error"
                PropertyChanges { target: createBookingHeaderLabel; text: qsTr("Not authorized") }
                PropertyChanges { target: createBookingTextLabel; text: qsTr("To be able to create bookings for cars you need to authorize GetMeWheels and select the account which will be charged for the ride.") + "\n\n" + qsTr("Open settings now?")}
            }
        ]
        title: Column {
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                id: createBookingHeaderLabel
                width: parent.width
                font.pixelSize: 40
                color: "white"
            }
        }

        content: Column {
            anchors.fill: parent
            anchors.margins: 10
            //anchors.horizontalCenter: parent.horizontalCenter
            Label {
                id: createBookingTextLabel
                width: parent.width
                wrapMode: Text.WordWrap
                color: "white"
            }
        }
        buttons {
            ButtonRow {
                width: parent.width
                Button {
                    id: yesButton
                    text: qsTr("Yes");
                    onClicked: {
                        if(createBookingDialog.state == "book") {
                            if(gmwEngine.createBooking(createBookingDialog.gmwItem)) {
                                infoBanner.text = qsTr("Car booked successfully");
                                infoBanner.open();
                            } else {
                                infoBanner.text = qsTr("Failed to create booking: %1").arg(gmwEngine.error());
                                infoBanner.open();
                            }
                        } else if(createBookingDialog.state == "cancel") {
                            if(gmwEngine.cancelBooking(createBookingDialog.gmwItem)) {
                                infoBanner.text = qsTr("Booking cancelled successfully");
                                infoBanner.open();
                            } else {
                                infoBanner.text = qsTr("Failed to cancel booking: %1").arg(gmwEngine.error());
                                infoBanner.open();
                            }

                        }else if(createBookingDialog.state == "error") {
                            var component = Qt.createComponent("SettingsSheet.qml")
                            if (component.status == Component.Ready) {
                                pageStack.push(component);
                            } else {
                                console.log("Error loading component:", component.errorString());
                            }
                        }
                        createBookingDialog.close();
                    }
                }
                Button {
                    id: noButton
                    text: qsTr("No");
                    onClicked: createBookingDialog.close();
                }
            }
        }
    }

    InfoBanner {
        id: infoBanner
//        timerShowTime: 5000
    }
}
