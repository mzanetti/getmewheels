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

#include "car2goengine.h"

#include "data/gmwparkingspot.h"
#include "data/gmwgasstation.h"
#include "data/gmwvehicle.h"

#include <qjson/parser.h>

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtGui/QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <QTimer>
#include <QBuffer>
#include <QPainter>
#include <QtXml/QXmlStreamReader>

const QByteArray Car2goEngine::Car2GoRequestTokenURL = "https://www.car2go.com/api/reqtoken";
const QByteArray Car2goEngine::Car2GoAccessTokenURL  = "https://www.car2go.com/api/accesstoken";
const QByteArray Car2goEngine::Car2GoAuthorizeURL    = "https://www.car2go.com/api/authorize";

const QByteArray Car2goEngine::ConsumerKey    = "GetMeWheels";
const QByteArray Car2goEngine::ConsumerSecret = "qleDqQghx5lPelzT";

const QByteArray Car2goEngine::ParamCallback      = "oauth_callback";
const QByteArray Car2goEngine::ParamCallbackValue = "foo";
const QByteArray Car2goEngine::ParamVerifier      = "oauth_verifier";
const QByteArray Car2goEngine::ParamScreenName    = "screen_name";


Car2goEngine::Car2goEngine() :
    m_networkReplyParkingSpots(NULL),
    m_networkReplyGasStations(NULL),
    m_networkReplyVehicles(NULL),
    m_networkReplyParkingSpotsImage(NULL),
    m_networkReplyGasStationsImage(NULL),
    m_networkReplyVehiclesImage(NULL)
{
    connect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(receivedData(QNetworkReply*)));

    // Create the cache directory if it does not exist
    m_cacheDir = QDir::homePath() + "/.getmewheels/car2go";
    QDir cacheDirectory(m_cacheDir);
    if(!cacheDirectory.exists()){
        qDebug() << "cachedir for car2go doesnt exist... creating" << m_cacheDir;
        cacheDirectory.mkpath(m_cacheDir);
    }

    connect(this, SIGNAL(locationChanged()), SLOT(loadBusinessArea()));

    // Init qoauth
    m_qoauth = new QOAuth::Interface( this );
    m_qoauth->setConsumerKey( ConsumerKey );
    m_qoauth->setConsumerSecret( ConsumerSecret );

    QSettings settings("getmewheels", "getmewheels");
    settings.beginGroup("car2go");
    if(settings.contains("OAuthAccessToken")) {
        m_account = GMWAccount(settings.value("OAuthAccountID").toInt(), settings.value("OAuthAccountDescription").toString());
        m_token = settings.value("OAuthAccessToken").toByteArray();
        m_tokenSecret = settings.value("OAuthAccessTokenSecret").toByteArray();

        qDebug() << "loaded token:" << m_token << "secret:" << m_tokenSecret;
    }

    m_locations.insert("Amsterdam", "amsterdam");
    m_locations.insert("Austin", "austin");
//    m_locations.insert(QString("Düsseldorf").toUtf8(), "duesseldorf");
    m_locations.insert("Hamburg", "hamburg");
    m_locations.insert("San Diego", "sandiego");
    m_locations.insert("Ulm", "ulm");
    m_locations.insert("Vancouver", "vancouver");
    m_locations.insert("Washington DC", "washingtondc");
    m_locations.insert("Wien", "wien");
}

QStringList Car2goEngine::supportedLocations()
{
    return m_locations.keys();
}

QGeoBoundingBox Car2goEngine::startingBounds()
{
    if(location() == "Amsterdam") {
        return QGeoBoundingBox(QGeoCoordinate(52.396896, 4.853783), QGeoCoordinate(52.339317, 4.949881));
    } else if (location() == "Austin") {
        return QGeoBoundingBox(QGeoCoordinate(30.29000, -97.77000), QGeoCoordinate(30.25000, -97.70000));
    } else if (location() == "Hamburg") {
        return QGeoBoundingBox(QGeoCoordinate(53.72000, 9.84000), QGeoCoordinate(53.41300, 10.15500));
    } else if(location() == "San Diego") {
        return QGeoBoundingBox(QGeoCoordinate(32.80715, -117.250194), QGeoCoordinate(32.707689, -117.117355));
    } else if (location() == "Ulm") {
        return QGeoBoundingBox(QGeoCoordinate(48.41000, 9.96000), QGeoCoordinate(48.39000, 10.00000));
    } else if(location() == "Vancouver") {
        return QGeoBoundingBox(QGeoCoordinate(49.314789, -123.237269), QGeoCoordinate(49.230886, -123.062995));
    } else if(location() == "Washington DC") {
        return QGeoBoundingBox(QGeoCoordinate(38.978878, -77.165943), QGeoCoordinate(38.810696, -76.919818));
    } else if(location() == "Wien") {
        return QGeoBoundingBox(QGeoCoordinate(48.303924, 16.240785), QGeoCoordinate(48.133475, 16.532411));
    } else {
        return QGeoBoundingBox();
    }
}

