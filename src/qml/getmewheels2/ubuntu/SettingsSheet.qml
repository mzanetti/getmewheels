import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Themes.Ambiance 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems

ComposerSheet {
    id: settingsSheet

    Component.onCompleted: {
        __foreground.__styleInstance.headerColor = "#0249b3"
        __foreground.__styleInstance.backgroundColor = "#0365bf"
    }

    function done() {
        /*        if(gmwEngine.defaultAccountName !== accountName) {
            gmwEngine.defaultAccountName = accountName;
        }
*/        if(gmwEngine.locationName !== locationButton.text) {
            //            mainPage.tracking= false;
            gmwModel.clearAll();
            gmwEngine.locationName = locationButton.text;
        }
    }

//    onConfirmClicked: {
//        done();
//    }

    ListModel {
        id: locationsModel
    }

    Flickable {
        anchors.fill: parent
        anchors.margins: 10
        contentHeight: contentColumn.height
        clip: true
        Column {
            id: contentColumn
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10

            ListItems.ValueSelector {
                width: parent.width
                text: qsTr("Location:")
                values: gmwEngine.supportedLocationNames()
                selectedIndex: values.indexOf(gmwEngine.locationName)

                onSelectedIndexChanged: {
                    if (gmwEngine.locationName != values[selectedIndex]) {
                        gmwEngine.locationName = values[selectedIndex];
                        accountSelectionButton.values = gmwEngine.accountNames(gmwEngine.locationName);
                    }
                }
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
                    var oauthPopup = PopupUtils.open(Qt.resolvedUrl("OAuthSetupSheet.qml"), settingsSheet);
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


            ListItems.ValueSelector {
                id: accountSelectionButton
                text: qsTr("Account")
                width: parent.width
                enabled: gmwEngine.authenticated
                values: enabled ? gmwEngine.accountNames(gmwEngine.locationName) : []
                selectedIndex: values.indexOf(gmwEngine.defaultAccountName)

                onSelectedIndexChanged: {
                    if (gmwEngine.defaultAccountName != values[selectedIndex]) {
                        gmwEngine.defaultAccountName = values[selectedIndex]
                    }
                }
                onValuesChanged: {
                    if (values.length == 1) {
                        selectedIndex = 0;
                    }
                }
            }
        }
    }
}



//    function done() {
//        if(gmwEngine.defaultAccountName !== accountName) {
//            gmwEngine.defaultAccountName = accountName;
//        }
//        if(gmwEngine.locationName !==locationName) {
//            mainPage.tracking= false;
//            gmwModel.clearAll();
//            gmwEngine.locationName = locationName;
//        }
//    }

//    onConfirmClicked: {
//        done();
//    }


//    property alias locationName: locationButton.text
//    property alias accountName: accountSelectionButton.subTitleText

//    Dialog {
//        id: accountCheckDialog
//        width: parent.width
//        height: 250

//        title: Column {
//            width: parent.width - 20
//            anchors.horizontalCenter: parent.horizontalCenter

//            Label {
//                width: parent.width
//                font.pixelSize: 40
//                color: "white"
//                text: qsTr("Warning")
//            }
//        }

//        content: Column {
//            height: textLabel.height
//            width: parent.width - 20
//            anchors.horizontalCenter: parent.horizontalCenter
//            Label {
//                id: textLabel
//                width: parent.width
//                wrapMode: Text.WordWrap
//                color: "white"
//                text: qsTr("GetMeWheels is authorized for bookings but there is no account selected yet. Do you want to select an account now?")
//            }
//        }
//        buttons {
//            ButtonRow {
//                width: parent.width
//                Button {
//                    id: yesButton
//                    text: qsTr("Yes");
//                    onClicked: {
//                        accountCheckDialog.close();
//                        openAccountSelection();
//                    }
//                }
//                Button {
//                    id: noButton
//                    text: qsTr("No");
//                    onClicked: {
//                        accountCheckDialog.close();
//                        settingsSheet.done();
//                    }
//                }
//            }
//        }
//    }

/*    function openAccountSelection() {
        selectAccountDialog.model = undefined;
        accountModel.clear();
        gmwEngine.accountNames(locationButton.subTitleText).forEach(function(item) { accountModel.append({name: item}) } );
        selectAccountDialog.model = accountModel;
        selectAccountDialog.open();
    }
*/

/*    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        SectionHeader {
            width: parent.width
            headerText: qsTr("General")
        }

        Row {
            height: locationButton.height
            width: parent.width
            Label {
                text: qsTr("Location:")
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - locationButton.width
            }

            Button {
                id: locationButton
                text: gmwEngine.locationName
                width: units.gu(16)
                onClicked: {
                    locationsModel.clear();
                    gmwEngine.supportedLocationNames().forEach(function(item) { print("got location:" + item); locationsModel.append( {name: item} ) });
                    PopupUtils.open(locationsPopover, locationButton)
                }
            }
        }

        Component {
            id: locationsPopover

            Popover {
                id: popover
                callerMargin: units.gu(5)

                ListView {
                    clip: true
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                    }
                    model: locationsModel
                    height: units.gu(50)
                    delegate: ListItems.Standard {
                        icon: Qt.resolvedUrl("avatar_contacts_list.png")
                        text: modelData
                        onClicked: {
                            locationButton.text = modelData;
                            PopupUtils.close(popover);
                        }
                    }
                }
            }
        }

//        SelectionButton {
//            id: locationButton
//            titleText: qsTr("Location")
//            subTitleText: gmwEngine.locationName
//            width: parent.width
//            onClicked: {
//                selectLocationsDialog.model = undefined;
//                locationsModel.clear();
//                gmwEngine.supportedLocationNames().forEach(function(item) { print("got location:" + item); locationsModel.append( {name: item} ) });
//                selectLocationsDialog.model = locationsModel;
//                selectLocationsDialog.open();
//            }
//        }

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


//        SelectionButton {
//            id: accountSelectionButton
//            titleText: qsTr("Account")
//            subTitleText: gmwEngine.defaultAccountName
//            width: parent.width
//            enabled: gmwEngine.authenticated
//            onClicked: {
//                openAccountSelection();
//            }
//        }



    }

//    SelectionDialog {
//        id: selectLocationsDialog
//        titleText: qsTr("Select Location")
//        model: locationsModel
//        onAccepted: {
//            locationButton.subTitleText = locationsModel.get(selectLocationsDialog.selectedIndex).name;
//        }
//    }

//    ListModel {
//        id: accountModel
//    }
//    SelectionDialog {
//        id: selectAccountDialog
//        titleText: qsTr("Select Account")
//        onAccepted: {
//            print("selected: "+ accountModel.get(selectAccountDialog.selectedIndex).name);
//            gmwEngine.defaultAccountName = accountModel.get(selectAccountDialog.selectedIndex).name;
////            accountSelectionButton.subTitleText = gmwEngine.defaultAccountName();
//        }
//    }

    OAuthSetupSheet {
        id: oauthSetupSheet
    }
*/
//}
