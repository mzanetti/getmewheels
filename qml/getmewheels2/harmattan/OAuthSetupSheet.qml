import QtQuick 1.1
import QtWebKit 1.0
import com.nokia.meego 1.0

Sheet {
    id: root
    rejectButtonText: qsTr("Cancel")
    state: "step1"

    property string callbackUrl: "http://notyetthere.org/getmewheels/oauth_callback"

    Connections {
        target: gmwEngine
        onAuthenticateUrl: {
            webView.url = url;
        }
    }

    Component.onCompleted: {
        gmwEngine.startAuthentication();
    }

    content: Item {
        anchors.fill: parent
        Flickable {
            id: flickable
            anchors.fill: parent
            contentWidth: Math.max(parent.width,webView.width)
            contentHeight: Math.max(parent.height,webView.height)
            pressDelay: 200

            WebView {
                id: webView
                preferredWidth: flickable.width
                preferredHeight: flickable.height

                onUrlChanged: {
                    print("url changed *******", url)
                    var urlString = url.toString();
                    if (urlString.substring(0, callbackUrl.length) == callbackUrl) {
                        var params = urlString.split('&');
                        params = params[params.length-1].split('=');
                        var verifier = params[params.length-1];
                        print("setting verifier", verifier);
                        webView.visible = false;
                        if (gmwEngine.setAccessCode(verifier)) {
                            statusLabel.text = qsTr("GetMeWheels authorized successfully. You should now be able to select the billed account and manage your bookings for cars.") + " <b>" + qsTr("The authorization will expire in 31 days.") + "</b>"
                            root.acceptButtonText = qsTr("OK")
                            root.rejectButtonText = ""
                        } else {
                            qsTr("Failed to authorize GetMeWheels. Please try again.")
                        }

                        statusLabel.visible = true
                    }
                }
            }
        }
        Label {
            id: statusLabel
            anchors.centerIn: parent
            width: parent.width - 100
            wrapMode: Text.WordWrap
            visible: false
        }
    }
//        Column {
//        anchors.fill: parent
//        anchors.margins: 10
//        spacing: 10

//        SectionHeader {
//            id: step1Header
//            width: parent.width
//            headerText: qsTr("Step 1")
//            visible: false
//        }

//        Label {
//            id: step1Label
//            width: parent.width
//            wrapMode: Text.WordWrap
//            text: qsTr("Get the authorization code from the car2go website. Log in with your car2go account and grant access to GetMeWheels.")
//            visible: false
//        }

//        Button {
//            id: getCodeButton
//            text: qsTr("Get code")
//            width: parent.width
//            visible: false
//            onClicked: {
//                gmwEngine.startAuthentication();
//                settingsSheet.state = "step2";
//            }
//        }

//        SectionHeader {
//            id: step2Header
//            width: parent.width
//            headerText: qsTr("Step 2")
//            visible: false
//        }

//        Label {
//            id: codeLabel
//            width: parent.width
//            text: qsTr("Enter the code in the field below.")
//            visible: false
//        }

//        Row {
//            id: codeRow
//            width: parent.width
//            spacing: 10
//            visible: false
//            TextArea {
//                id: codeArea
//                width: parent.width - confirmButton.width - 10
//                onTextChanged: {
//                    if(text.length > 0) {
//                        confirmButton.enabled = true;
//                    } else {
//                        confirmButton.enabled = false;
//                    }
//                }
//            }
//            Button {
//                id: confirmButton
//                text: qsTr("OK")
//                width: 80
//                anchors.verticalCenter: parent.verticalCenter
//                onClicked: {
//                    if(gmwEngine.setAccessCode(codeArea.text)) {
//                        settingsSheet.state = "step3";
//                    } else {
//                        settingsSheet.state = "step4";
//                    }
//                }
//            }
//        }

//        SectionHeader {
//            id: step3Header
//            width: parent.width
//            headerText: qsTr("Result")
//            visible: false
//        }

//        Label {
//            id: statusLabel
//            width: parent.width
//            visible: false
//            wrapMode: Text.WordWrap
//        }


//    }
//    states: [
//        State {
//            name: "step1"
//            PropertyChanges { target: step1Header; visible: true }
//            PropertyChanges { target: step1Label; visible: true }
//            PropertyChanges { target: getCodeButton; visible: true }
//        },
//        State {
//            name: "step2"
//            PropertyChanges { target: step1Header; visible: true }
//            PropertyChanges { target: step1Label; visible: true }
//            PropertyChanges { target: getCodeButton; visible: true }
//            PropertyChanges { target: step2Header; visible: true }
//            PropertyChanges { target: codeLabel; visible: true }
//            PropertyChanges { target: codeRow; visible: true }
//        },
//        State {
//            name: "step3"
//            PropertyChanges { target: step3Header; visible: true; headerText: qsTr("Done") }
//            PropertyChanges { target: statusLabel; visible: true; text: qsTr("GetMeWheels authorized successfully. You should now be able to select the billed account and manage your bookings for cars.") + " <b>" + qsTr("The authorization will expire in 31 days.") + "</b>"}
//            PropertyChanges { target: settingsSheet; acceptButtonText: qsTr("OK") ; rejectButtonText: "" }
//        },
//        State {
//            name: "step4"
//            PropertyChanges { target: step1Header; visible: true }
//            PropertyChanges { target: step1Label; visible: true }
//            PropertyChanges { target: getCodeButton; visible: true }
//            PropertyChanges { target: step3Header; visible: true; headerText: qsTr("Error") }
//            PropertyChanges { target: statusLabel; visible: true; text: qsTr("Failed to authorize GetMeWheels. Please try again.") }
//        }

//    ]
}
