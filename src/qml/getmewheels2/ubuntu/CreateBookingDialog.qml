import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1

Dialog {
    id: createBookingDialog

    title: qsTr("Create booking?")
    text: qsTr("The booking will be valid for 30 minutes from now.")

    property QtObject gmwItem

    Component.onCompleted: print("humppa")

    states: [
        State {
            name: "book"
            PropertyChanges { target: createBookingDialog; title: qsTr("Create booking?") }
            PropertyChanges { target: createBookingDialog; text: qsTr("The booking will be valid for 30 minutes from now.")}
        },
        State {
            name: "cancel"
            PropertyChanges { target: createBookingDialog; title: qsTr("Cancel booking") }
            PropertyChanges { target: createBookingDialog; text: qsTr("Are you sure?")}
        },
        State {
            name: "error"
            PropertyChanges { target: createBookingDialog; title: qsTr("Not authorized") }
            PropertyChanges { target: createBookingDialog; text: qsTr("To be able to create bookings for cars you need to authorize GetMeWheels and select the account which will be charged for the ride.") + "\n\n" + qsTr("Open settings now?")}
        }
    ]

    Button {
        id: yesButton
        text: qsTr("Yes")
        onClicked: {
            if(createBookingDialog.state == "book") {
                if(gmwEngine.createBooking(createBookingDialog.gmwItem)) {
                    infoBanner.text = qsTr("Car booked successfully");
                    infoBanner.show();
                } else {
                    infoBanner.text = qsTr("Failed to create booking: %1").arg(gmwEngine.error());
                    infoBanner.show();
                }
            } else if(createBookingDialog.state == "cancel") {
                if(gmwEngine.cancelBooking(createBookingDialog.gmwItem)) {
                    infoBanner.text = qsTr("Booking cancelled successfully");
                    infoBanner.show();
                } else {
                    infoBanner.text = qsTr("Failed to cancel booking: %1").arg(gmwEngine.error());
                    infoBanner.show();
                }
            } else if(createBookingDialog.state == "error") {
                var component = Qt.createComponent("SettingsSheet.qml")
                if (component.status == Component.Ready) {
                    var sheet = component.createObject(itemDetailsSheet);
                    sheet.open();
                } else {
                    console.log("Error loading component:", component.errorString());
                }
            }
            PopupUtils.close(createBookingDialog);
        }
    }
    Button {
        id: noButton
        text: qsTr("No")
        onClicked: PopupUtils.close(createBookingDialog);
    }
}
