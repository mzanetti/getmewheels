import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: settingsSheet
    orientationLock: PageOrientation.LockPortrait
//    rejectButtonText: "Cancel"
    state: "step1"

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "toolbar-back"
//            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: pageStack.pop();
        }
    }

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
            text: qsTr("Get the authorization code from the car2go website. Log in with your car2go account and grant access to GetMeWheels.")
            visible: false
        }

        Button {
            id: getCodeButton
            text: qsTr("Get code")
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
            text: qsTr("Enter the code in the field below.")
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
                text: qsTr("OK")
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
            PropertyChanges { target: step3Header; visible: true; headerText: qsTr("Done") }
            PropertyChanges { target: statusLabel; visible: true; text: qsTr("GetMeWheels authorized successfully. You should now be able to select the billed account and manage your bookings for cars.") + " <b>" + qsTr("The authorization will expire in 31 days.") + "</b>" }
        },
        State {
            name: "step4"
            PropertyChanges { target: step1Header; visible: true }
            PropertyChanges { target: step1Label; visible: true }
            PropertyChanges { target: getCodeButton; visible: true }
            PropertyChanges { target: step3Header; visible: true; headerText: qsTr("Error") }
            PropertyChanges { target: statusLabel; visible: true; text: qsTr("Failed to authorize GetMeWheels. Please try again.") }
        }

    ]
}
