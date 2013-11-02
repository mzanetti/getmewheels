# General project config
TARGET = getmewheels2

CONFIG += mobility
MOBILITY += location

contains(QT_VERSION, ^5\\..\\..*) {
  QT += gui quick qml network location xml
} else {
  QT += declarative network
}


# Files used on all platforms

SOURCES += main.cpp \
    core.cpp \
    data/gmwitemmodel.cpp \
    data/gmwitem.cpp \
    data/gmwvehicle.cpp \
    data/gmwparkingspot.cpp \
    data/gmwgasstation.cpp \
    data/gmwaccount.cpp \
    data/gmwbooking.cpp \
    data/gmwitemsortfilterproxymodel.cpp \
    data/gmwbusinessarea.cpp \
    engines/gmwengine.cpp \
    engines/car2go/car2goengine.cpp \
    settings.cpp \
    engines/location.cpp \
    engines/engineplugin.cpp


HEADERS += \
    core.h \
    data/gmwitemmodel.h \
    data/gmwitem.h \
    data/gmwvehicle.h \
    data/gmwparkingspot.h \
    data/gmwgasstation.h \
    data/gmwaccount.h \
    data/gmwbooking.h \
    data/gmwitemsortfilterproxymodel.h \
    data/gmwbusinessarea.h \
    engines/gmwengine.h \
    engines/car2go/car2goengine.h \
    settings.h \
    engines/location.h \
    engines/engineplugin.h

!contains(QT_VERSION, ^5\\..\\..*) {

SOURCES += mapwidget.cpp \
    gmwmarker.cpp \

HEADERS += \
    mapwidget.h \
    gmwmarker.h \
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

RESOURCES += \
    getmewheels2.qrc

# translations
#translations.files = i18n/getmewheels_de.qm

#translations.path = .
#INSTALLS += translations

# MeeGo specific stuff
contains(MEEGO_EDITION,harmattan) {
    # Speed up launching on MeeGo/Harmattan when using applauncherd daemon
    CONFIG += qdeclarative-boostable
    DEFINES += MEEGO

    INCLUDEPATH += $$PWD/../3rdParty/qoauth/include
    LIBS += -L$$OUT_PWD/../3rdParty/qoauth/lib/ -L$$OUT_PWD/../3rdParty/qtm-geoservices-extras/openstreetmap/ -lqca -lqoauth -lqjson -lqtgeoservices_osm

    qmldir.source = qml/getmewheels2/harmattan
    qmldir.target = qml
    DEPLOYMENTFOLDERS = qmldir

    splash.files = splash.png
    splash.path = /opt/$${TARGET}
    INSTALLS += splash

    linguist {
        SOURCES += \
            qml/getmewheels2/harmattan/ItemDetailsSheet.qml \
            qml/getmewheels2/harmattan/ItemList.qml \
            qml/getmewheels2/harmattan/MainPage.qml \
            qml/getmewheels2/harmattan/main.qml \
            qml/getmewheels2/harmattan/OAuthSetupSheet.qml \
            qml/getmewheels2/harmattan/SectionHeader.qml \
            qml/getmewheels2/harmattan/SelectionButton.qml \
            qml/getmewheels2/harmattan/SettingsSheet.qml \
            qml/getmewheels2/harmattan/ZoomSlider.qml \
    }

}

# Simulator specific stuff
simulator: {
    INCLUDEPATH += $$PWD/../3rdParty/qoauth/include
    LIBS+= -lqoauth -lqjson -L$$PWD -lqca

    qmldir.source = qml/getmewheels2/harmattan
    qmldir.target = qml
    DEPLOYMENTFOLDERS = qmldir
}

# Symbian specific stuff
symbian: {
    DEPLOYMENT.display_name = GetMeWheels

    # Dependency to qt components for regular sis
    CONFIG += qt-components qt
    CONFIG += qtquickcomponents
    # and for smart installer
    DEPLOYMENT.installer_header = 0x2002CCCF

    installrules.pkg_prerules += \
        "; Dependency to Symbian Qt Quick components" \
        "(0x200346DE), 1, 1, 0, {\"Qt Quick components\"}"
    DEPLOYMENT += installrules

    #TARGET.UID3 = 0xE0BACF3E
    TARGET.UID3 = 0x20062245
    # Allow network access on Symbian
    TARGET.CAPABILITY += NetworkServices Location #ReadDeviceData #WriteDeviceData

    # In case of Symbian we use our 3rdParty distribution of qjson
    INCLUDEPATH += $$PWD/3rdParty/
    LIBS += -lgmw_qjson.lib
#    jsonFiles.sources = gmw_qjson.dll
#    jsonFiles.path = /sys/bin
#    DEPLOYMENT += jsonFiles

    LIBS += -lgmw_kqoauth.lib
#    oauthFiles.sources = gmw_kqoauth.dll
#    oauthFiles.path = /sys/bin
#    DEPLOYMENT += oauthFiles

    LIBS += -lgmw_qtgeoservices_osm.lib
#    osmFiles.sources = gmw_qtgeoservices_osm.dll
#    osmFiles.path = /sys/bin
#    DEPLOYMENT += osmFiles

    qmldir.source = qml/getmewheels2/symbian
    qmldir.target = qml
    DEPLOYMENTFOLDERS = qmldir

    my_deployment.pkg_prerules += vendorinfo

    DEPLOYMENT += my_deployment

    vendorinfo += "%{\"Michael Zanetti\"}" ":\"Michael Zanetti\""

    linguist {
        SOURCES += \
            qml/getmewheels2/symbian/ItemDetailsSheet.qml \
            qml/getmewheels2/symbian/ItemList.qml \
            qml/getmewheels2/symbian/MainPage.qml \
            qml/getmewheels2/symbian/main.qml \
            qml/getmewheels2/symbian/OAuthSetupSheet.qml \
            qml/getmewheels2/symbian/SectionHeader.qml \
            qml/getmewheels2/symbian/SelectionButton.qml \
            qml/getmewheels2/symbian/SettingsSheet.qml \
            qml/getmewheels2/symbian/ZoomSlider.qml \
    }
}

contains(QT_VERSION, ^5\\..\\..*) {
    LIBS += -L$$OUT_PWD/../3rdParty/kqoauth/lib/ -lgmw_kqoauth

    folder_01.source = qml/getmewheels2/ubuntu
    folder_01.target = qml
    DEPLOYMENTFOLDERS = folder_01

    linguist {
        SOURCES += \
            qml/getmewheels2/ubuntu/ItemDetailsSheet.qml \
            qml/getmewheels2/ubuntu/ItemList.qml \
            qml/getmewheels2/ubuntu/MainPage.qml \
            qml/getmewheels2/ubuntu/main.qml \
            qml/getmewheels2/ubuntu/OAuthSetupSheet.qml \
            qml/getmewheels2/ubuntu/SectionHeader.qml \
            qml/getmewheels2/ubuntu/SelectionButton.qml \
            qml/getmewheels2/ubuntu/SettingsSheet.qml \
            qml/getmewheels2/ubuntu/ZoomSlider.qml
    }

    desktopfile.files = $${TARGET}_ubuntu.desktop
    desktopfile.path = /usr/share/applications
}

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
