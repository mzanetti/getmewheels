import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: settingsSheet
    orientationLock: PageOrientation.LockPortrait
//    acceptButtonText: "Save"
//    rejectButtonText: "Cancel"

    property alias locationName: locationButton.subTitleText
    property alias accountName: accountSelectionButton.subTitleText

    signal accepted();

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "toolbar-back"
//            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: settingsSheet.accepted();
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        SectionHeader {
            width: parent.width
            headerText: qsTr("General")
        }

        SelectionButton {
            id: locationButton
            titleText: "Location"
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

//        Item {
//            id: spacer1
//            width: parent.width
//            height: 20
//        }

        SectionHeader {
            width: parent.width
            headerText: qsTr("car2go account")
        }

        Label {
            width: parent.width
            text: "To be able to create bookings for cars you need to be signed in with your car2go account. Please visit http://www.car2go.com for detailed informations on costs."
            wrapMode: Text.WordWrap
        }

        Label {
            width: parent.width
            text: "Authentication expiry: " + Qt.formatDate(gmwEngine.authExpirationDate)
            wrapMode: Text.WordWrap
            visible: gmwEngine.authenticated
        }

        Button {
            text: gmwEngine.authenticated ? "Renew Authentication" : "Authenticate GetMeWheels"
            width: parent.width
            onClicked: {
                oauthSetupSheet.state = "step1";
                pageStack.push(oauthSetupSheet);
            }
        }

        SelectionButton {
            id: accountSelectionButton
            titleText: "Account"
            subTitleText: gmwEngine.defaultAccountName
            width: parent.width
            enabled: gmwEngine.authenticated
            onClicked: {
                selectAccountDialog.model = undefined;
                accountModel.clear();
                gmwEngine.accountNames(locationButton.subTitleText).forEach(function(item) { accountModel.append({name: item}) } );
                selectAccountDialog.model = accountModel;
                selectAccountDialog.open();
            }
        }



    }

    ListModel { // Fill with dummy stuff to make the Dialog resize itself propertly
        id: locationsModel
        ListElement { name: "0" }
        ListElement { name: "0" }
        ListElement { name: "0" }
        ListElement { name: "0" }
        ListElement { name: "0" }
        ListElement { name: "0" }
        ListElement { name: "0" }
        ListElement { name: "0" }
    }
    SelectionDialog {
        id: selectLocationsDialog
        titleText: "Select Location"
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
        titleText: "Select Account"
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
