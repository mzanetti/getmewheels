import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
//import Unity.Notifications 1.0

Dialog {
    id: createBookingDialog

    title: qsTr("Create booking?")
    text: qsTr("The booking will be valid for 30 minutes from now.")

    property QtObject gmwItem

    Component.onCompleted: print("humppa")

    signal openSettings();

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
        },
        State {
            name: "bookingDone"
            PropertyChanges { target: createBookingDialog; title: qsTr("Booking successful") }
            PropertyChanges { target: createBookingDialog; text: qsTr("Car booked successfully")}
            PropertyChanges { target: noButton; text: qsTr("OK")}
            PropertyChanges { target: yesButton; visible: false}
        },
        State {
            name: "bookingError"
            PropertyChanges { target: createBookingDialog; title: qsTr("Booking failed") }
            PropertyChanges { target: createBookingDialog; text: qsTr("Failed to create booking: %1").arg(gmwEngine.error())}
            PropertyChanges { target: noButton; text: qsTr("OK")}
            PropertyChanges { target: yesButton; visible: false}
        },
        State {
            name: "cancelDone"
            PropertyChanges { target: createBookingDialog; title: qsTr("Cancel successful") }
            PropertyChanges { target: createBookingDialog; text: qsTr("Booking cancelled successfully")}
            PropertyChanges { target: noButton; text: qsTr("OK")}
            PropertyChanges { target: yesButton; visible: false}
        },
        State {
            name: "cancelError"
            PropertyChanges { target: createBookingDialog; title: qsTr("Cancel failed") }
            PropertyChanges { target: createBookingDialog; text: qsTr("Failed to cancel booking: %1").arg(gmwEngine.error())}
            PropertyChanges { target: noButton; text: qsTr("OK")}
            PropertyChanges { target: yesButton; visible: false}
        }
    ]

    Button {
        id: yesButton
        text: qsTr("Yes")
        onClicked: {
            if(createBookingDialog.state == "book") {
                if(gmwEngine.createBooking(createBookingDialog.gmwItem)) {
                    createBookingDialog.state = "bookingDone"
                } else {
                    createBookingDialog.state = "bookingError"
                }
            } else if(createBookingDialog.state == "cancel") {
                if(gmwEngine.cancelBooking(createBookingDialog.gmwItem)) {
                    createBookingDialog.state = "cancelDone"
                } else {
                    createBookingDialog.state = "cancelError"
                }
            } else if(createBookingDialog.state == "error") {
                createBookingDialog.openSettings();
                PopupUtils.close(createBookingDialog);
            }

        }
    }
    Button {
        id: noButton
        text: qsTr("No")
        onClicked: PopupUtils.close(createBookingDialog);
    }

}
