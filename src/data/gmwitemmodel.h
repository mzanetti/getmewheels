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

#ifndef GMWITEMMODEL_H
#define GMWITEMMODEL_H

#include "gmwitem.h"
#include "engines/gmwengine.h"

#include <QAbstractTableModel>
#include <QGeoPositionInfo>

class GMWVehicle;

class GMWItemModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(GMWEngine* engine READ engine WRITE setEngine NOTIFY engineChanged)

public:
    enum Role {
        RoleName = Qt::DisplayRole,
        RoleObject = Qt::UserRole,
        RoleAddress,
        RoleDistance,
        RoleType,
        RoleDistanceString,
        RoleEngineType,
        RoleParkingCP
    };

    explicit GMWItemModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;

    GMWItem *nearestItem(GMWItem::Type type) const;
    GMWVehicle *findVehicle(const QString &vin) const;

    GMWEngine *engine();
    void setEngine(GMWEngine *engine);

#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif

public slots:
    void addObject(GMWItem *item);
    void addObjects(QList<GMWItem*> items);
    void clearAll();
    void clearVehicles();
    void currentPositionChanged(const QGeoCoordinate &position, qreal direction = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

signals:
    void locationChanged();
    void engineChanged();

private:
    GMWEngine *m_engine;
    QList<GMWItem*> m_objects;
    QGeoCoordinate m_currentPosition;

#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> m_roleNames;
#endif

private slots:
    void itemChanged();
};

#endif // GMWITEMMODEL_H
