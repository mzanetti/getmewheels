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

#include "data/gmwitem.h"
#include "data/gmwvehicle.h"
#include "data/gmwbusinessarea.h"
#include "data/gmwbooking.h"

#include <QtNetwork/QNetworkReply>

#include <qgeoboundingbox.h>

class GMWEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QGeoBoundingBox startingBounds READ startingBounds NOTIFY locationChanged)
    Q_PROPERTY(QString defaultAccountName READ defaultAccountName NOTIFY defaultAccountChanged)
    Q_PROPERTY(bool authenticated READ authenticated NOTIFY authenticatedChanged)
    Q_PROPERTY(QDateTime authExpirationDate READ authExpirationDate NOTIFY authenticatedChanged())

public:
    GMWEngine();
    virtual ~GMWEngine();

    void setLocation(const QString &location);
    QString location();

    Q_INVOKABLE virtual QGeoBoundingBox startingBounds() = 0;
    Q_INVOKABLE virtual QGeoCoordinate center() = 0;

    Q_INVOKABLE virtual QStringList supportedLocations() = 0;

    virtual GMWBusinessArea businessArea() = 0;

    Q_INVOKABLE virtual bool startAuthentication() = 0;
    Q_INVOKABLE virtual bool setAccessCode(const QString &code) = 0;
    Q_INVOKABLE virtual QDateTime authExpirationDate() = 0;
    virtual bool authenticated() = 0;

    Q_INVOKABLE virtual QList<GMWAccount> accounts() = 0;
    Q_INVOKABLE QStringList accountNames();
    virtual GMWAccount defaultAccount() = 0;
    Q_INVOKABLE QString defaultAccountName();

    virtual QList<GMWVehicle*> bookings() = 0;

    Q_INVOKABLE virtual QString error() = 0;

public slots:
    virtual void refreshStationary(bool useCache = true) = 0;
    virtual void refreshVehicles(bool useCache = true) = 0;

    virtual void setDefaultAccount(const GMWAccount &account) = 0;
    void clearDefaultAccount();
    void setDefaultAccountName(const QString &accountName);
    virtual bool createBooking(GMWVehicle *vehicle, const GMWAccount &account = GMWAccount()) = 0;
    virtual bool cancelBooking(GMWVehicle *vehicle) = 0;

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

private:
    QString m_location;
};

#endif // GMWENGINE_H
