#TARGET = getmewheels2
#datadir.source = engines/car2go/data
#DEPLOYMENTFOLDERS = qmldir datadir

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
CONFIG += mobility
MOBILITY += location

QT += network

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    mapwidget.cpp \
    gmwmarker.cpp \
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
    mapwidget.h \
    gmwmarker.h \
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

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog


contains(MEEGO_EDITION,harmattan) {
    INCLUDEPATH += $$PWD/3rdParty/qoauth/include
    LIBS += -L$$PWD/3rdParty/qoauth/lib/ -L$$PWD/3rdParty/qtm-geoservices-extras/build/ -lqca -lqoauth -lqjson -lqtgeoservices_osm

    qmldir.source = qml/getmewheels2/harmattan
    qmldir.target = qml
    DEPLOYMENTFOLDERS = qmldir
}

simulator: {
    LIBS+= -lqoauth -lqjson -L$$PWD -lqca
}

maemo5: {
    qmldir.source = qml/getmewheels2
    qmldir.target = qml
    DEPLOYMENTFOLDERS = qmldir
}

symbian: {
    CONFIG += qt-components

    TARGET.UID3 = 0xE0BACF3E

    # Allow network access on Symbian
    TARGET.CAPABILITY += NetworkServices ReadDeviceData WriteDeviceData Location

    # In case of Symbian we use our 3rdParty distribution of qjson
    INCLUDEPATH += $$PWD/3rdParty/
    LIBS += -lgmw_qjson
    jsonFiles.sources = gmw_qjson.dll
    jsonFiles.path = /sys/bin
    DEPLOYMENT += jsonFiles

    LIBS += -lgmw_kqoauth
    oauthFiles.sources = gmw_kqoauth.dll
    oauthFiles.path = /sys/bin
    DEPLOYMENT += oauthFiles

    qmldir.source = qml/getmewheels2/symbian
    qmldir.target = qml
    DEPLOYMENTFOLDERS = qmldir

}

RESOURCES += \
    getmewheels2.qrc

splash.files = splash.png
splash.path = /opt/$${TARGET}
INSTALLS += splash

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