QList<GMWAccount> Car2goEngine::accounts()
{
    if(!m_accounts.isEmpty()) {
        return m_accounts;
    }
    QList<GMWAccount> retList;

    QByteArray requestUrl( "https://www.car2go.com/api/v2.0/accounts");

    QOAuth::ParamMap map;
    // create a request parameters map
    map.insert("loc", m_locations.value(location()).toUtf8());
    map.insert("format", "json");

    // construct the parameters string
    QByteArray content = m_qoauth->createParametersString(requestUrl, QOAuth::GET, m_token, m_tokenSecret, QOAuth::HMAC_SHA1, map, QOAuth::ParseForInlineQuery);

    // append parameters string to the URL
    requestUrl.append( content );
    qDebug() << "requesting accounts:" << requestUrl;
    QNetworkRequest request;
    request.setUrl(QUrl::fromEncoded(requestUrl));
//    request.setUrl(QUrl(requestUrl));

    qDebug() << "getting:" << request.url();

    m_networkReplyAccounts = m_network.get(request);

    if(!waitForResponse()) {
        qDebug() << "timeout:" << m_networkReplyAccounts->readAll();
        return retList;
    }
    QByteArray response = m_networkReplyAccounts->readAll();
    qDebug() << "got response" << response;
    QJson::Parser parser;
    bool ok = true;
    QVariantMap accountsMap = parser.parse(response, &ok).toMap();
    if(ok) {
        foreach(const QVariant &account, accountsMap.value("account").toList()) {
            retList.append(GMWAccount(account.toMap().value("accountId").toInt(), account.toMap().value("description").toString()));
            qDebug() << "got account:" << retList.last().id() << retList.last().description();
        }

    }
    m_accounts = retList;
    return retList;

}

void Car2goEngine::setDefaultAccount(const GMWAccount &account)
{
    if(m_account != account) {
        m_account = account;
        QSettings settings("getmewheels", "getmewheels");
        settings.beginGroup("car2go");
        settings.setValue("OAuthAccountID", m_account.id());
        settings.setValue("OAuthAccountDescription", m_account.description());
        emit defaultAccountChanged();
    }
}

GMWAccount Car2goEngine::defaultAccount()
{
    return m_account;
}

bool Car2goEngine::startAuthentication()
{

    // Request Token
    QOAuth::ParamMap params;
    params.insert(ParamCallback, ParamCallbackValue);
    QOAuth::ParamMap requestToken = m_qoauth->requestToken(Car2GoRequestTokenURL, QOAuth::POST, QOAuth::HMAC_SHA1 , params);

    if(m_qoauth->error() != QOAuth::NoError) {
            qDebug() << "############# failed to get requesttoken" << m_qoauth->error() << requestToken;
        return false;
    }

    m_tokenSecret = QUrl::fromEncoded(requestToken.value( QOAuth::tokenSecretParameterName() )).toString().toUtf8();
    m_token = requestToken.value( QOAuth::tokenParameterName() );
    qDebug() << "got token" << m_token;

    QString url = Car2GoAuthorizeURL;
    url.append( "?" );
    url.append( "&" + QOAuth::tokenParameterName() + "=" + m_token );
//    url.append( "&" + ParamCallback + "=" + ParamCallbackValue );
    qDebug() << "got url:" << QUrl(url);

    QDesktopServices::openUrl(QUrl::fromEncoded(url.toUtf8()));

    return true;
}

bool Car2goEngine::setAccessCode(const QString &code)
{
    // Authorize
    QOAuth::ParamMap otherArgs;
    otherArgs.insert( ParamVerifier, code.toLocal8Bit().toPercentEncoding());
    qDebug() << "Setting auth code" << code << ":" << otherArgs.value(ParamVerifier);
    qDebug() << "Token" << m_token;
    qDebug() << "TokenSecret" << m_tokenSecret;
    qDebug() << "Url" << Car2GoAccessTokenURL;
    QOAuth::ParamMap accessToken = m_qoauth->accessToken( Car2GoAccessTokenURL, QOAuth::POST, m_token,
                                                        m_tokenSecret, QOAuth::HMAC_SHA1, otherArgs );
    if(m_qoauth->error() != QOAuth::NoError) {
        qDebug() << "error setting auth code:" << m_qoauth->error();
        emit authenticatedChanged();
        return false;
    }

    // Save informations
    m_screenName = accessToken.value( ParamScreenName );
    m_token = accessToken.value( QOAuth::tokenParameterName() );
    m_tokenSecret = QUrl::fromEncoded(accessToken.value( QOAuth::tokenSecretParameterName() )).toString().toUtf8();

    QSettings settings("getmewheels", "getmewheels");
    settings.beginGroup("car2go");
    settings.setValue("OAuthAccessToken", m_token);
    settings.setValue("OAuthAccessTokenSecret", m_tokenSecret);
    settings.setValue("AuthExpirationDate", QDateTime::currentDateTime().addDays(31));
    qDebug() << "OAuth completed. ScreenName:" << m_screenName << "Token:" << m_token << "TokeSecret:" << m_tokenSecret;

    // Clear accounts cache
    m_accounts.clear();

    emit authenticatedChanged();
    return true;
}

