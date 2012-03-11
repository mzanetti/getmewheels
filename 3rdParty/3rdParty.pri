TEMPLATE = subdirs

symbian: {
SUBDIRS += kqoauth qjson
}

contains(MEEGO_EDITION,harmattan) {
SUBDIRS += qoauth qtm-geoservices-extras
}
