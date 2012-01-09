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

#include "gmwvehicle.h"

GMWVehicle::GMWVehicle(const QString &name, const QString &address, const QGeoCoordinate &location, const QPixmap image,
        quint8 fuelLevel, GMWVehicle::State interiorState, GMWVehicle::State exteriorState, const QString &vin) :
    GMWItem(name, address, location, image),
    m_fuelLevel(fuelLevel), m_interiorState(interiorState), m_exteriorState(exteriorState), m_vin(vin), m_booking(new GMWBooking(this))
{
}

GMWVehicle::State GMWVehicle::vehicleStateFromString(const QString &state)
{
    if (state.toLower() == "excellent") return GMWVehicle::StateExcellent;
    else if (state.toLower() == "good") return GMWVehicle::StateGood;
    else if (state.toLower() == "satisfied") return GMWVehicle::StateSatisfied;
    else if (state.toLower() == "unacceptable") return GMWVehicle::StateUnacceptable;
    else if (state.toLower() == "issuesexist") return GMWVehicle::StateIssuesExist;
    else return GMWVehicle::StateUnknown;
}

GMWItem::Type GMWVehicle::objectType() const
{
    return GMWItem::TypeVehicle;
}

quint8 GMWVehicle::fuelLevel() const
{
    return m_fuelLevel;
}

GMWVehicle::State GMWVehicle::interiorState() const
{
    return m_interiorState;
}

GMWVehicle::State GMWVehicle::exteriorState() const
{
    return m_exteriorState;
}

QString GMWVehicle::vin() const
{
    return m_vin;
}

void GMWVehicle::setBooking(GMWBooking *booking)
{
    booking->setParent(this);
    m_booking = booking;
    emit changed();
}

GMWBooking* GMWVehicle::booking() const
{
    return m_booking;
}

void GMWVehicle::clearBooking()
{
    m_booking->invalidate();
    emit changed();
}

QDataStream &operator<<(QDataStream& stream, const GMWVehicle& vehicle)
{
    stream << dynamic_cast<const GMWItem&>(vehicle);
    stream << vehicle.m_fuelLevel << vehicle.m_vin << (qint32)vehicle.m_interiorState << (qint32)vehicle.m_exteriorState;
    //stream << vehicle.m_fuelLevel << vehicle.m_vin;
    return stream;
}

QDataStream &operator>>(QDataStream& stream, GMWVehicle& vehicle)
{
    qint32 interiorState;
    qint32 exteriorState;
    stream >> dynamic_cast<GMWItem&>(vehicle);
    stream >> vehicle.m_fuelLevel;
    stream >> vehicle.m_vin;
    stream >> interiorState;
    vehicle.m_interiorState = (GMWVehicle::State)interiorState;
    stream >> exteriorState;
    vehicle.m_exteriorState = (GMWVehicle::State)exteriorState;
    return stream;
}