bool Car2goEngine::authenticated()
{
    return !m_token.isEmpty() && !m_tokenSecret.isEmpty();
}

QDateTime Car2goEngine::authExpirationDate()
{
    QSettings settings("getmewheels", "getmewheels");
    settings.beginGroup("car2go");
    return settings.value("AuthExpirationDate").toDateTime();
}

bool Car2goEngine::createBooking(GMWVehicle *vehicle, const GMWAccount &account)
{
//    qDebug() << "Faking booking request";
//    GMWBooking *booking = new GMWBooking(1, account);
//    booking->setTime(QDateTime::currentDateTime());
//    booking->setExpirationTime(QDateTime::currentDateTime().addSecs(60));
//    vehicle->setBooking(booking);
//    return true;


    QByteArray requestUrl("https://www.car2go.com/api/v2.0/bookings");

    QOAuth::ParamMap map;
    // create a request parameters map
    map.insert("format", "json");
    map.insert("loc", m_locations.value(location()).toUtf8());
    map.insert("vin", vehicle->vin().toUtf8());
    map.insert("account", account.isValid() ? QString::number(account.id()).toUtf8() : QString::number(m_account.id()).toUtf8());

    // construct the parameters string
    QByteArray content = m_qoauth->createParametersString(requestUrl, QOAuth::POST, m_token, m_tokenSecret, QOAuth::HMAC_SHA1, map, QOAuth::ParseForRequestContent);

    // append parameters string to the URL
//    requestUrl.append( content );
    qDebug() << "requesting accounts:" << requestUrl;
    QNetworkRequest request;
    request.setUrl(QUrl::fromEncoded(requestUrl));
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    qDebug() << "postin:" << request.url() << "content:" << content;

    m_networkReplyCreateBooking = m_network.post(request, content);

    if(!waitForResponse()) {
        m_error = m_networkReplyCreateBooking->errorString();
        qDebug() << "timeout:" << m_networkReplyCreateBooking->readAll() << "error:" << m_error;
        return false;
    }
    QByteArray response = m_networkReplyCreateBooking->readAll();
    qDebug() << "got response" << response;
    QJson::Parser parser;
    bool ok = true;
    QVariantMap bookingResponseMap = parser.parse(response, &ok).toMap();
    if(ok) {
        if(bookingResponseMap.value("returnValue").toMap().value("code").toInt() != 0) {
            qDebug() << "Booking failed:" << bookingResponseMap.value("returnValue").toMap().value("description").toString();
            m_error = bookingResponseMap.value("returnValue").toMap().value("description").toString();
            return false;
        }
        QVariantList bookingDataList = bookingResponseMap.value("booking").toList();
        qDebug() << "Booking successful";// << bookingData;
        foreach(const QVariant &tmp, bookingDataList) {
//            qDebug() << tmp;
            QVariantMap bookingData = tmp.toMap();
            qDebug() << "account:" << bookingData.value("account").toMap().value("accountId").toLongLong() << bookingData.value("account").toMap().value("description").toString();
            qDebug() << "bookingId:" << bookingData.value("bookingId").toInt();
            qDebug() << "bookingposition:" << bookingData.value("bookingposition").toMap().value("address").toString() << bookingData.value("bookingposition").toMap().value("latitude").toString() << bookingData.value("bookingposition").toMap().value("longitude").toString();

            QDateTime reservationTime = QDateTime::fromTime_t(bookingData.value("reservationTime").toMap().value("timeInMillis").toLongLong()/1000);
            GMWBooking *gmwbooking = new GMWBooking(bookingData.value("bookingId").toInt(), account);
            gmwbooking->setTime(reservationTime);
            gmwbooking->setExpirationTime(reservationTime.addSecs(15*60));
            vehicle->setBooking(gmwbooking);
            vehicle->setImage(m_imageVehicles);
        }
        return true;

//        foreach(const QVariant &account, accountsMap.value("account").toList()) {
//            retList.append(qMakePair(account.toMap().value("accountId").toLongLong(), account.toMap().value("description").toString()));
//        }

    }
    return false;
}

