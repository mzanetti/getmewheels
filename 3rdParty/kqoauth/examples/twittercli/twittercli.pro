TARGET = twittercli
TEMPLATE = app

# include(../../kqoauth.prf)

QT += network
CONFIG += crypto
CONFIG += kqoauth

macx {
    CONFIG -= app_bundle
    QMAKE_POST_LINK += install_name_tool -change kqoauth.framework/Versions/0/kqoauth \
                       ../../lib/kqoauth.framework/Versions/0/kqoauth $${TARGET}
}
else:unix {
  # the second argument (after colon) is for
  # being able to run make check from the root source directory
  LIBS += -Wl,-rpath,../../lib:lib -L/home/micha/Develop/kqoauth-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Release/lib/
  INCLUDEPATH+=/home/micha/Develop/kqoauth/src/
  INCLUDEPATH+=/home/micha/Develop/kqoauth/include/
}

#INCLUDEPATH += . ../../src
HEADERS += twittercli.h
SOURCES += twittercli.cpp
