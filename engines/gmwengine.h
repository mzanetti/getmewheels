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

#ifndef GMWENGINE_H
#define GMWENGINE_H

#include "engineplugin.h"
#include "location.h"
#include "data/gmwitem.h"
#include "data/gmwvehicle.h"
#include "data/gmwbusinessarea.h"
#include "data/gmwbooking.h"
#include "data/gmwgasstation.h"
#include "data/gmwparkingspot.h"

#include <QtNetwork/QNetworkReply>

#include <qgeoboundingbox.h>

class GMWEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationChanged)
    Q_PROPERTY(QString defaultAccountName READ defaultAccountName WRITE setDefaultAccountName NOTIFY defaultAccountChanged)
    Q_PROPERTY(bool authenticated READ authenticated NOTIFY authenticatedChanged)
    Q_PROPERTY(QDateTime authExpirationDate READ authExpirationDate NOTIFY authenticatedChanged())

public:
    GMWEngine();
    virtual ~GMWEngine();

    void setLocationName(const QString &location);
    QString locationName();
    Location* location();


    Q_INVOKABLE QStringList supportedLocationNames();
    Q_INVOKABLE QList<Location *> supportedLocations();

    Q_INVOKABLE bool startAuthentication();
    Q_INVOKABLE bool setAccessCode(const QString &code);
    Q_INVOKABLE QDateTime authExpirationDate();
    bool authenticated();

    Q_INVOKABLE QList<GMWAccount> accounts(Location *location = 0);
    Q_INVOKABLE QStringList accountNames(const QString &location);
    GMWAccount defaultAccount();
    Q_INVOKABLE QString defaultAccountName();

    QList<GMWVehicle*> bookings();

    Q_INVOKABLE QString error();

public slots:
    void refreshStationary(bool useCache = true);
    void refreshVehicles(bool useCache = true);

    void setDefaultAccount(const GMWAccount &account);
    void clearDefaultAccount();
    void setDefaultAccountName(const QString &accountName);
    bool createBooking(GMWVehicle *vehicle, const GMWAccount &account = GMWAccount());
    bool cancelBooking(GMWVehicle *vehicle);

signals:
    void objectsReceived(QList<GMWItem*> list);
    void downloadStarted();
    void downloadFinished(GMWItem::Type type, bool success, const QString &errorString);
    void downloadError(const QString &message);
    void loadedFromCache(GMWItem::Type type, const QDateTime &cacheDate);
    void locationChanged();
    void businessAreaChanged();
    void authenticatedChanged();
    void defaultAccountChanged();

private slots:
    void gasStationsReceived(QList<GMWGasStation*> gasStations);
    void parkingSpotsReceived(QList<GMWParkingSpot*> parkingSpots);
    void vehiclesReceived(QList<GMWVehicle*> vehicles);
private:
    EnginePlugin *m_plugin;

    QList<Location*> m_supportedLocations;
    Location *m_location;

    QHash<Location*, QList<GMWAccount> > m_accounts;
};

#endif // GMWENGINE_H