QList<GMWVehicle*> Car2goEngine::bookings()
{
    QList<GMWVehicle*> retList;

    QByteArray requestUrl( "https://www.car2go.com/api/v2.0/bookings");

    QOAuth::ParamMap map;
    // create a request parameters map
    map.insert("loc", m_locations.value(location()).toUtf8());
    map.insert("format", "json");

    // construct the parameters string
    QByteArray content = m_qoauth->createParametersString(requestUrl, QOAuth::GET, m_token, m_tokenSecret, QOAuth::HMAC_SHA1, map, QOAuth::ParseForInlineQuery);

    // append parameters string to the URL
    requestUrl.append( content );
    qDebug() << "requesting accounts:" << requestUrl;
    QNetworkRequest request;
    request.setUrl(QUrl::fromEncoded(requestUrl));
//    request.setUrl(QUrl(requestUrl));

    qDebug() << "getting:" << request.url();

    m_networkReplyBookings = m_network.get(request);

    if(!waitForResponse()) {
        qDebug() << "timeout:" << m_networkReplyBookings->readAll();
        return retList;
    }
    QByteArray response = m_networkReplyBookings->readAll();
//    qDebug() << "got response" << response;
    QJson::Parser parser;
    bool ok = true;
    QVariantMap accountsMap = parser.parse(response, &ok).toMap();
    if(ok) {
        foreach(const QVariant &booking, accountsMap.value("booking").toList()) {
            QVariantMap bookingData = booking.toMap();

            QDateTime reservationTime = QDateTime::fromTime_t(bookingData.value("reservationTime").toMap().value("timeInMillis").toLongLong()/1000);
            GMWVehicle *vehicle = parseVehicle(bookingData.value("vehicle").toMap());
            GMWAccount account(bookingData.value("account").toMap().value("accountId").toInt(), bookingData.value("account").toMap().value("description").toString());
            GMWBooking *gmwbooking = new GMWBooking(bookingData.value("bookingId").toInt(), account);
            gmwbooking->setTime(reservationTime);
            gmwbooking->setExpirationTime(reservationTime.addSecs(15*60));
            vehicle->setBooking(gmwbooking);
            retList.append(vehicle);
            qDebug() << "booking:" << gmwbooking->account().description() << gmwbooking->time() << vehicle->vin();
        }

    }

//    qDebug() << "bookings are:" << retList;
    return retList;
}

bool Car2goEngine::cancelBooking(GMWVehicle *vehicle)
{
    if(!vehicle->booking()->isValid()) {
        qDebug() << "Vehicle does not seem to be booked!";
        return false;
    }
    QByteArray requestUrl("https://www.car2go.com/api/v2.0/booking/");
    requestUrl.append(QString::number(vehicle->booking()->id()));

    QOAuth::ParamMap map;
    // create a request parameters map
    map.insert("format", "json");
    map.insert("bookingId", QString::number(vehicle->booking()->id()).toUtf8());

    // construct the parameters string
    QByteArray content = m_qoauth->createParametersString(requestUrl, QOAuth::DELETE, m_token, m_tokenSecret, QOAuth::HMAC_SHA1, map, QOAuth::ParseForInlineQuery);

    // append parameters string to the URL
    qDebug() << "content:" << content;
    requestUrl.append(content);

    QNetworkRequest request;
    request.setUrl(QUrl::fromEncoded(requestUrl));
//    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    qDebug() << "sending DELETE :" << request.url();//<< "content:" << content;

    QBuffer buffer(&content);
    m_networkReplyCancelBooking = m_network.sendCustomRequest(request, "DELETE");

    if(!waitForResponse()) {
        qDebug() << "timeout:" << m_networkReplyCancelBooking->readAll();
        return false;
    }
    QByteArray response = m_networkReplyCancelBooking->readAll();
    qDebug() << "got response" << response;
    QJson::Parser parser;
    bool ok = true;
    QVariantMap bookingResponseMap = parser.parse(response, &ok).toMap();
    if(ok) {
        if(bookingResponseMap.value("returnValue").toMap().value("code").toInt() != 0) {
            qDebug() << "Booking failed:" << bookingResponseMap.value("returnValue").toMap().value("description").toString();
            return false;
        }
        QVariantList bookingDataList = bookingResponseMap.value("cancelBooking").toList();
        qDebug() << "Cancelling booking successful";// << bookingData;
        foreach(const QVariant &tmp, bookingDataList) {
            qDebug() << tmp;
            QVariantMap bookingData = tmp.toMap();
        }
        vehicle->clearBooking();
        vehicle->setImage(m_imageVehicles);
        return true;
    }
    return false;
}

void Car2goEngine::refreshStationary(bool useCache)
{
    refreshGasStations(useCache);
    refreshParkingSpots(useCache);
}

void Car2goEngine::refreshGasStations(bool useCache)
{
    QFile cacheFile(m_cacheDir + "/gasstations.dat");
    if (useCache && cacheFile.open(QFile::ReadOnly)) {
        qDebug() << "Using cache for gas stations.";

        QFile gasStationsImageFile(m_cacheDir + "/gasstation.png");
        gasStationsImageFile.open(QFile::ReadOnly);
        QDataStream gasStationsImageStream(&gasStationsImageFile);
        gasStationsImageStream >> m_imageGasStations;
        gasStationsImageFile.close();

        QList<GMWItem*> items;
        QDataStream cacheStream(&cacheFile);
        QTime stats; stats.start();
        while (!cacheStream.atEnd()) {
            GMWGasStation *gasStation = new GMWGasStation(QString(), QString(), QGeoCoordinate(), QPixmap());
            cacheStream >> *gasStation;
            gasStation->setImage(m_imageGasStations);
            items.append(gasStation);
        }
        qDebug() << "Loading gas stations from cache took" << stats.elapsed() << "ms";
        if (!items.isEmpty()) {
            emit objectsReceived(items);
            emit loadedFromCache(GMWItem::TypeGasStation, QFileInfo(cacheFile).lastModified());
        }
        cacheFile.close();
    } else {
        qDebug() << "Not using cache for gas stations.";

        // Request GasStations
        if (hasDownloadError()) return;
        delete m_networkReplyGasStations;
        qDebug() << "Downloading gas stations";
        m_networkReplyGasStations = m_network.get(QNetworkRequest("http://www.car2go.com/api/v2.0/gasstations?loc=" + m_locations.value(location())));
        //m_networkReplyGasStations = m_network.get(QNetworkRequest(QUrl("file:///home/fetzerc/gasstations")));
        m_downloads.append(m_networkReplyGasStations);
        if (m_downloads.size()) {
            qDebug() << "Download started!";
            emit downloadStarted();
        }
    }
}

