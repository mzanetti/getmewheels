#include <QGeoServiceProvider>
#include <QGeoMappingManager>
#include <QGraphicsSceneMouseEvent>
#include <QGeoCoordinate>
#include <QGeoMapPixmapObject>
#include <QGeoBoundingCircle>

#include "mapwidget.h"
#include "data/gmwitem.h"
#include "engines/car2go/car2goengine.h"
#include "core.h"

#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QPen>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGeoRoutingManager>
#include <QGeoRouteSegment>
#include <QGeoMapPolygonObject>

// A widget for QML, therefore we need the parameter-less constructor.
MapWidget::MapWidget() :
    QGraphicsGeoMap(Core::instance()->serviceProvider()->mappingManager()),
    m_tracking(false)
{
    addMapObject(&m_itemGroup);

    QPen pen(Qt::red);
    pen.setWidth(2);
    pen.setCosmetic(true);

//    m_positionMarker.setPen(pen);
//    QColor fill(QColor(255, 0, 0, 127));
//    m_positionMarker.setBrush(QBrush(fill));
//    m_positionMarker.setVisible(true);
//    m_positionMarker.setRadius(10);
    QPixmap ownPosition("/usr/share/themes/blanco/meegotouch/icons/icon-m-common-location-selected.png");
    m_positionMarker.setPixmap(ownPosition);
    m_positionMarker.setOffset(QPoint(-ownPosition.width() / 2, -ownPosition.height()));
    addMapObject(&m_positionMarker);
    m_positionMarker.setZValue(2);

    m_proxyModel = new GMWItemSortFilterProxyModel(this);

    // GPS
    m_gps = QGeoPositionInfoSource::createDefaultSource(this);
    qDebug() << "Got GPS:" << m_gps;
    if (m_gps) {
        m_gps->setUpdateInterval(5000);
        connect(m_gps, SIGNAL(positionUpdated(const QGeoPositionInfo&)), this, SLOT(positionUpdated(const QGeoPositionInfo&)));
        m_gps->startUpdates();
    }

    Core::instance()->serviceProvider()->routingManager()->setLocale(QLocale("en"));
    connect(Core::instance()->serviceProvider()->routingManager(), SIGNAL(finished(QGeoRouteReply*)), SLOT(routingFinished(QGeoRouteReply*)));
    connect(Core::instance()->serviceProvider()->routingManager(), SIGNAL(error(QGeoRouteReply*,QGeoRouteReply::Error,QString)), SLOT(routingError(QGeoRouteReply*,QGeoRouteReply::Error,QString)));

    pen.setColor(Qt::blue);
    pen.setWidth(5);
    m_routeObject.setPen(pen);
    m_routeObject.setZValue(2);
    addMapObject(&m_routeObject);
}

void MapWidget::positionUpdated(const QGeoPositionInfo &info) {
    qDebug() << "GPS Position updated:" << QDateTime::currentDateTime().toString();
    m_positionMarker.setCoordinate(info.coordinate());
//    if(info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy) && info.hasAttribute(QGeoPositionInfo::VerticalAccuracy)) {
//        m_positionMarker.setRadius(qMax(info.attribute(QGeoPositionInfo::HorizontalAccuracy), info.attribute(QGeoPositionInfo::VerticalAccuracy)));
//    }

    if(m_tracking) {
        animatedPanTo(info.coordinate());
    }

    if(m_model->engine()) {
        m_model->currentPositionChanged(info.coordinate(), info.attribute(QGeoPositionInfo::Direction));
    }
}

void MapWidget::setModel(GMWItemModel *model)
{
    m_model = model; //new GMWItemModel(this);
    m_proxyModel->setSourceModel(m_model);
    rowsInserted(QModelIndex(), 0, m_model->rowCount() - 1);

    connect(m_model, SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(rowsInserted(QModelIndex, int, int)));
    connect(m_model, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));
    connect(m_model, SIGNAL(locationChanged()), SLOT(centerToStartCoordinates()));

    QTimer::singleShot(2000, this, SLOT(centerToStartCoordinates()));

    if(m_model->engine()) {
        setBusinessArea();
    }
    connect(m_model, SIGNAL(engineChanged()), SLOT(mapEngineChanged()));

}

