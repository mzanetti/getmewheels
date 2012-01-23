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

#ifndef GMWITEMSORTFILTERPROXYMODEL_H
#define GMWITEMSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QGeoPositionInfo>

#include "gmwitem.h"
#include "gmwitemmodel.h"

class GMWItemSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(GMWItemModel *model READ itemModel WRITE setItemModel NOTIFY itemModelChanged())
    Q_PROPERTY(bool onlyBooked READ onlyBooked WRITE setOnlyBooked)

public:

    explicit GMWItemSortFilterProxyModel(QObject *parent = 0);
    GMWItem::Types filterType() const;
    void setFilterType(GMWItem::Types types);
    void setItemModel(GMWItemModel *model);
    GMWItemModel *itemModel();

    bool onlyBooked();
    void setOnlyBooked(bool onlyBooked);

signals:
    void filterChanged(GMWItem::Types types);
    void itemModelChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
//    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private slots:
    void modelDataChanged(const QModelIndex &firstIndex, const QModelIndex &lastIndex);
private:
    GMWItem::Types m_gmwObjectTypes;
    QGeoCoordinate m_currentPosition;
    bool m_onlyBooked;
};


#endif // GMWITEMSORTFILTERPROXYMODEL_H
