/*****************************************************************************
 * Copyright: 2012 Michael Zanetti <michael_zanetti@gmx.net>                 *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "core.h"
#include "engines/car2go/car2goengine.h"
#include "settings.h"

#if QT_VERSION < 0x050000
  #include "mapwidget.h"
  #include <QApplication>
  #include <QtDeclarative>
  typedef QApplication QGuiApplication;
#else
  #include <QGuiApplication>
  #include <QtQml/qqml.h>
  #include <QDir>
#endif

#include "qmlapplicationviewer.h"
#include <QDebug>
#include <QTranslator>
#include <QLibraryInfo>


#if !defined QT_SIMULATOR && QT_VERSION < 0x050000
Q_IMPORT_PLUGIN(qtgeoservices_osm)
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QString language = QLocale::system().name().split('_').first();
    qDebug() << "got languange" << language;

    QScopedPointer<QGuiApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    QTranslator qtTranslator;
    if(!qtTranslator.load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        qDebug() << "couldn't load qt_" + language;
    }
    app->installTranslator(&qtTranslator);

    QTranslator translator;
    if (!translator.load(":/i18n/getmewheels_" + language + ".qm")) {
        qDebug() << "Cannot load translation file" << "getmewheels_" + language + ".pm";
    }
    app->installTranslator(&translator);


#if QT_VERSION < 0x050000
    qmlRegisterType<MapWidget>("GetMeWheels", 1, 0, "GmwMap");
#endif
    qmlRegisterType<GMWItemModel>("GetMeWheels", 1, 0, "GmwModel");
    qmlRegisterType<GMWItemSortFilterProxyModel>("GetMeWheels", 1, 0, "GmwProxyModel");
    qmlRegisterType<GMWItem>();
    qmlRegisterType<Location>();
    qmlRegisterUncreatableType<GMWItem>("GetMeWheels", 1, 0, "GmwItem", "Cannot create items of type GmwItem");
    qmlRegisterUncreatableType<GMWVehicle>("GetMeWheels", 1, 0, "GmwVehicle", "Cannot create items of type GmwVehicle");
    qmlRegisterType<GMWBooking>();

    qmlRegisterType<GMWEngine>("GetMeWheels", 1, 0, "GmwEngine");

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
#if defined Q_WS_S60
    viewer->setMainQmlFile(QLatin1String("qml/symbian/main.qml"));
#elif defined MEEGO
    viewer->setMainQmlFile(QLatin1String("/opt/getmewheels2/qml/harmattan/main.qml"));
#else
    QString prefix;
    if(QCoreApplication::applicationDirPath() == QDir(("/usr/bin")).canonicalPath()) {
        prefix = "/usr";
    } else {
        prefix = ".";
    }
    viewer->setSource(QUrl::fromLocalFile(prefix + "/share/getmewheels2/qml/ubuntu/main.qml"));
    viewer->resize(720, 1280);
#endif
    viewer->showExpanded();

    return app->exec();
}
