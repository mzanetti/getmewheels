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

#include "gmwitemsortfilterproxymodel.h"

#include <core.h>
#include <qmath.h>
#include <QDebug>

GMWItemSortFilterProxyModel::GMWItemSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_sourceModel(0),
    m_onlyBooked(false),
    m_thinning(false)
{
    m_gmwObjectTypes = GMWItem::TypeUnknown | GMWItem::TypeVehicle | GMWItem::TypeGasStation | GMWItem::TypeParkingSpot;

    QHash<int, QByteArray> roleNames;
    roleNames.insert(GMWItemModel::RoleObject, "gmwitem");
    roleNames.insert(GMWItemModel::RoleName, "itemName");
    roleNames.insert(GMWItemModel::RoleAddress, "itemAddress");
    roleNames.insert(GMWItemModel::RoleDistance, "itemDistance");
    roleNames.insert(GMWItemModel::RoleDistanceString, "itemDistanceString");
    roleNames.insert(GMWItemModel::RoleType, "itemType");
    roleNames.insert(GMWItemModel::RoleEngineType, "itemEngineType");
    roleNames.insert(GMWItemModel::RoleParkingCP, "itemParkingCP");

#if QT_VERSION < 0x050000
    setRoleNames(roleNames);
#else
    m_roleNames = roleNames;
#endif

    setSortRole(GMWItemModel::RoleDistance);
}

GMWItem::Types GMWItemSortFilterProxyModel::filterType() const
{
    return m_gmwObjectTypes;
}

void GMWItemSortFilterProxyModel::setFilterType(GMWItem::Types types)
{
    m_gmwObjectTypes = types;
    invalidateFilter();
    emit filterChanged(m_gmwObjectTypes);
}

void GMWItemSortFilterProxyModel::setItemModel(GMWItemModel *model)
{
    m_sourceModel = model;
    setSourceModel(model);
    emit itemModelChanged();
    sort(0);
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(modelDataChanged(QModelIndex, QModelIndex)));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(updateThinningFilter()));}

void GMWItemSortFilterProxyModel::modelDataChanged(const QModelIndex &firstIndex, const QModelIndex &lastIndex)
{
    Q_UNUSED(firstIndex)
    Q_UNUSED(lastIndex)
    sort(0);
    updateThinningFilter();
}

void GMWItemSortFilterProxyModel::updateThinningFilter()
{
    if (!m_thinning || !m_sourceModel) {
        return;
    }

    double degrees;
    if(m_zoomLevel < 9) {
        degrees = .1;
    } else if(m_zoomLevel < 14){
        degrees = .4 / qPow(2,(m_zoomLevel - 8));
    } else if(m_zoomLevel < 17){
        degrees = .2 / qPow(2,(m_zoomLevel - 8));
    } else {
        degrees = 0;
    }

    // TODO: add a thinningFactor property
    degrees *= 2;

//    qDebug() << "filtering for" << degrees << "°";

    m_visibleItems.clear();
    for (int i = 0; i < sourceModel()->rowCount() -1 ; ++i) {
        GMWItem *newItem = m_sourceModel->item(i);

        bool alwaysShown = false;
        if(newItem->objectType() == GMWItem::TypeVehicle) {
            GMWVehicle *vehicle = qobject_cast<GMWVehicle*>(newItem);
            if(vehicle->booking()->isValid()) {
                alwaysShown = true;
            }
        }
        bool intersects = false;
        if(!alwaysShown && m_zoomLevel < 17) {
            foreach(GMWItem *tmp, m_visibleItems.keys()) {
                if(tmp->objectType() == newItem->objectType() && m_visibleItems.value(tmp).contains(newItem->location())) {
                    intersects = true;
                    break;
                }
            }
        }
        if(!intersects) {
            m_visibleItems.insert(newItem, QGeoRectangle(newItem->location(), degrees, degrees));
        }
    }
//    qDebug() << "filtering finished";

    invalidateFilter();
//    qDebug() << "map contains" << m_visibleItems.count() << "items now";
}

GMWItemModel *GMWItemSortFilterProxyModel::itemModel()
{
    return qobject_cast<GMWItemModel*>(sourceModel());
}

bool GMWItemSortFilterProxyModel::onlyBooked()
{
    return m_onlyBooked;
}

void GMWItemSortFilterProxyModel::setOnlyBooked(bool onlyBooked)
{
    m_onlyBooked = onlyBooked;
    invalidateFilter();
    sort(0);
}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> GMWItemSortFilterProxyModel::roleNames() const
{
    return m_roleNames;
}
#endif

bool GMWItemSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    GMWItem* object = sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), Qt::UserRole).value<GMWItem*>();
    if(m_onlyBooked) {
        if(object->objectType() != GMWItem::TypeVehicle) {
            return false;
        }
        GMWVehicle *vehicle = qobject_cast<GMWVehicle*>(object);
        if(!vehicle->booking()->isValid()) {
            return false;
        }
    }

    if (m_thinning && !m_visibleItems.keys().contains(object)) {
        return false;
    }
    return m_gmwObjectTypes.testFlag(object->objectType());
}

bool GMWItemSortFilterProxyModel::thinningEnabled() const
{
    return m_thinning;
}

void GMWItemSortFilterProxyModel::setThinningEnabled(bool thinningEnabled)
{
    m_thinning = thinningEnabled;
    updateThinningFilter();
}

int GMWItemSortFilterProxyModel::zoomLevel() const
{
    return m_zoomLevel;
}

void GMWItemSortFilterProxyModel::setZoomLevel(int zoomLevel)
{
    if (zoomLevel != m_zoomLevel) {
        qDebug() << "zoom level set" << zoomLevel;
        m_zoomLevel = zoomLevel;
        updateThinningFilter();
    }
}