void Car2goEngine::refreshParkingSpots(bool useCache) {
    QFile cacheFile(m_cacheDir + "/parkingspots.dat");
    if(useCache && cacheFile.open(QFile::ReadOnly)) {
        qDebug() << "Using cache for parking spots.";

        QFile parkingLotsImageFile(m_cacheDir + "/parkinglot.png");
        parkingLotsImageFile.open(QFile::ReadOnly);
        QDataStream parkingLotsImageStream(&parkingLotsImageFile);
        parkingLotsImageStream >> m_imageParkingSpots;
        parkingLotsImageFile.close();

        QList<GMWItem*> items;
        QDataStream cacheStream(&cacheFile);
        QTime stats; stats.start();
        while (!cacheStream.atEnd()) {
            GMWParkingSpot *parkingSpot = new GMWParkingSpot(QString(), QString(), QGeoCoordinate(), QPixmap(), 0, 0);
            cacheStream >> *parkingSpot;
            parkingSpot->setImage(m_imageParkingSpots);
            items.append(parkingSpot);
        }
        qDebug() << "Loading parking spots from cache took" << stats.elapsed() << "ms";
        if (!items.isEmpty()) {
            emit objectsReceived(items);
            emit loadedFromCache(GMWItem::TypeParkingLot, QFileInfo(cacheFile).lastModified());
        }
        cacheFile.close();
    } else {
        qDebug() << "Not using cache for parking spots.";

        // Request ParkingSpots
        if (hasDownloadError()) return;
        delete m_networkReplyParkingSpots;
        qDebug() << "Downloading parking spots";
        m_networkReplyParkingSpots = m_network.get(QNetworkRequest("http://www.car2go.com/api/v2.0/parkingspots?loc=" + m_locations.value(location())));
        //m_networkReplyParkingSpots = m_network.get(QNetworkRequest(QUrl("file:///home/fetzerc/parkingspots")));
        m_downloads.append(m_networkReplyParkingSpots);
        if (m_downloads.size()) {
            emit downloadStarted();
        }
    }
}

