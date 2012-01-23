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

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "data/gmwitemmodel.h"
#include "data/gmwitemsortfilterproxymodel.h"
#include "gmwmarker.h"
#include "engines/gmwengine.h"

#include <QGraphicsGeoMap>
#include <QGeoMapGroupObject>
#include <QGeoMapCircleObject>
#include <QGeoMapPixmapObject>
#include <QGeoPositionInfoSource>
#include <QModelIndex>
#include <QGeoRouteReply>
#include <QGeoMapRouteObject>
#include <QGeoServiceProvider>

QTM_USE_NAMESPACE

class MapWidget : public QGraphicsGeoMap
{
    Q_OBJECT

    Q_PROPERTY(double centerLatitude READ centerLatitude WRITE setCenterLatitude)
    Q_PROPERTY(double centerLongitude READ centerLongitude WRITE setCenterLongitude)

    Q_PROPERTY(GMWItemModel* model READ model WRITE setModel)
    Q_PROPERTY(bool tracking READ tracking WRITE setTracking)
//    Q_PROPERTY(QString location READ location WRITE setLocation)
//    Q_PROPERTY(QStringList supportedLocations READ supportedLocations)

public:
    MapWidget();

    GMWItemModel *model();
    void setModel(GMWItemModel *model);

    Q_INVOKABLE void fitInViewport(const QGeoBoundingBox &bounds, bool preserveViewportCenter = false) { QGraphicsGeoMap::fitInViewport(bounds, preserveViewportCenter); }
    Q_INVOKABLE void animatedPanTo(const QGeoCoordinate &coordinage);
    Q_INVOKABLE void animatedPanTo(GMWItem *item);
    Q_INVOKABLE void clicked(qreal mouseX, qreal mouseY);

    bool tracking();
    void setTracking(bool tracking);

    double centerLatitude() const;
    void setCenterLatitude(double lat);
    double centerLongitude() const;
    void setCenterLongitude(double lon);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

public slots:
    void centerToStartCoordinates();
    void routeTo(GMWItem *item);

signals:
    void itemClicked(GMWItem* item);
    void itemsClicked(const QVariant &items);
    void routingFailed(const QString &error);

private slots:
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

    void positionUpdated(const QGeoPositionInfo &info);

    void routingFinished(QGeoRouteReply *reply);
    void routingError(QGeoRouteReply *reply, QGeoRouteReply::Error error, const QString &errorString);

    void mapEngineChanged();
    void setBusinessArea();

    void updateMapItems();

private:
    GMWItemModel *m_model;
    GMWItemSortFilterProxyModel *m_proxyModel;

    GMWEngine *m_engine;

    QGeoMapGroupObject m_itemGroup;
    QMap<GMWItem*, GMWMarker*>  m_items;

    QGeoPositionInfoSource *m_gps;
    QGeoMapPixmapObject m_positionMarker;
    QGeoMapCircleObject m_accuracyMarker;

    bool m_tracking;

    QGeoMapRouteObject          m_routeObject;

    QList<QGeoMapObject*>       m_businessAreaList;
    QGeoMapGroupObject          m_businessAreaGroup;

};

#endif // MAPWIDGET_H
