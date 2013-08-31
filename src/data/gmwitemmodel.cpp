/*****************************************************************************
 * Copyright: 2010-2011 Christian Fetzer <fetzer.ch@googlemail.com>          *
 * Copyright: 2010-2012 Michael Zanetti <michael_zanetti@gmx.net>            *
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

#include "gmwitemmodel.h"
#include "gmwitem.h"
#include "gmwvehicle.h"
#include "gmwparkingspot.h"

#include <QDebug>
#include <QPaintEngine>
#include <QRectF>

GMWItemModel::GMWItemModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_engine(0)
{
    qRegisterMetaType<GMWItem*>("GMWObject*");

    QHash<int, QByteArray> roleNames;
    roleNames.insert(RoleObject, "gmwitem");
    roleNames.insert(RoleName, "itemName");
    roleNames.insert(RoleAddress, "itemAddress");
    roleNames.insert(RoleDistance, "itemDistance");
    roleNames.insert(RoleDistanceString, "itemDistanceString");
    roleNames.insert(RoleType, "itemType");
    roleNames.insert(RoleEngineType, "itemEngineType");
    roleNames.insert(RoleParkingCP, "itemParkingCP");
#if QT_VERSION < 0x050000
    setRoleNames(roleNames);
#else
    m_roleNames = roleNames;
#endif
}

GMWEngine *GMWItemModel::engine()
{
    return m_engine;
}

void GMWItemModel::setEngine(GMWEngine *engine)
{
    qDebug() << "setting engine:" << engine;
    m_engine = engine;

    QObject::connect(m_engine, SIGNAL(objectsReceived(QList<GMWItem*>)), SLOT(addObjects(QList<GMWItem*>)));
    QObject::connect(m_engine, SIGNAL(locationChanged()), SIGNAL(locationChanged()));

    m_engine->refreshStationary(true);
    m_engine->refreshVehicles(true);

    emit engineChanged();
}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> GMWItemModel::roleNames() const
{
    return m_roleNames;
}
#endif

void GMWItemModel::addObject(GMWItem *item) {
    addObjects(QList<GMWItem*>() << item);
}

void GMWItemModel::addObjects(QList<GMWItem*> items) {
    qDebug() << "Adding " << items.count() << " items to model; Current size is" << m_objects.count();
    beginInsertRows(QModelIndex(), m_objects.size(), m_objects.size() + items.count() - 1);
    int i = 0;
    foreach(GMWItem* item, items) {
//        qDebug() << "Adding item" << i++ << item->name();
        connect(item, SIGNAL(changed()), SLOT(itemChanged()));
        m_objects.append(item);
    }
    endInsertRows();
    qDebug() << "Items added; New size is " << m_objects.count();
}

void GMWItemModel::itemChanged()
{
    for(int i = 0; i <  m_objects.count() - 1; ++i) {
        if(m_objects.at(i) == sender()) {
            emit dataChanged(index(i, 0),index(i,0));
        }
    }
}

void GMWItemModel::clearAll() {
    if (m_objects.isEmpty()) return;

    qDebug() << "Removing all items from model; Current size is" << m_objects.count();
    beginRemoveRows(QModelIndex(), 0, m_objects.size()-1);
    qDeleteAll(m_objects);
    m_objects.clear();
    endRemoveRows();
    qDebug() << "All items removed";
}

void GMWItemModel::clearVehicles() {
    qDebug() << "Removing vehicles from model; Current size is" << m_objects.count();
    QMutableListIterator<GMWItem*> i(m_objects);
    while (i.hasNext()) {
        if (i.next()->objectType() == GMWItem::TypeVehicle) {
            beginRemoveRows(QModelIndex(), m_objects.indexOf(i.value()), m_objects.indexOf(i.value()));
            delete i.value();
            i.remove();
            endRemoveRows();
        }
    }
    qDebug() << "Vehicles removed; Size:" << m_objects.count();
}

void GMWItemModel::currentPositionChanged(const QGeoCoordinate &position, qreal direction)
{
    foreach (GMWItem* obj, m_objects) {
         obj->calculateDistance(position, direction);
    }
    emit dataChanged(index(0, 1), index(m_objects.size()-1, 2));
}

int GMWItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_objects.count();
}

int GMWItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (m_currentPosition.isValid()) {
        return 1;
    } else {
        return 3;
    }
}

QVariant GMWItemModel::data(const QModelIndex &index, int role) const
{
    GMWItem *object = m_objects.at(index.row());

    switch(role) {
    case RoleName:
        return object->name();
    case RoleObject:
        return qVariantFromValue(object);
    case RoleAddress:
        return object->address();
    case RoleDistance:
        return object->distance();
    case RoleDistanceString:
        return object->distanceString();
    case RoleType:
        return object->objectType();
    case RoleEngineType: {
        GMWVehicle *vehicle = qobject_cast<GMWVehicle*>(object);
        if(vehicle) {
            return vehicle->engineType();
        }
        break;
    }
    case RoleParkingCP: {
        GMWParkingSpot *spot = qobject_cast<GMWParkingSpot*>(object);
        if(spot) {
            return spot->chargingPole();
        }
        break;
    }
    }

    return QVariant();
}

GMWItem *GMWItemModel::item(int row) const
{
    return m_objects.at(row);
}

GMWItem *GMWItemModel::nearestItem(GMWItem::Type type) const
{
    GMWItem *nearest = 0;
    foreach(GMWItem *item, m_objects) {
        if(item->objectType() == type) {
            if(nearest == 0 || item->distance() < nearest->distance()) {
                nearest = item;
            }
        }
    }
    return nearest;
}

GMWVehicle *GMWItemModel::findVehicle(const QString &vin) const
{
    foreach(GMWItem *item, m_objects) {
        if(item->objectType() == GMWItem::TypeVehicle) {
            GMWVehicle *vehicle = dynamic_cast<GMWVehicle*>(item);
            if(vehicle->vin() == vin) {
                return vehicle;
            }
        }
    }
    return 0;
}
