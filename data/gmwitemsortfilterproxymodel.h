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

#ifndef GMWITEMSORTFILTERPROXYMODEL_H
#define GMWITEMSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QGeoPositionInfo>

#include "gmwitem.h"

class GMWItemSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit GMWItemSortFilterProxyModel(QObject *parent = 0);
    GMWItem::Types filterGMWObjectType() const;
    void setFilterGMWObjectType(GMWItem::Types types);

signals:
    void filterChanged(GMWItem::Types types);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    GMWItem::Types m_gmwObjectTypes;
    QGeoCoordinate m_currentPosition;
};

#endif // GMWITEMSORTFILTERPROXYMODEL_H
