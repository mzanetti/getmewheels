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

#include "gmwparkingspot.h"

GMWParkingSpot::GMWParkingSpot(const QString &name, const QString &address, const QGeoCoordinate &location, const QPixmap &image,
        const QPixmap &imageL, quint16 capacityUsed, quint16 capacityTotal, bool chargingPole) :
    GMWItem(name, address, location, image, imageL),
    m_capacityUsed(capacityUsed),
    m_capacityTotal(capacityTotal),
    m_chargingPole(chargingPole)
{
}

GMWItem::Type GMWParkingSpot::objectType() const
{
    return GMWItem::TypeParkingSpot;
}

quint16 GMWParkingSpot::capacityUsed() const
{
    return m_capacityUsed;
}

quint16 GMWParkingSpot::capacityTotal() const
{
    return m_capacityTotal;
}

bool GMWParkingSpot::chargingPole() const
{
    return m_chargingPole;
}

QDataStream &operator<<(QDataStream& stream, const GMWParkingSpot& parkingSpot)
{
    stream << dynamic_cast<const GMWItem&>(parkingSpot);
    stream << parkingSpot.m_capacityUsed << parkingSpot.m_capacityTotal << parkingSpot.chargingPole();
    return stream;
}

QDataStream &operator>>(QDataStream& stream, GMWParkingSpot& parkingSpot)
{
    stream >> dynamic_cast<GMWItem&>(parkingSpot);
    stream >> parkingSpot.m_capacityUsed;
    stream >> parkingSpot.m_capacityTotal;
    stream >> parkingSpot.m_chargingPole;
    return stream;
}
