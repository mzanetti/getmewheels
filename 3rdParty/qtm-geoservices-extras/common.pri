include (config.pri)

target.path=$${QT_MOBILITY_PLUGINS}/$${PLUGIN_TYPE}
#INSTALLS += target

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
#    INSTALLS += target
}
