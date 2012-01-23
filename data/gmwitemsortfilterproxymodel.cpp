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
#include <QDebug>

GMWItemSortFilterProxyModel::GMWItemSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
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
    setRoleNames(roleNames);

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
    setSourceModel(model);
    emit itemModelChanged();
    sort(0);
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(modelDataChanged(QModelIndex, QModelIndex)));
}

void GMWItemSortFilterProxyModel::modelDataChanged(const QModelIndex &firstIndex, const QModelIndex &lastIndex)
{
    Q_UNUSED(firstIndex)
    Q_UNUSED(lastIndex)
    sort(0);
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
    return m_gmwObjectTypes.testFlag(object->objectType());
}

//bool GMWItemSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
//{
//    GMWItem* leftObject = sourceModel()->data(left, Qt::UserRole).value<GMWItem*>();
//    GMWItem* rightObject = sourceModel()->data(right, Qt::UserRole).value<GMWItem*>();

//    switch (left.column()) {
//        case 0:
//            return leftObject->objectType() < rightObject->objectType();
//        case 1:
//            return leftObject->address() < rightObject->address();
//        case 2:
//            return leftObject->distance() < rightObject->distance();
//        case 3:
//            return leftObject->azimuth() < rightObject->azimuth();
//        default:
//            return leftObject->distance() < rightObject->distance();
//    }
//}
