# Add more folders to ship with the application, here
contains(MEEGO_EDITION,harmattan) {
message("Including MeeGoo QML")
qmldir.source = qml/getmewheels2
}
symbian: {
message("Including Symbian QML")
qmldir.source = qml/getmewheels2/symbian
}
qmldir.target = qml

datadir.source = engines/car2go/data

DEPLOYMENTFOLDERS = qmldir datadir

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
CONFIG += mobility qt-components
MOBILITY += location

QT += network

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

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

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
#include(maps/maps.pri)

qtcAddDeployment()

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


simulator: {
INCLUDEPATH += /usr/include/QtCrypto
LIBS+= -lqoauth -lqjson -L$$PWD -lqca
}

contains(MEEGO_EDITION,harmattan) {
message("adding meego includes")
INCLUDEPATH += /usr/include/QtCrypto/
LIBS+= -lqoauth -lqjson -lqtgeoservices_openstreetmap -L$$PWD -lqca
}

symbian:{
    TARGET.UID3 = 0xE0BACF3E

    SOURCES += oauth/hash.c oauth/oauth.c oauth/xmalloc.c

    # Smart Installer package's UID
    # This UID is from the protected range and therefore the package will
    # fail to install if self-signed. By default qmake uses the unprotected
    # range value if unprotected UID is defined for the application and
    # 0x2002CCCF value if protected UID is given to the application
    #symbian:DEPLOYMENT.installer_header = 0x2002CCCF

    # Allow network access on Symbian
    TARGET.CAPABILITY += LocalServices Location NetworkServices ReadUserData UserEnvironment WriteUserData PowerMgmt ProtServ ReadDeviceData SurroundingsDD SwEvent TrustedUI WriteDeviceData

    LIBS+= -lgmw_qjson -L$$PWD -llibcrypto

    # You need to compile libqjson and deploy it to Symbian sysroot as
    # libgmw_qjson.dll on your own for now as libs need to have the
    # same capabilites (or more) than the linking binary.
    # Don't forget to edit the capabilities there to the same as here.
    # This will add the lib from the sysroot to the package.
    addFiles.sources = gmw_qjson.dll
    addFiles.path = /sys/bin
    DEPLOYMENT += addFiles

}

RESOURCES += \
    getmewheels2.qrc

splash.files = splash.png
splash.path = /opt/$${TARGET}
INSTALLS += splash

symbian {
INCLUDEPATH += ../qoauth/
}
