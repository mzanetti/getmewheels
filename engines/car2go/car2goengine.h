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

#ifndef CAR2GOENGINE_H
#define CAR2GOENGINE_H

#include "engines/gmwengine.h"

#include <QtCore/QList>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>

#include <QtOAuth/QtOAuth>

class Car2goEngine : public GMWEngine
{
    Q_OBJECT

public:
    // See http://code.google.com/p/car2go/wiki/OpenAPIv20r10Locations
    Car2goEngine();

    QStringList supportedLocations();

    Q_INVOKABLE QGeoBoundingBox startingBounds();
    Q_INVOKABLE QGeoCoordinate center() {return startingBounds().center(); }

    GMWBusinessArea businessArea();

    bool startAuthentication();
    bool setAccessCode(const QString &code);
    bool authenticated();
    QDateTime authExpirationDate();

    QString error() {return m_error;}

public slots: // for easier testing now
    QList<GMWAccount> accounts();
    void setDefaultAccount(const GMWAccount &account);
    GMWAccount defaultAccount();

    bool createBooking(GMWVehicle *vehicle, const GMWAccount &account = GMWAccount());
    QList<GMWVehicle*> bookings();
    bool cancelBooking(GMWVehicle *vehicle);


public slots:
    void refreshStationary(bool useCache = true);
    void refreshVehicles(bool useCache = true);

private:
    QMap<QString, QString> m_locations;

    enum NetworkReplyType {IconReply, GasStationReply, VehiclesReply, ParkingSpotsReply};
    QString m_cacheDir;

    QNetworkAccessManager m_network;

    QNetworkReply* m_networkReplyParkingSpots;
    QNetworkReply* m_networkReplyGasStations;
    QNetworkReply* m_networkReplyVehicles;
    QNetworkReply* m_networkReplyParkingSpotsImage;
    QNetworkReply* m_networkReplyGasStationsImage;
    QNetworkReply* m_networkReplyVehiclesImage;
    QNetworkReply* m_networkReplyAccounts;
    QNetworkReply* m_networkReplyBookings;
    QNetworkReply* m_networkReplyCreateBooking;
    QNetworkReply* m_networkReplyCancelBooking;

    QList<QNetworkReply*> m_downloads;

    QString m_xmlParkingSpots;
    QString m_xmlGasStations;
    QString m_xmlVehicles;

    QPixmap m_imageParkingSpots;
    QPixmap m_imageGasStations;
    QPixmap m_imageVehicles;

    QTime m_stats;

    GMWBusinessArea m_businessArea;

    QOAuth::Interface *m_qoauth;
    static const QByteArray Car2GoRequestTokenURL;
    static const QByteArray Car2GoAccessTokenURL;
    static const QByteArray Car2GoAuthorizeURL;

    static const QByteArray ConsumerKey;
    static const QByteArray ConsumerSecret;

    static const QByteArray ParamCallback;
    static const QByteArray ParamCallbackValue;
    static const QByteArray ParamVerifier;
    static const QByteArray ParamScreenName;

    QByteArray m_screenName;
    QByteArray m_token;
    QByteArray m_tokenSecret;
    GMWAccount m_account;
    QEventLoop m_loop;

    void parseImage(NetworkReplyType type, const QString &xml);
    void parse(NetworkReplyType type, const QString &xml);
    bool hasDownloadError() const;

    void refreshGasStations(bool useCache);
    void refreshParkingSpots(bool useCache);

    bool waitForResponse();
    bool m_timeout;

    QList<GMWAccount> m_accounts;

    GMWVehicle *parseVehicle(const QVariantMap &vehicleMap);

    QString m_error;

private slots:
    void receivedData(QNetworkReply *reply);
    void loadBusinessArea();
};

#endif // CAR2GOENGINE_H