void Car2goEngine::refreshVehicles(bool useCache)
{
    QFile cacheFile(m_cacheDir + "/vehicles.dat");
    if(useCache && cacheFile.open(QFile::ReadOnly) && (QFileInfo(cacheFile).lastModified().addSecs(3600) > QDateTime::currentDateTime())) {
        qDebug() << "Using cache vehicles.";
        QFile vehicleImageFile(m_cacheDir + "/vehicle.png");
        vehicleImageFile.open(QFile::ReadOnly);
        QDataStream vehicleImageStream(&vehicleImageFile);
        vehicleImageStream >> m_imageVehicles;
        vehicleImageFile.close();
        QList<GMWItem*> items;
        QDataStream cacheStream(&cacheFile);
        QTime stats; stats.start();
        while (!cacheStream.atEnd()) {
            GMWVehicle *vehicle = new GMWVehicle(QString(), QString(), QGeoCoordinate(), QPixmap(), 0, GMWVehicle::StateUnknown, GMWVehicle::StateUnknown, QString());
            cacheStream >> *vehicle;
            vehicle->setImage(m_imageVehicles);
            items.append(vehicle);
        }
        qDebug() << "Loading vehicles from cache took" << stats.elapsed() << "ms";
        if(m_account.isValid()) {
            foreach(GMWVehicle *bookedVehicle, bookings()) {
                qDebug() << "adding booked car:" << bookedVehicle->name() << bookedVehicle->address();

                QImage overlay = m_imageVehicles.toImage();
                QPainter painter(&overlay);
                painter.drawPixmap(overlay.width() - overlay.width() / 1.5, overlay.height() - overlay.height() / 1.5, QPixmap(":/car-booked.png").scaled(overlay.size() / 1.5, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                bookedVehicle->setImage(QPixmap::fromImage(overlay));

                items.append(bookedVehicle);
            }
        }
        if (!items.isEmpty()) {
            emit objectsReceived(items);
            emit loadedFromCache(GMWItem::TypeVehicle, QFileInfo(cacheFile).lastModified());
        }
        cacheFile.close();
    } else {
        qDebug() << "Not using cache for vehicles.";

        // Request Vehicles
        if (hasDownloadError()) return;
        delete m_networkReplyVehicles;
        qDebug() << "Downloading vehicles";
        m_networkReplyVehicles = m_network.get(QNetworkRequest("http://www.car2go.com/api/v2.0/vehicles?loc=" + m_locations.value(location())));
        //m_networkReplyVehicles = m_network.get(QNetworkRequest(QUrl("file:///home/fetzerc/vehicles")));
        m_downloads.append(m_networkReplyVehicles);
        if (m_downloads.size() > 1) {
            emit downloadStarted();
        }
    }
}

GMWBusinessArea Car2goEngine::businessArea()
{
    return m_businessArea;
}

void Car2goEngine::receivedData(QNetworkReply *reply)
{
    qDebug() << "Reply received: " << reply->request().url();
    m_downloads.removeAll(reply);

    if (reply->error() != QNetworkReply::NoError) {
        foreach (QNetworkReply *reply, m_downloads) {
            reply->abort();
            qDebug() << reply->request().url() << "aborted";
        }
        m_downloads.clear();
        qDebug() << "error:" << reply->errorString();
        emit downloadFinished(GMWItem::TypeUnknown, false, reply->errorString());
        return;
    }

    if (reply == m_networkReplyParkingSpots) {
        m_xmlParkingSpots = QString::fromUtf8(reply->readAll());
        parseImage(ParkingSpotsReply, m_xmlParkingSpots);
    } else if (reply == m_networkReplyGasStations) {
        m_xmlGasStations = QString::fromUtf8(reply->readAll());
        parseImage(GasStationReply, m_xmlGasStations);
    } else if (reply == m_networkReplyVehicles) {
        m_xmlVehicles = QString::fromUtf8(reply->readAll());
        parseImage(VehiclesReply, m_xmlVehicles);
    } else if (reply == m_networkReplyParkingSpotsImage) {
        m_imageParkingSpots.loadFromData(reply->readAll());
        m_imageParkingSpots = m_imageParkingSpots.scaledToHeight(48, Qt::SmoothTransformation );
        parse(ParkingSpotsReply, m_xmlParkingSpots);
    } else if (reply == m_networkReplyGasStationsImage) {
        m_imageGasStations.loadFromData(reply->readAll());
        m_imageGasStations = m_imageGasStations.scaledToHeight(48, Qt::SmoothTransformation );
        parse(GasStationReply, m_xmlGasStations);
    } else if (reply == m_networkReplyVehiclesImage) {
        m_imageVehicles.loadFromData(reply->readAll());
        m_imageVehicles = m_imageVehicles.scaledToHeight(48, Qt::SmoothTransformation );
        parse(VehiclesReply, m_xmlVehicles);
    } else if (reply == m_networkReplyAccounts || reply == m_networkReplyBookings || reply == m_networkReplyCreateBooking || reply == m_networkReplyCancelBooking) {
        m_timeout = false;
        m_loop.quit();
    } else {
        qDebug() << "Error: Unknown data received.";
    }
}

void Car2goEngine::parseImage(NetworkReplyType type, const QString &xmlData)
{
    QXmlStreamReader xml(xmlData);
    while (!xml.atEnd()) {
        xml.readNext();
        //qDebug() << xml.tokenString() << " " << xml.name();
        if (xml.tokenType() != QXmlStreamReader::StartElement) continue;
        if (xml.name() == "IconStyle") {
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "IconStyle")) {
                xml.readNext();
                //qDebug() << xml.tokenString() << " " << xml.name();
                if (xml.tokenType() != QXmlStreamReader::StartElement) continue;
                if (xml.name() == "Icon") {
                    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Icon")) {
                        xml.readNext();
                        //qDebug() << xml.tokenString() << " " << xml.name();
                        if (xml.tokenType() != QXmlStreamReader::StartElement) continue;
                        if (xml.name() == "href") {
                            xml.readNext();
                            //qDebug() << xml.text().toString();
                            if (!hasDownloadError()) {
                                if (type == ParkingSpotsReply) {
                                    delete m_networkReplyParkingSpotsImage;
                                    m_networkReplyParkingSpotsImage = m_network.get(QNetworkRequest(xml.text().toString()));
                                    m_downloads.append(m_networkReplyParkingSpotsImage);
                                } else if (type == GasStationReply) {
                                    delete m_networkReplyGasStationsImage;
                                    m_networkReplyGasStationsImage = m_network.get(QNetworkRequest(xml.text().toString()));
                                    m_downloads.append(m_networkReplyGasStationsImage);
                                } else if (type == VehiclesReply) {
                                    delete m_networkReplyVehiclesImage;
                                    m_networkReplyVehiclesImage = m_network.get(QNetworkRequest(xml.text().toString()));
                                    m_downloads.append(m_networkReplyVehiclesImage);
                                } else {
                                    qDebug() << "Error: Unknown image requestet.";
                                }
                            }
                            return;
                        }
                    }
                }
            }
        }
    }
}

