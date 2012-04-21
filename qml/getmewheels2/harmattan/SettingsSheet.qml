import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {
    id: settingsSheet

    function done() {
        accept();
        if(gmwEngine.defaultAccountName !== accountName) {
            gmwEngine.defaultAccountName = accountName;
        }
        if(gmwEngine.locationName !==locationName) {
            mainPage.tracking= false;
            gmwModel.clearAll();
            gmwEngine.locationName = locationName;
        }
    }

    buttons: Item {
        id: buttonRow
        anchors.fill: parent
        SheetButton {
            text: qsTr("Cancel")
            anchors.left: parent.left
            anchors.leftMargin: settingsSheet.platformStyle.rejectButtonLeftMargin
            anchors.verticalCenter: parent.verticalCenter
            onClicked: reject()
        }
        SheetButton {
            text: qsTr("Save")
            enabled: map.gpsAvailable
            anchors.right: parent.right
            anchors.rightMargin: settingsSheet.platformStyle.acceptButtonRightMargin
            anchors.verticalCenter: parent.verticalCenter
            platformStyle: SheetButtonAccentStyle { }
            onClicked: {
                if(gmwEngine.authenticated && gmwEngine.defaultAccountName.length === 0) {
                    accountCheckDialog.open();
                } else {
                    settingsSheet.done();
                }
            }
        }

    }

    property alias locationName: locationButton.subTitleText
    property alias accountName: accountSelectionButton.subTitleText

    Dialog {
        id: accountCheckDialog
        width: parent.width
        height: 250

        title: Column {
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                width: parent.width
                font.pixelSize: 40
                color: "white"
                text: qsTr("Warning")
            }
        }

        content: Column {
            height: textLabel.height
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            Label {
                id: textLabel
                width: parent.width
                wrapMode: Text.WordWrap
                color: "white"
                text: qsTr("GetMeWheels is authorized for bookings but there is no account selected yet. Do you want to select an account now?")
            }
        }
        buttons {
            ButtonRow {
                width: parent.width
                Button {
                    id: yesButton
                    text: qsTr("Yes");
                    onClicked: {
                        accountCheckDialog.close();
                        openAccountSelection();
                    }
                }
                Button {
                    id: noButton
                    text: qsTr("No");
                    onClicked: {
                        accountCheckDialog.close();
                        settingsSheet.done();
                    }
                }
            }
        }
    }

    function openAccountSelection() {
        selectAccountDialog.model = undefined;
        accountModel.clear();
        gmwEngine.accountNames(locationButton.subTitleText).forEach(function(item) { accountModel.append({name: item}) } );
        selectAccountDialog.model = accountModel;
        selectAccountDialog.open();
    }


    content: Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        SectionHeader {
            width: parent.width
            headerText: qsTr("General")
        }

        SelectionButton {
            id: locationButton
            titleText: qsTr("Location")
            subTitleText: gmwEngine.locationName
            width: parent.width
            onClicked: {
                selectLocationsDialog.model = undefined;
                locationsModel.clear();
                gmwEngine.supportedLocationNames().forEach(function(item) { print("got location:" + item); locationsModel.append( {name: item} ) });
                selectLocationsDialog.model = locationsModel;
                selectLocationsDialog.open();
            }
        }

        Item {
            id: spacer1
            width: parent.width
            height: 20
        }

        SectionHeader {
            width: parent.width
            headerText: qsTr("car2go account")
        }

        Label {
            width: parent.width
            text: qsTr("To be able to create bookings for cars you need to authorize GetMeWheels and select the account which will be charged for the ride.")
            wrapMode: Text.WordWrap
        }

        Label {
            width: parent.width
            text: qsTr("Authorization expiry:") + " " + Qt.formatDate(gmwEngine.authExpirationDate)
            wrapMode: Text.WordWrap
            visible: gmwEngine.authenticated
        }

        Button {
            text: gmwEngine.authenticated ? qsTr("Renew Authorization") : qsTr("Authorize GetMeWheels")
            width: parent.width
            onClicked: {
                oauthSetupSheet.state = "step1";
                oauthSetupSheet.open();
            }
        }
        Button {
            text: qsTr("Clear Authorization")
            width: parent.width
            visible: gmwEngine.authenticated
            onClicked: {
                gmwEngine.clearDefaultAccount();
                gmwEngine.clearAuthentication();
            }
        }

        SelectionButton {
            id: accountSelectionButton
            titleText: qsTr("Account")
            subTitleText: gmwEngine.defaultAccountName
            width: parent.width
            enabled: gmwEngine.authenticated
            onClicked: {
                openAccountSelection();
            }
        }



    }

    ListModel {
        id: locationsModel
    }
    SelectionDialog {
        id: selectLocationsDialog
        titleText: qsTr("Select Location")
        model: locationsModel
        onAccepted: {
            locationButton.subTitleText = locationsModel.get(selectLocationsDialog.selectedIndex).name;
        }
    }

    ListModel {
        id: accountModel
    }
    SelectionDialog {
        id: selectAccountDialog
        titleText: qsTr("Select Account")
        onAccepted: {
            print("selected: "+ accountModel.get(selectAccountDialog.selectedIndex).name);
            gmwEngine.defaultAccountName = accountModel.get(selectAccountDialog.selectedIndex).name;
//            accountSelectionButton.subTitleText = gmwEngine.defaultAccountName();
        }
    }

    OAuthSetupSheet {
        id: oauthSetupSheet
    }
}
