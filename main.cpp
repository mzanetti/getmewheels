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

#include "mapwidget.h"
#include "core.h"
#include "engines/car2go/car2goengine.h"
#include "settings.h"

#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <QtDeclarative>

#ifndef QT_SIMULATOR
Q_IMPORT_PLUGIN(qtgeoservices_osm)
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    qmlRegisterType<MapWidget>("GetMeWheels", 1, 0, "GmwMap");
    qmlRegisterType<GMWItemModel>("GetMeWheels", 1, 0, "GmwModel");
    qmlRegisterType<GMWItemSortFilterProxyModel>("GetMeWheels", 1, 0, "GmwProxyModel");
    qmlRegisterType<GMWItem>();
    qmlRegisterType<Location>();
    qmlRegisterUncreatableType<GMWItem>("GetMeWheels", 1, 0, "GmwItem", "Cannot create items of type GmwItem");
    qmlRegisterUncreatableType<GMWVehicle>("GetMeWheels", 1, 0, "GmwVehicle", "Cannot create items of type GmwVehicle");
    qmlRegisterType<GMWBooking>();

    qmlRegisterType<GMWEngine>("GetMeWheels", 1, 0, "GmwEngine");

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/getmewheels2/main.qml"));
    viewer->showExpanded();

    return app->exec();
}
