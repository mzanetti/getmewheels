TEMPLATE = lib
CONFIG += plugin

    CONFIG += static
contains(MEEGO_EDITION,harmattan) {
}

TARGET = $$qtLibraryTarget(qtgeoservices_osm)
symbian {
TARGET = gmw_$$qtLibraryTarget(qtgeoservices_osm)
}

PLUGIN_TYPE=geoservices

include(../common.pri)

QT += network

CONFIG += mobility
MOBILITY = location

HEADERS += \
            qgeomappingmanagerengine_osm.h \
            qgeomapreply_osm.h \
            qgeoserviceproviderplugin_osm.h \
            qgeocodeparser.h \
            qgeosearchreply_osm.h \
            qgeosearchmanagerengine_osm.h \
            parseproxy.h \
            qgeoroutingmanagerengine_osm.h \
            qgeoroutereply_osm.h \
            routeparser.h

SOURCES += \
            qgeomappingmanagerengine_osm.cpp \
            qgeomapreply_osm.cpp \
            qgeoserviceproviderplugin_osm.cpp \
            qgeocodeparser.cpp \
            qgeosearchreply_osm.cpp \
            qgeosearchmanagerengine_osm.cpp \
            qgeoroutingmanagerengine_osm.cpp \
            qgeoroutereply_osm.cpp \
            routeparser.cpp

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.EPOCALLOWDLLDATA = 1
    #TARGET.CAPABILITY = ALL -TCB
    TARGET.CAPABILITY += NetworkServices ReadDeviceData WriteDeviceData Location
    pluginDep.sources = $${TARGET}.dll
    pluginDep.path = $${QT_PLUGINS_BASE_DIR}/$${PLUGIN_TYPE}
    DEPLOYMENT += pluginDep
}

