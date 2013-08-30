import QtQuick 2.0
import QtWebKit 3.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1

DefaultSheet {
    id: root
    Component.onCompleted: {
        gmwEngine.startAuthentication();
    }

    signal done()

    Connections {
        target: gmwEngine
        onAuthenticateUrl: {
            print("foooooooooooooooooooo", url)
            webView.url = url
            webView.visible = true;
        }
    }
    WebView {
        id: webView
        anchors.fill: parent
        visible: false

        property string callbackUrl: "http://notyetthere.org/getmewheels/oauth_callback"

        onUrlChanged: {
            print("url changed *******", url)
            var urlString = url.toString();
            if (urlString.substring(0, callbackUrl.length) == callbackUrl) {
                var params = urlString.split('&');
                params = params[params.length-1].split('=');
                var verifier = params[params.length-1];
                print("setting verifier", verifier);
                gmwEngine.setAccessCode(verifier);
                root.done();
                PopupUtils.close(root);
            }
        }
    }

}
