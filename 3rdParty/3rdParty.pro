TEMPLATE = subdirs

symbian: {
SUBDIRS += kqoauth qjson qtm-geoservices-extras
}

contains(MEEGO_EDITION,harmattan) {
SUBDIRS += qoauth qtm-geoservices-extras
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
