import QtQuick 1.1
import QtWebKit 1.0
import com.nokia.meego 1.0

Sheet {
    id: settingsSheet
    rejectButtonText: qsTr("Cancel")

    property alias url: webView.url

    content: WebView {
        id: webView
        anchors.fill: parent
    }
}
