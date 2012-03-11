#-------------------------------------------------
#
# Project created by QtCreator 2011-04-21T21:39:27
#
#-------------------------------------------------

QT       -= gui

TARGET = gmw_qjson
TEMPLATE = lib

DEFINES += QJSON_LIBRARY QJSON_MAKEDLL

SOURCES += src/parser.cpp \
src/qobjecthelper.cpp \
src/json_scanner.cpp \
src/json_parser.cc \
src/parserrunnable.cpp \
src/serializer.cpp \
src/serializerrunnable.cpp

HEADERS += src/parserrunnable.h \
  src/serializerrunnable.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE55B574E
    TARGET.CAPABILITY = NetworkServices ReadDeviceData WriteDeviceData Location
#    TARGET.CAPABILITY += LocalServices Location NetworkServices ReadUserData UserEnvironment WriteUserData PowerMgmt ProtServ ReadDeviceData SurroundingsDD SwEvent TrustedUI WriteDeviceData
    TARGET.EPOCALLOWDLLDATA = 1
    #addFiles.sources = gmw_qjson.dll
    #addFiles.path = !:/sys/bin
    #DEPLOYMENT += addFiles

    # All files are needed for remote compiler
    HEADERS += src/qjson_export.h \
      src/qobjecthelper.h \
      src/parser.h \
      src/parser_p.h \
      src/json_parser.hh \
      src/stack.hh \
      src/json_scanner.h \
      src/qjson_debug.h \
      src/location.hh \
      src/position.hh \
      src/serializer.h

}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