void Car2goEngine::parse(NetworkReplyType type, const QString &xmlData)
{
    QString name;
    QString description;
    QStringList coordinates;
    QGeoCoordinate location;
    QMap<QString, QString> extendedData;
    QString extendedDataName;

    QList<GMWItem*> parsedItems;
    QTime stats; stats.start();

    QXmlStreamReader xml(xmlData);
    while (!xml.atEnd()) {
        xml.readNext();
        //qDebug() << xml.tokenString() << " " << xml.name();

        // Placemark
        if (xml.name() == "Placemark") {

            // name, description, Point, ExtendedData
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Placemark")) {
                xml.readNext();
                //qDebug() << xml.tokenString() << " " << xml.name();

                if (xml.tokenType() != QXmlStreamReader::StartElement) continue;
                if (xml.name() == "name") {
                    xml.readNext();
                    name = xml.text().toString();
                } else if (xml.name() == "description") {
                    xml.readNext();
                    description = xml.text().toString();
                } else if (xml.name() == "Point") {
                    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Point")) {
                        xml.readNext();
                        //qDebug() << xml.tokenString() << " " << xml.name();

                        if (xml.tokenType() != QXmlStreamReader::StartElement) continue;
                        if (xml.name() == "coordinates") {
                            xml.readNext();
                            coordinates = xml.text().toString().split(",");
                            location = QGeoCoordinate(coordinates[1].toDouble(), coordinates[0].toDouble(), coordinates[2].toDouble());
                        }
                    }
                } else if (xml.name() == "ExtendedData") {
                    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "ExtendedData")) {
                        xml.readNext();
                        //qDebug() << xml.tokenString() << " " << xml.name();

                        if (xml.tokenType() != QXmlStreamReader::StartElement) continue;
                        if (xml.name() == "Data") {
                            extendedDataName = xml.attributes().value("name").toString();
                            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Data")) {
                                xml.readNext();
                                //qDebug() << xml.tokenString() << " " << xml.name();

                                if (xml.tokenType() != QXmlStreamReader::StartElement) continue;
                                if (xml.name() == "value") {
                                    xml.readNext();
                                    extendedData.insert(extendedDataName, xml.text().toString());
                                    //qDebug() << extendedDataName << xml.text().toString();
                                }
                            }
                        }
                    }
                }
            }

            //qDebug() << name << description << location;

            if (type == ParkingSpotsReply) {
                GMWParkingSpot *parkingSpot = new GMWParkingSpot(
                    "car2go Parking Spot",
                    name,
                    location,
                    m_imageParkingSpots,
                    extendedData.value("usedCapacity").toInt(),
                    extendedData.value("totalCapacity").toInt()
                );
                parsedItems.append(parkingSpot);
            } else if (type == GasStationReply) {
                GMWGasStation *gasStation = new GMWGasStation(
                    "Shell car2go Partner Gas Station",
                    name,
                    location,
                    m_imageGasStations
                );
                parsedItems.append(gasStation);
            } else if (type == VehiclesReply) {
                //qDebug() << description.section("<br/>", 0, 0);
                GMWVehicle *vehicle = new GMWVehicle(
                    name,
                    description.section("<br/>", 0, 0),
                    location,
                    m_imageVehicles,
                    extendedData.value("fuel").toInt(),
                    GMWVehicle::vehicleStateFromString(extendedData.value("interior")),
                    GMWVehicle::vehicleStateFromString(extendedData.value("exterior")),
                    extendedData.value("vin")
                );
                parsedItems.append(vehicle);
            } else {
                qDebug() << "Error: Unknown object received.";
            }
        }
    }

    if (!parsedItems.isEmpty()) {
        qDebug() << "Parsing of" << parsedItems.count() << "items took" << stats.restart() << "ms";

        // Write the parsed item to the cache
        QString cacheFileName;
        if (type == GasStationReply) {
            cacheFileName = "gasstations.dat";
        } else if (type == ParkingSpotsReply) {
            cacheFileName = "parkingspots.dat";
        } else if (type == VehiclesReply) {
            cacheFileName = "vehicles.dat";
        } else {
            qDebug() << "Error: Unknown object received.";
        }
        QFile cacheFile(m_cacheDir + "/" + cacheFileName);
        qDebug() << "Writing" << parsedItems.count() << "items to cachefile:" << cacheFile.fileName();
        cacheFile.open(QFile::WriteOnly);
        QDataStream cacheStream(&cacheFile);
        foreach (GMWItem* item, parsedItems) {
            if (type == GasStationReply) {
                cacheStream << dynamic_cast<GMWGasStation&>(*item);
            } else if (type == ParkingSpotsReply) {
                cacheStream << dynamic_cast<GMWParkingSpot&>(*item);
            } else if (type == VehiclesReply) {
                cacheStream << dynamic_cast<GMWVehicle&>(*item);
            }
        }
        cacheFile.close();
        // writing images to disk
        QFile vehicleImageFile(m_cacheDir + "/vehicle.png");
        vehicleImageFile.open(QFile::WriteOnly);
        QDataStream vehicleImageStream(&vehicleImageFile);
        vehicleImageStream << m_imageVehicles;
        vehicleImageFile.close();
        QFile parkingLotsImageFile(m_cacheDir + "/parkinglot.png");
        parkingLotsImageFile.open(QFile::WriteOnly);
        QDataStream parkingLotsImageStream(&parkingLotsImageFile);
        parkingLotsImageStream << m_imageParkingSpots;
        parkingLotsImageFile.close();
        QFile gasStationsImageFile(m_cacheDir + "/gasstation.png");
        gasStationsImageFile.open(QFile::WriteOnly);
        QDataStream gasStationsImageStream(&gasStationsImageFile);
        gasStationsImageStream << m_imageGasStations;
        gasStationsImageFile.close();
        qDebug() << "Writing to cache took" << stats.elapsed() << "ms";

        // items fetched. add booked items
        if(m_account.isValid()) {
            foreach(GMWVehicle *bookedVehicle, bookings()) {
                qDebug() << "adding booked vehicle:" << bookedVehicle->vin() << bookedVehicle->address() << bookedVehicle->location();

                QImage overlay = m_imageVehicles.toImage();
                QPainter painter(&overlay);
                painter.drawPixmap(overlay.width() - overlay.width() / 1.5, overlay.height() - overlay.height() / 1.5, QPixmap(":/car-booked.png").scaled(overlay.size() / 1.5, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                bookedVehicle->setImage(QPixmap::fromImage(overlay));

                parsedItems.append(bookedVehicle);
            }
        }

        // Notify Map and List
        emit objectsReceived(parsedItems);
    }

    GMWItem::Type itemType = GMWItem::TypeUnknown;
    switch(type) {
    case ParkingSpotsReply:
        itemType = GMWItem::TypeParkingLot;
        break;
    case GasStationReply:
        itemType = GMWItem::TypeGasStation;
        break;
    case VehiclesReply:
        itemType = GMWItem::TypeVehicle;
        break;
    case IconReply:
        itemType = GMWItem::TypeUnknown;
    }

    if (xml.hasError()) {
        qDebug() << "Xml has errors!";
        emit downloadFinished(itemType, false, tr("Downloaded Data contains errors"));
    } else {
        // Emit the finished signal only if this is the last download in progress.
        if (m_downloads.size() == 0) {
            emit downloadFinished(itemType, true, "");
        }
    }
}

bool Car2goEngine::hasDownloadError() const
{
    foreach (QNetworkReply *reply, m_downloads) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Download has error" << reply->errorString();
            return true;
        }
    }
    return false;
}

