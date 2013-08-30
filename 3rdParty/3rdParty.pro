TEMPLATE = subdirs

symbian: {
SUBDIRS += kqoauth qjson qtm-geoservices-extras
}

contains(MEEGO_EDITION,harmattan) {
SUBDIRS += qoauth qtm-geoservices-extras
}

contains(QT_VERSION, ^5\\..\\..*) {
SUBDIRS += kqoauth
}