void MapWidget::mapEngineChanged()
{
    setBusinessArea();
    connect(m_model->engine(), SIGNAL(businessAreaChanged()), SLOT(setBusinessArea()));
}

void MapWidget::setBusinessArea()
{
    qDebug() << "Settings business area";
    // clean old ones
    removeMapObject(&m_businessAreaGroup);

    while(!m_businessAreaList.isEmpty()) {
        QGeoMapObject *obj = m_businessAreaList.takeFirst();
        m_businessAreaGroup.removeChildObject(obj);
        delete obj;
    }

    QPen pen(Qt::blue);
    pen.setWidth(0);
    QColor color(Qt::blue);
    color.setAlpha(20);
    QBrush brush(color);

    foreach(const Area &area, m_model->engine()->businessArea().areaList()) {
        QGeoMapPolygonObject *region = new QGeoMapPolygonObject();
        region->setBrush(brush);
        region->setPen(pen);

        region->setPath(area.path());
        m_businessAreaGroup.addChildObject(region);
        m_businessAreaList.append(region);
    }

    color = QColor(Qt::red);
    color.setAlpha(30);
    brush.setColor(color);

    foreach(const Area &area, m_model->engine()->businessArea().excludes()) {
        QGeoMapPolygonObject *region = new QGeoMapPolygonObject();
        region->setBrush(brush);
        region->setPen(pen);

        region->setPath(area.path());
        region->setZValue(0);
        m_businessAreaGroup.addChildObject(region);
        m_businessAreaList.append(region);
    }

    addMapObject(&m_businessAreaGroup);
    m_businessAreaGroup.setZValue(0);
}

void MapWidget::centerToStartCoordinates()
{
    fitInViewport(m_model->engine()->startingBounds());
    setZoomLevel(14);
}

GMWItemModel *MapWidget::model()
{
    return m_model;
}

bool MapWidget::tracking()
{
    return m_tracking;
}

void MapWidget::setTracking(bool tracking)
{
    m_tracking = tracking;
    if(tracking) {
        animatedPanTo(m_positionMarker.coordinate());
    }
}


// without this no mouseMoveEvent()'s are reported
void MapWidget::mousePressEvent(QGraphicsSceneMouseEvent*)
{
}

void MapWidget::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    m_tracking = false;
    QPointF dp(event->lastPos() - event->pos());
    pan(dp.x(), dp.y());
}

void MapWidget::rowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)

    qDebug() << "Inserting markers into map";
    removeMapObject(&m_itemGroup);

    for(int i = start; i <= end; ++i) {
        GMWItem *item = m_model->index(i, 0).data(Qt::UserRole).value<GMWItem*>();
        GMWMarker *marker = new GMWMarker(item);
        m_items.insert(item, marker);
        m_itemGroup.addChildObject(marker);
    }
    addMapObject(&m_itemGroup);
}

void MapWidget::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)

    qDebug() << "Removing markers from map" << start << "-" << end << "Current markers:" << m_items.count() << "Markers to remove:" << end-start+1;
    for(int i = start; i <= end; ++i) {
        QModelIndex index = m_model->index(i, 0);
        GMWItem *item = m_model->data(index, Qt::UserRole).value<GMWItem*>();
        QGeoMapPixmapObject *marker = m_items.value(item);
        m_itemGroup.removeChildObject(marker);
        m_items.remove(item);
        delete marker;
    }

//    qDebug() << "items removed; Items:" << m_items.count();
}

void MapWidget::animatedPanTo(GMWItem *item)
{
    animatedPanTo(item->location());
}

