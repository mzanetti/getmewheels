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

#ifndef GMWVEHICLE_H
#define GMWVEHICLE_H

#include "gmwitem.h"
#include "gmwbooking.h"

class GMWVehicle : public GMWItem
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_ENUMS(EngineType)
    Q_PROPERTY(quint8 fuelLevel READ fuelLevel NOTIFY changed)
    Q_PROPERTY(State interiorState READ interiorState NOTIFY changed)
    Q_PROPERTY(State exteriorState READ exteriorState NOTIFY changed)
    Q_PROPERTY(QString vin READ vin NOTIFY changed)
    Q_PROPERTY(GMWBooking* booking READ booking NOTIFY changed)
    Q_PROPERTY(EngineType engineType READ engineType NOTIFY changed)

public:
    friend QDataStream &operator<<(QDataStream& stream, const GMWVehicle& vehicle);
    friend QDataStream &operator>>(QDataStream& stream, GMWVehicle& vehicle);

    enum State {
        StateUnknown = -1,
        StateIssuesExist,
        StateUnacceptable,
        StateSatisfied,
        StateGood,
        StateExcellent
    };

    enum EngineType {
        EngineTypeED,
        EngineTypeCD
    };

    GMWVehicle(const QString &name, const QString &address, const QGeoCoordinate &location, const QPixmap &image, const QPixmap &imageL,
        quint8 fuelLevel, GMWVehicle::State interiorState, GMWVehicle::State exteriorState, const QString &vin, EngineType engineType);

    static GMWVehicle::State vehicleStateFromString(const QString &state);

    GMWItem::Type objectType() const;

    quint8 fuelLevel() const;
    GMWVehicle::State interiorState() const;
    GMWVehicle::State exteriorState() const;
    QString vin() const;
    EngineType engineType() const;

    void setBooking(GMWBooking *booking);
    GMWBooking *booking() const;
    void clearBooking();

signals:
    void changed();

private:
    quint8 m_fuelLevel; // 0-100
    GMWVehicle::State m_interiorState;
    GMWVehicle::State m_exteriorState;
    QString m_vin;      // Vehicle identification number
    GMWBooking *m_booking;
    EngineType m_engineType;
};

QDataStream &operator<<(QDataStream& stream, const GMWVehicle& vehicle);
QDataStream &operator>>(QDataStream& stream, GMWVehicle& vehicle);

#endif // GMWVEHICLE_H
