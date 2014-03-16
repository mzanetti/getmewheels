import QtQuick 2.0
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components 0.1
import QtLocation 5.0
import QtPositioning 5.2
import GetMeWheels 1.0

MainView {
    id: appWindow

    headerColor: "#0249b3"
    backgroundColor: "#0365bf"
    footerColor: "#06bce4"

    property bool showOnlyBookedCars: false

    ToolbarItems {
        id: mainToolbar
        ToolbarButton {
            iconSource: "/usr/share/icons/ubuntu-mobile/actions/scalable/settings.svg"
            text: "Settings"
            onTriggered: {
                PopupUtils.open(Qt.resolvedUrl("SettingsSheet.qml"), appWindow);
            }
        }
        ToolbarButton {
            iconSource: "images/info.svg"
            text: qsTr("About")
            onTriggered: {
                PopupUtils.open(aboutDialogComponent, appWindow)
            }
        }
        ToolbarButton {
            iconSource: "/usr/share/icons/ubuntu-mobile/actions/scalable/reload.svg"
            text: qsTr("Roload")
            onTriggered: {
                gmwModel.clearVehicles();
                gmwEngine.refreshVehicles(false);
            }
        }
        ToolbarButton {
            iconSource: "/usr/share/icons/ubuntu-mobile/actions/scalable/favorite-" + (showOnlyBookedCars ? "" : "un") + "selected.svg"
            text: showOnlyBookedCars ? qsTr("All") : qsTr("Booked")
            onTriggered: {
                showOnlyBookedCars = !showOnlyBookedCars
            }
        }
    }

    Tabs {
        anchors.fill: parent
        visible: gmwEngine.locationName.length > 0

        Tab {
            title: "City Map"
            id: mapTab
            page: MainPage {
                id: mainPage
            }
        }

        Tab {
            title: "Vehicle List"
            page: ItemList {
                model: gmwModel
                onlyBooked: showOnlyBookedCars
            }
        }
    }

    GmwEngine {
        id: gmwEngine

        onLocationNameChanged: print("location changed to", locationName)

    }

    GmwModel {
        id: gmwModel

        engine: gmwEngine
    }

    Column {
        anchors.centerIn: parent
        width: parent.width - units.gu(6)
        visible: gmwEngine.locationName.length == 0
        spacing: units.gu(2)
        UbuntuShape {
            width: units.gu(10)
            height: units.gu(10)
            anchors.horizontalCenter: parent.horizontalCenter
            radius: "medium"
            image: Image {
                source: "images/car_blue.svg"
                smooth: true
            }
        }

        Label {
            anchors {
                left: parent.left
                right: parent.right
            }

            wrapMode: Text.WordWrap
            text: qsTr("Welcome to GetMeWheels. A car finder application for car2go.")
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            anchors {
                left: parent.left
                right: parent.right
            }

            wrapMode: Text.WordWrap
            text: qsTr("Please select a location for which you would like to browse available cars.")
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            anchors {
                left: parent.left
                right: parent.right
            }

            wrapMode: Text.WordWrap
            text: qsTr("If you want to create bookings for cars, additionally you need to sign in with your car2go account.")
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            text: qsTr("Open settings")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: PopupUtils.open(Qt.resolvedUrl("SettingsSheet.qml"), appWindow)
        }
    }

    PositionSource {
        id: positionSource
        updateInterval: 10000
        active: true
        property bool gotPosition: false
        onPositionChanged: {
            gotPosition = true
            print("Bug tvoss as long as this show up more often than every 10 seconds.")
        }
    }

    // Workaround for positionSource updateInterval not working yet
    Timer {
        interval: 10000
        running: positionSource.active && positionSource.gotPosition
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            var coord = positionSource.position.coordinate;
            gmwModel.currentPositionChanged(coord)
            console.log("updating position:", coord.longitude, coord.latitude);
            mainPage.positionCoordinate = coord;
        }
    }


    Component {
        id: aboutDialogComponent

        Dialog {
            id: aboutDialog
            title: "GetMeWheels 1.2"
            text: "Michael Zanetti\nmichael_zanetti@gmx.net\nChristian Fetzer\nfetzerch@googlemail.com"

            Item {
                width: parent.width
                height: units.gu(40)
                Column {
                    id: contentColumn
                    anchors.fill: parent
                    spacing: units.gu(1)

                    UbuntuShape {
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: units.gu(6)
                        width: units.gu(6)
                        radius: "medium"
                        image: Image {
                            source: "images/car_blue.svg"
                        }
                    }

                    Flickable {
                        width: parent.width
                        height: parent.height - y - (closeButton.height + parent.spacing) * 3
                        contentHeight: gplLabel.implicitHeight
                        clip: true
                        Label {
                            id: gplLabel
                            width: parent.width
                            wrapMode: Text.WordWrap
                            text: "This program is free software: you can redistribute it and/or modify " +
                                  "it under the terms of the GNU General Public License as published by " +
                                  "the Free Software Foundation, either version 3 of the License, or " +
                                  "(at your option) any later version.\n\n" +

                                  "This program is distributed in the hope that it will be useful, " +
                                  "but WITHOUT ANY WARRANTY; without even the implied warranty of " +
                                  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the " +
                                  "GNU General Public License for more details.\n\n" +

                                  "You should have received a copy of the GNU General Public License " +
                                  "along with this program.  If not, see http://www.gnu.org/licenses/."
                        }
                    }
                    Button {
                        id: closeButton
                        width: parent.width
                        text: qsTr("Close")
                        onClicked: PopupUtils.close(aboutDialog)
                    }
                    Button {
                        id: donateButton
                        width: parent.width
                        text: "donate";
                        onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=UPA7TFA73GKFY")
                    }
                    Button {
                        text: "flattr";
                        width: parent.width
                        onClicked: Qt.openUrlExternally("https://flattr.com/thing/465960/GetMeWheels")
                    }
                }

            }

        }
    }
}
