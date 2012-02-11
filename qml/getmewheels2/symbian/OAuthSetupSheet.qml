import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: settingsSheet
//    rejectButtonText: "Cancel"
    state: "step1"

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        SectionHeader {
            id: step1Header
            width: parent.width
            headerText: qsTr("Step 1")
            visible: false
        }

        Label {
            id: step1Label
            width: parent.width
            wrapMode: Text.WordWrap
            text: "Get the authentication code from the car2go website. Log in with your car2go account and grant access to GetMeWheels"
            visible: false
        }

        Button {
            id: getCodeButton
            text: "Get code"
            width: parent.width
            visible: false
            onClicked: {
                gmwEngine.startAuthentication();
                settingsSheet.state = "step2";
            }
        }

        SectionHeader {
            id: step2Header
            width: parent.width
            headerText: qsTr("Step 2")
            visible: false
        }

        Label {
            id: codeLabel
            width: parent.width
            text: "Enter the code in the field below."
            visible: false
        }

        Row {
            id: codeRow
            width: parent.width
            spacing: 10
            visible: false
            TextArea {
                id: codeArea
                width: parent.width - confirmButton.width - 10
                onTextChanged: {
                    if(text.length > 0) {
                        confirmButton.enabled = true;
                    } else {
                        confirmButton.enabled = false;
                    }
                }
            }
            Button {
                id: confirmButton
                text: "OK"
                width: 80
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    if(gmwEngine.setAccessCode(codeArea.text)) {
                        settingsSheet.state = "step3";
                    } else {
                        settingsSheet.state = "step4";
                    }
                }
            }
        }

        SectionHeader {
            id: step3Header
            width: parent.width
            headerText: qsTr("Result")
            visible: false
        }

        Label {
            id: statusLabel
            width: parent.width
            visible: false
            wrapMode: Text.WordWrap
        }


    }
    states: [
        State {
            name: "step1"
            PropertyChanges { target: step1Header; visible: true }
            PropertyChanges { target: step1Label; visible: true }
            PropertyChanges { target: getCodeButton; visible: true }
        },
        State {
            name: "step2"
            PropertyChanges { target: step1Header; visible: true }
            PropertyChanges { target: step1Label; visible: true }
            PropertyChanges { target: getCodeButton; visible: true }
            PropertyChanges { target: step2Header; visible: true }
            PropertyChanges { target: codeLabel; visible: true }
            PropertyChanges { target: codeRow; visible: true }
        },
        State {
            name: "step3"
            PropertyChanges { target: step3Header; visible: true; headerText: "Done" }
            PropertyChanges { target: statusLabel; visible: true; text: "GetMeWheels authenticated successfully. You should now be able to select an account and manage your bookings for cars. <b>The authentication will expire in 31 days.</b>" }
            PropertyChanges { target: settingsSheet; acceptButtonText: "OK" ; rejectButtonText: "" }
        },
        State {
            name: "step4"
            PropertyChanges { target: step1Header; visible: true }
            PropertyChanges { target: step1Label; visible: true }
            PropertyChanges { target: getCodeButton; visible: true }
            PropertyChanges { target: step3Header; visible: true; headerText: "Error" }
            PropertyChanges { target: statusLabel; visible: true; text: "Failed to authenticate GetMeWheels. Please try again." }
        }

    ]
}