void MapWidget::animatedPanTo(const QGeoCoordinate &center)
{
    qDebug() << "panning to" << center;
    if(!center.isValid()) return;

    QPropertyAnimation *latAnim = new QPropertyAnimation(this, "centerLatitude");
    latAnim->setEndValue(center.latitude());
    latAnim->setDuration(200);
    QPropertyAnimation *lonAnim = new QPropertyAnimation(this, "centerLongitude");
    lonAnim->setEndValue(center.longitude());
    lonAnim->setDuration(200);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(latAnim);
    group->addAnimation(lonAnim);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

double MapWidget::centerLatitude() const
{
    return center().latitude();
}

double MapWidget::centerLongitude() const
{
    return center().longitude();
}

void MapWidget::setCenterLatitude(double lat)
{
    QGeoCoordinate c = center();
    c.setLatitude(lat);
    setCenter(c);
}

void MapWidget::setCenterLongitude(double lon)
{
    QGeoCoordinate c = center();
    c.setLongitude(lon);
    setCenter(c);
}

void MapWidget::clicked(qreal mouseX, qreal mouseY)
{
    qDebug() << "clicked at" << mouseX << mouseY;

    foreach(GMWMarker*marker, m_items) {
        marker->setHighlight(false);
    }

    // walk in reverse order to pick the topmost item
    for(int i = mapObjectsInViewport().count() - 1; i >= 0; --i) {
        QGeoMapObject *obj = mapObjectsInViewport().at(i);
//        qDebug() << "object in screen" << coordinateToScreenPosition(obj->boundingBox().topLeft()) << coordinateToScreenPosition(obj->boundingBox().bottomRight());
        QPointF topLeft = coordinateToScreenPosition(obj->boundingBox().topLeft());
        QPointF bottomRight = coordinateToScreenPosition(obj->boundingBox().bottomRight());
        if(topLeft.x() <= mouseX && topLeft.y() <= mouseY && bottomRight.x() >= mouseX && bottomRight.y() >= mouseY) {
            GMWMarker *po = qobject_cast<GMWMarker*>(obj);
            if(po) {
                po->setHighlight(true);
                emit itemClicked(m_items.key(po));
                return;
            }
        }
    }
}

void MapWidget::routeTo(GMWItem *item)
{
    if(!Core::instance()->serviceProvider()->routingManager()) {
        qDebug() << "Routing not supported by map provider";
        return;
    }
    qDebug() << "routing from" << m_positionMarker.coordinate() << "to" << item->location();

    QGeoRouteRequest request;
    request.setTravelModes(QGeoRouteRequest::PedestrianTravel);
    request.setRouteOptimization(QGeoRouteRequest::ShortestRoute);
    request.setWaypoints(QList<QGeoCoordinate>() << m_positionMarker.coordinate() << item->location());

    Core::instance()->serviceProvider()->routingManager()->calculateRoute(request);
}

void MapWidget::routingFinished(QGeoRouteReply *reply)
{
    qDebug() << "Routing finished. Found" << reply->routes().count() << "routes";
    if(reply->routes().count() <= 0) {
        reply->deleteLater();
        return;
    }

    qDebug() << "Nodes:" << reply->routes().first().path().count();
    qDebug() << "Distance:" << reply->routes().first().distance();
    qDebug() << reply->routes().first().routeId();
    qDebug() << reply->routes().first().travelMode();
    qDebug() << reply->routes().first().travelTime();
    qDebug() << reply->routes().first().firstRouteSegment().path();

    removeMapObject(&m_routeObject);
    m_routeObject.setRoute(reply->routes().first());
    addMapObject(&m_routeObject);

    reply->deleteLater();
}

void MapWidget::routingError(QGeoRouteReply *reply, QGeoRouteReply::Error error, const QString &errorString)
{
    Q_UNUSED(error);
    qDebug() << "Routing failed:" << errorString;
    reply->deleteLater();
}
