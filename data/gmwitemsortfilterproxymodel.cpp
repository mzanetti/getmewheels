/*****************************************************************************
 * Copyright: 2010-2011 Christian Fetzer <fetzer.ch@googlemail.com>          *
 * Copyright: 2010-2011 Michael Zanetti <mzanetti@kde.org>                   *
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

#include <QDebug>

GMWItemSortFilterProxyModel::GMWItemSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_gmwObjectTypes = GMWItem::TypeUnknown | GMWItem::TypeVehicle | GMWItem::TypeGasStation | GMWItem::TypeParkingLot;

    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::UserRole, "gmwitem");
    setRoleNames(roleNames);

}

GMWItem::Types GMWItemSortFilterProxyModel::filterGMWObjectType() const
{
    return m_gmwObjectTypes;
}

void GMWItemSortFilterProxyModel::setFilterGMWObjectType(GMWItem::Types types)
{
    m_gmwObjectTypes = types;
    invalidateFilter();
    emit filterChanged(m_gmwObjectTypes);
}

bool GMWItemSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    GMWItem* object = sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), Qt::UserRole).value<GMWItem*>();

    return m_gmwObjectTypes.testFlag(object->objectType());
}

bool GMWItemSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    GMWItem* leftObject = sourceModel()->data(left, Qt::UserRole).value<GMWItem*>();
    GMWItem* rightObject = sourceModel()->data(right, Qt::UserRole).value<GMWItem*>();

    switch (left.column()) {
        case 0:
            return leftObject->objectType() < rightObject->objectType();
        case 1:
            return leftObject->address() < rightObject->address();
        case 2:
            return leftObject->distance() < rightObject->distance();
        case 3:
            return leftObject->azimuth() < rightObject->azimuth();
        default:
            return leftObject->distance() < rightObject->distance();
    }
}