void Car2goEngine::loadBusinessArea()
{
    QFile file("/usr/share/getmewheels/data/" + m_locations.value(location()) + ".gpx");
    qDebug() << "loading" << file.fileName();
    file.open(QIODevice::ReadOnly);
    QXmlStreamReader xml(&file);

    QList<Area> areaList;
    QList<Area> excludeList;
    while(!xml.atEnd()) {
        xml.readNext();
        qDebug() << xml.tokenString() << " " << xml.name();

        // Track
        if (xml.name() == "trk") {
            xml.readNext();

            QList<QGeoCoordinate> path;

            bool exclude = false;
            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "trk")) {
                  xml.readNext();
//                  qDebug() << xml.tokenString() << " " << xml.name();
                  if(xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "name") {
                      xml.readNext();
//                      qDebug() << "got track" << xml.text();
                      if(xml.text().toString().startsWith("Exclude_")) {
                          exclude = true;
                      } else {
                          exclude = false;
                      }
                  }
                  if(xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "trkseg") {
                      xml.readNext();
//                      qDebug() << "got track segment:" << xml.text();
                      while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "trkseg")) {
                          xml.readNext();
                          if(xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "trkpt") {
                              QGeoCoordinate coord;
                              coord.setLongitude(xml.attributes().value("lon").toString().toDouble());
                              coord.setLatitude(xml.attributes().value("lat").toString().toDouble());
                              path.append(coord);
                          }
                      }
                  }

            }

            Area area;
            area.setPath(path);
//            qDebug() << "appending path" << path;
            if(!exclude) {
                areaList.append(area);
            } else {
                excludeList.append(area);
            }
        }

    }
    m_businessArea.setAreaList(areaList);
    m_businessArea.setExcludes(excludeList);
    emit businessAreaChanged();
}

bool Car2goEngine::waitForResponse()
{
    m_timeout = true;
    QTimer::singleShot(5000, &m_loop, SLOT(quit()));
    m_loop.exec();
    return !m_timeout;
}

GMWVehicle* Car2goEngine::parseVehicle(const QVariantMap &vehicleMap)
{
    QString name = vehicleMap.value("numberPlate").toString();
    QString address = vehicleMap.value("position").toMap().value("address").toString();
    QGeoCoordinate position;
    position.setLatitude(vehicleMap.value("position").toMap().value("latitude").toDouble());
    position.setLongitude(vehicleMap.value("position").toMap().value("longitude").toDouble());
    quint8 fuel = vehicleMap.value("fuel").toInt();
    GMWVehicle::State interiorState = GMWVehicle::vehicleStateFromString(vehicleMap.value("interior").toString());
    GMWVehicle::State exteriorState = GMWVehicle::vehicleStateFromString(vehicleMap.value("exterior").toString());
    QString vin = vehicleMap.value("vin").toString();
    return new GMWVehicle(name, address, position, m_imageVehicles, fuel, interiorState, exteriorState, vin);
}
