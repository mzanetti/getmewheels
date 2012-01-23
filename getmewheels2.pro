# Add more folders to ship with the application, here
qmldir.source = qml/getmewheels2
qmldir.target = qml

datadir.source = engines/car2go/data

DEPLOYMENTFOLDERS = qmldir datadir

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE0BACF3E

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
CONFIG += mobility
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


INCLUDEPATH += /usr/include/QtCrypto

simulator: {
LIBS+= -lqoauth -lqjson -L$$PWD -lqca
} else {
LIBS+= -lqoauth -lqjson -lqtgeoservices_openstreetmap -L$$PWD -lqca
}

RESOURCES += \
    getmewheels2.qrc

