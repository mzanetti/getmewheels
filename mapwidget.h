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

private slots:
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

    void positionUpdated(const QGeoPositionInfo &info);

    void routingFinished(QGeoRouteReply *reply);
    void routingError(QGeoRouteReply *reply, QGeoRouteReply::Error error, const QString &errorString);

    void mapEngineChanged();
    void setBusinessArea();

private:
    GMWItemModel *m_model;
    GMWItemSortFilterProxyModel *m_proxyModel;

    GMWEngine *m_engine;

    QGeoMapGroupObject m_itemGroup;
    QMap<GMWItem*, GMWMarker*>  m_items;

    QGeoPositionInfoSource *m_gps;
    QGeoMapPixmapObject m_positionMarker;

    bool m_tracking;

    QGeoMapRouteObject          m_routeObject;

    QList<QGeoMapObject*>       m_businessAreaList;
    QGeoMapGroupObject          m_businessAreaGroup;

};

#endif // MAPWIDGET_H
