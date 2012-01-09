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

#ifndef GMWPARKINGSPOT_H
#define GMWPARKINGSPOT_H

#include "gmwitem.h"

class GMWParkingSpot : public GMWItem
{
    Q_OBJECT
    Q_PROPERTY(quint16 capacityUsed READ capacityUsed NOTIFY changed)
    Q_PROPERTY(quint16 capacityTotal READ capacityTotal NOTIFY changed)

public:
    friend QDataStream &operator<<(QDataStream& stream, const GMWParkingSpot& parkingSpot);
    friend QDataStream &operator>>(QDataStream& stream, GMWParkingSpot& parkingSpot);

    GMWParkingSpot(const QString &name, const QString &address, const QGeoCoordinate &location, const QPixmap image,
        quint16 capacityUsed, quint16 capacityTotal);

    GMWItem::Type objectType() const;

    quint16 capacityUsed() const;
    quint16 capacityTotal() const;

signals:
    void changed();

private:
    quint16 m_capacityUsed;
    quint16 m_capacityTotal;
};

QDataStream &operator<<(QDataStream& stream, const GMWParkingSpot& parkingSpot);
QDataStream &operator>>(QDataStream& stream, GMWParkingSpot& parkingSpot);

#endif // GMWPARKINGSPOT_H
