/*****************************************************************************
 * Copyright: 2010-2011 Christian Fetzer <fetzer.ch@googlemail.com>          *
 * Copyright: 2010-2012 Michael Zanetti <michael_zanetti@mgx.net>            *
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
#include "engines/location.h"

#ifdef Q_WS_S60
#include <qjson/src/parser.h>
#elif QT_VERSION < 0x050000
#include <qjson/parser.h>
#else
#include <QJsonDocument>
#endif

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
    m_currentDownloadLocation(NULL)
{
    connect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(receivedData(QNetworkReply*)));

#if !defined Q_WS_S60 && QT_VERSION < 0x050000
    // Init qoauth
    m_qoauth = new QOAuth::Interface( this );
    m_qoauth->setConsumerKey( ConsumerKey );
    m_qoauth->setConsumerSecret( ConsumerSecret );

#else
    m_oauthManager = new KQOAuthManager(this);
    m_oauthManager->setHandleUserAuthorization(false);

    connect(m_oauthManager, SIGNAL(receivedToken(QString,QString)), SLOT(receivedToken(QString,QString)));
    connect(m_oauthManager, SIGNAL(temporaryTokenReceived(QString,QString)), this, SLOT(temporaryTokenReceived(QString, QString)));
    connect(m_oauthManager, SIGNAL(authorizationReceived(QString,QString)), this, SLOT(authorizationReceived(QString, QString)));
    connect(m_oauthManager, SIGNAL(accessTokenReceived(QString,QString)), this, SLOT(accessTokenReceived(QString,QString)));
    connect(m_oauthManager, SIGNAL(requestReady(QByteArray)), this, SLOT(requestReady(QByteArray)));
    connect(m_oauthManager, SIGNAL(authorizedRequestDone()), SLOT(authorizedRequestDone()));

    m_oauthRequest = new KQOAuthRequest(this);
#endif

#if QT_VERSION < 0x050000
    QSettings settings("getmewheels", "getmewheels");
#else
    QSettings settings("com.ubuntu.developer.mzanetti.getmewheels2", "getmewheels2");
#endif
    settings.beginGroup("car2go");
    if(settings.contains("OAuthAccessToken")) {
        m_account = GMWAccount(settings.value("OAuthAccountID").toInt(), settings.value("OAuthAccountDescription").toString());
        m_token = settings.value("OAuthAccessToken").toByteArray();
        m_tokenSecret = settings.value("OAuthAccessTokenSecret").toByteArray();

        qDebug() << "loaded token:" << m_token << "secret:" << m_tokenSecret;
    }

    m_imageGasStations = QPixmap(":qml/getmewheels2/harmattan/images/gas_map.png");
    m_imageParkingSpots = QPixmap(":qml/getmewheels2/harmattan/images/parking_map.png");
    m_imageParkingSpotsCP = QPixmap(":qml/getmewheels2/harmattan/images/parking_cp_map.png");
    m_imageVehicles = QPixmap(":qml/getmewheels2/harmattan/images/car_map.png");
    m_imageVehiclesED = QPixmap(":qml/getmewheels2/harmattan/images/car_ed_map.png");
    m_imageGasStationsL = QPixmap(":qml/getmewheels2/harmattan/images/gas_map_l.png");
    m_imageParkingSpotsL = QPixmap(":qml/getmewheels2/harmattan/images/parking_map_l.png");
    m_imageParkingSpotsCPL = QPixmap(":qml/getmewheels2/harmattan/images/parking_cp_map_l.png");
    m_imageVehiclesL = QPixmap(":qml/getmewheels2/harmattan/images/car_map_l.png");
    m_imageVehiclesEDL = QPixmap(":qml/getmewheels2/harmattan/images/car_ed_map_l.png");
}

QList<Location*> Car2goEngine::fetchLocations()
{
    QList<Location*> retList;

    QByteArray requestUrl( "https://www.car2go.com/api/v2.1/locations?oauth_consumer_key=GetMeWheels&format=json");
    qDebug() << "requesting locations:" << requestUrl;
    QNetworkRequest request;
    request.setUrl(QUrl::fromEncoded(requestUrl));

    qDebug() << "getting:" << request.url();


    QNetworkReply *reply = m_network.get(request);

    if(!waitForResponse()) {
        qDebug() << "timeout:" << reply->readAll();
        return retList;
    }
    QByteArray response = reply->readAll();
    qDebug() << "got response" << response;
    QVariantMap locationMap = parseJson(response);

    foreach(const QVariant &location, locationMap.value("location").toList()) {
        Location *l = new Location();

        l->setId(location.toMap().value("locationId").toInt());
        l->setName(location.toMap().value("locationName").toString());

        // workaround for broken start coordinates in car2go api
        switch(l->id()) {
        case 1: // Ulm
            l->setArea(QGeoRectangle(QGeoCoordinate(48.41000, 9.96000), QGeoCoordinate(48.39000, 10.00000)));
            break;
        case 2: // Austin
            l->setArea(QGeoRectangle(QGeoCoordinate(30.29000, -97.77000), QGeoCoordinate(30.25000, -97.70000)));
            break;
        case 3: //Hamburg
            l->setArea(QGeoRectangle(QGeoCoordinate(53.72000, 9.84000), QGeoCoordinate(53.41300, 10.15500)));
            break;
        case 4: // Vancouver
            l->setArea(QGeoRectangle(QGeoCoordinate(49.314789, -123.237269), QGeoCoordinate(49.230886, -123.062995)));
            break;
        case 5: // Amsterdam
            l->setArea(QGeoRectangle(QGeoCoordinate(52.396896, 4.853783), QGeoCoordinate(52.339317, 4.949881)));
            break;
        case 7: // Wien
            l->setArea(QGeoRectangle(QGeoCoordinate(48.303924, 16.240785), QGeoCoordinate(48.133475, 16.532411)));
            break;
        case 8: // San Diego
            l->setArea(QGeoRectangle(QGeoCoordinate(32.80715, -117.250194), QGeoCoordinate(32.707689, -117.117355)));
            break;
        case 9: // Washington DC
            l->setArea(QGeoRectangle(QGeoCoordinate(38.978878, -77.165943), QGeoCoordinate(38.810696, -76.919818)));
            break;
        default: {
            QVariantMap mapSection = location.toMap().value("mapSection").toMap();
            QGeoCoordinate upperLeft;
            upperLeft.setLatitude(mapSection.value("upperLeft").toMap().value("latitude").toDouble());
            upperLeft.setLongitude(mapSection.value("upperLeft").toMap().value("longitude").toDouble());

            QGeoCoordinate lowerRight;
            lowerRight.setLatitude(mapSection.value("lowerRight").toMap().value("latitude").toDouble());
            lowerRight.setLongitude(mapSection.value("lowerRight").toMap().value("longitude").toDouble());

            QGeoRectangle area;
            area.setTopLeft(upperLeft);
            area.setBottomRight(lowerRight);
            l->setArea(area);
        }
        }

        qDebug() << "got location" << l->id() << l->name() << l->area().topLeft() << l->area().bottomRight();

        retList.append(l);
    }

    return retList;
}

void Car2goEngine::fetchGasStations(Location *location)
{
    qDebug() << "Downloading gas stations";
    m_networkReplyGasStations = m_network.get(QNetworkRequest("https://www.car2go.com/api/v2.1/gasstations?loc=" + location->name() + "&oauth_consumer_key=" + Car2goEngine::ConsumerKey + "&format=json"));

}

void Car2goEngine::fetchParkingSpots(Location *location)
{
    qDebug() << "Downloading parking spots";
    m_networkReplyParkingSpots = m_network.get(QNetworkRequest("https://www.car2go.com/api/v2.1/parkingspots?loc=" + location->name() + "&oauth_consumer_key=" + Car2goEngine::ConsumerKey + "&format=json"));
}

void Car2goEngine::fetchVehicles(Location *location)
{
    qDebug() << "Downloading vehicles for" << location->name();
    m_currentDownloadLocation = location;
    m_networkReplyVehicles = m_network.get(QNetworkRequest("https://www.car2go.com/api/v2.1/vehicles?loc=" + location->name() + "&oauth_consumer_key=" + Car2goEngine::ConsumerKey + "&format=json"));

}

QList<GMWAccount> Car2goEngine::accounts(Location *location)
{
    QList<GMWAccount> retList;

#if !defined Q_WS_S60 && QT_VERSION < 0x050000
    QByteArray requestUrl( "https://www.car2go.com/api/v2.0/accounts");

    QOAuth::ParamMap map;
    // create a request parameters map
    map.insert("loc", location->name().toUtf8());
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

    QNetworkReply *reply = m_network.get(request);

    if(!waitForResponse()) {
        qDebug() << "timeout:" << reply->readAll();
        return retList;
    }
    QByteArray response = reply->readAll();
#else

    m_oauthRequest->clearRequest();
    m_oauthRequest->initRequest(KQOAuthRequest::AuthorizedRequest, QUrl("https://www.car2go.com/api/v2.0/accounts"));
    m_oauthRequest->setConsumerKey(Car2goEngine::ConsumerKey);
    m_oauthRequest->setConsumerSecretKey(Car2goEngine::ConsumerSecret);
    m_oauthRequest->setToken(m_token);
    m_oauthRequest->setTokenSecret(m_tokenSecret);
    m_oauthRequest->setHttpMethod(KQOAuthRequest::GET);
    m_oauthRequest->setEnableDebugOutput(false);
    KQOAuthParameters params;
    params.insert("format", "json");
    params.insert("loc", location->name().toUtf8());
    m_oauthRequest->setAdditionalParameters(params);


    m_oauthManager->executeRequest(m_oauthRequest);
    qDebug() << "requsting" << m_oauthRequest->requestBody() << m_oauthRequest->requestParameters();

    if(!waitForResponse()) {
        qDebug() << "getting accounts timed out";
        return retList;
    }

    QByteArray response = m_lastData;

#endif
    qDebug() << "got response" << response;
    QVariantMap accountsMap = parseJson(response);

    if (accountsMap.contains("account")) {
    foreach(const QVariant &account, accountsMap.value("account").toList()) {
        retList.append(GMWAccount(account.toMap().value("accountId").toInt(), account.toMap().value("description").toString()));
        qDebug() << "got account:" << retList.last().id() << retList.last().description();
    }

}

    return retList;

}

void Car2goEngine::setAccount(const GMWAccount &account)
{
    if(m_account != account) {
        m_account = account;
#if QT_VERSION < 0x050000
        QSettings settings("getmewheels", "getmewheels");
#else
        QSettings settings("com.ubuntu.developer.mzanetti.getmewheels2", "getmewheels2");
#endif
        settings.beginGroup("car2go");
        settings.setValue("OAuthAccountID", m_account.id());
        settings.setValue("OAuthAccountDescription", m_account.description());
    }
}

GMWAccount Car2goEngine::account()
{
    return m_account;
}

bool Car2goEngine::startAuthentication()
{
#if !defined Q_WS_S60 && QT_VERSION < 0x050000
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

//    QDesktopServices::openUrl(QUrl::fromEncoded(url.toUtf8()));
    emit authUrlReceived(url);

    return true;

#else
//    const char *request_token_uri =  "https://www.car2go.com/api/reqtoken";
//      const char *access_token_uri = "https://www.car2go.com/api/accesstoken";
//      const char *test_call_uri =    "https://www.car2go.com/api/authorize";
//      const char *c_key         = "GetMeWheels";
//      const char *c_secret      = "qleDqQghx5lPelzT";
//    char *postarg = NULL;
//    char *req_url;
//    req_url = oauth_sign_url2(request_token_uri, &postarg, OA_HMAC, NULL, c_key, c_secret, NULL, NULL);

//    qDebug() << "*************************Oauth init:" << req_url;

    m_oauthRequest->initRequest(KQOAuthRequest::TemporaryCredentials, QUrl(Car2goEngine::Car2GoRequestTokenURL));
    m_oauthRequest->setHttpMethod(KQOAuthRequest::POST);
    m_oauthRequest->setEnableDebugOutput(false);
    m_oauthRequest->setConsumerKey(Car2goEngine::ConsumerKey);
    m_oauthRequest->setConsumerSecretKey(Car2goEngine::ConsumerSecret);
    m_oauthRequest->setCallbackUrl(QUrl(Car2goEngine::ParamCallbackValue));

    m_oauthManager->executeRequest(m_oauthRequest);

    return true;
#endif
}

bool Car2goEngine::setAccessCode(const QString &code)
{
#if !defined Q_WS_S60 && QT_VERSION < 0x050000
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

    return true;

#else
    m_oauthRequest->clearRequest();
    m_oauthRequest->initRequest(KQOAuthRequest::AccessToken, QUrl(Car2goEngine::Car2GoAccessTokenURL));
    m_oauthRequest->setVerifier(code);
    m_oauthRequest->setHttpMethod(KQOAuthRequest::POST);
    m_oauthRequest->setConsumerKey(Car2goEngine::ConsumerKey);
    m_oauthRequest->setConsumerSecretKey(Car2goEngine::ConsumerSecret);
    m_oauthRequest->setToken(m_temporaryToken);
    m_oauthRequest->setTokenSecret(m_temporaryTokenSecret);
    m_oauthRequest->setEnableDebugOutput(false);
    //m_oauthRequest->setCallbackUrl(QUrl(Car2goEngine::ParamCallbackValue));

    m_oauthManager->executeRequest(m_oauthRequest);

    if(!waitForResponse()) {
        qDebug() << "getting oauth access token timed out...";
        return false;
    }
    return m_oauthManager->lastError() == 0;

#endif
}

bool Car2goEngine::authenticated()
{
    return !m_token.isEmpty() && !m_tokenSecret.isEmpty();
}

void Car2goEngine::removeAuthentication()
{
    m_token.clear();
    m_tokenSecret.clear();

#if QT_VERSION < 0x050000
    QSettings settings("getmewheels", "getmewheels");
#else
    QSettings settings("com.ubuntu.developer.mzanetti.getmewheels2", "getmewheels2");
#endif
    settings.beginGroup("car2go");
    settings.setValue("OAuthAccessToken", m_token);
    settings.setValue("OAuthAccessTokenSecret", m_tokenSecret);
    settings.remove("AuthExpirationDate");
}

QDateTime Car2goEngine::authExpirationDate()
{
#if QT_VERSION < 0x050000
    QSettings settings("getmewheels", "getmewheels");
#else
    QSettings settings("com.ubuntu.developer.mzanetti.getmewheels2", "getmewheels2");
#endif
    settings.beginGroup("car2go");
    return settings.value("AuthExpirationDate").toDateTime();
}

bool Car2goEngine::createBooking(Location * location, GMWVehicle *vehicle, const GMWAccount &account)
{
//    qDebug() << "Faking failing booking request";
//    m_error = "Car id invalid.";
//    return false;

//    qDebug() << "Faking booking request";
//    GMWBooking *booking = new GMWBooking(1, account);
//    booking->setTime(QDateTime::currentDateTime());
//    booking->setExpirationTime(QDateTime::currentDateTime().addSecs(60));
//    vehicle->setBooking(booking);
//    return true;


#if !defined Q_WS_S60 && QT_VERSION < 0x050000
    QByteArray requestUrl("https://www.car2go.com/api/v2.1/bookings");

    QOAuth::ParamMap map;
    // create a request parameters map
    map.insert("format", "json");
    map.insert("loc", location->name().toUtf8());
    map.insert("vin", vehicle->vin().toUtf8());
    if(account.isValid()) {
        map.insert("account", QString::number(account.id()).toUtf8());
    } else {
        map.insert("account", QString::number(m_account.id()).toUtf8());
    }

    // construct the parameters string
    QByteArray content = m_qoauth->createParametersString(requestUrl, QOAuth::POST, m_token, m_tokenSecret, QOAuth::HMAC_SHA1, map, QOAuth::ParseForRequestContent);

    // append parameters string to the URL
//    requestUrl.append( content );
    qDebug() << "requesting accounts:" << requestUrl;
    QNetworkRequest request;
    request.setUrl(QUrl::fromEncoded(requestUrl));
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    qDebug() << "postin:" << request.url() << "content:" << content;

    QNetworkReply *reply = m_network.post(request, content);

    if(!waitForResponse()) {
        m_error = reply->errorString();
        qDebug() << "timeout:" << reply->readAll() << "error:" << m_error;
        return false;
    }
    QByteArray response = reply->readAll();
#else
    m_oauthRequest->clearRequest();
    m_oauthRequest->initRequest(KQOAuthRequest::AuthorizedRequest, QUrl("https://www.car2go.com/api/v2.1/bookings"));
    m_oauthRequest->setConsumerKey(Car2goEngine::ConsumerKey);
    m_oauthRequest->setConsumerSecretKey(Car2goEngine::ConsumerSecret);
    m_oauthRequest->setToken(m_token);
    m_oauthRequest->setTokenSecret(m_tokenSecret);
    m_oauthRequest->setHttpMethod(KQOAuthRequest::POST);
    m_oauthRequest->setEnableDebugOutput(false);
    KQOAuthParameters params;
    params.insert("format", "json");
    params.insert("loc", location->name().toUtf8());
    params.insert("vin", vehicle->vin().toUtf8());
    if(account.isValid()) {
        params.insert("account", QString::number(account.id()).toUtf8());
    } else {
        params.insert("account", QString::number(m_account.id()).toUtf8());
    }

    m_oauthRequest->setAdditionalParameters(params);


    m_oauthManager->executeRequest(m_oauthRequest);

    if(!waitForResponse()) {
        qDebug() << "creating booking timed out";
        return false;
    }

    QByteArray response = m_lastData;
#endif
    qDebug() << "got response" << response;

    QVariantMap bookingResponseMap = parseJson(response);
    if (bookingResponseMap.isEmpty()) {
        return false;
    }

    if(bookingResponseMap.value("returnValue").toMap().value("code").toInt() != 0) {
        m_error = bookingResponseMap.value("returnValue").toMap().value("description").toString();
        qDebug() << "Booking failed:" << m_error;
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
}

QList<GMWVehicle*> Car2goEngine::bookings(Location *location)
{
    QList<GMWVehicle*> retList;

#if !defined Q_WS_S60 && QT_VERSION < 0x050000
    QByteArray requestUrl( "https://www.car2go.com/api/v2.1/bookings");

    QOAuth::ParamMap map;
    // create a request parameters map
    map.insert("loc", location->name().toUtf8());
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

    QNetworkReply *reply = m_network.get(request);

    if(!waitForResponse()) {
        qDebug() << "timeout:" << reply->readAll();
        return retList;
    }
    QByteArray response = reply->readAll();
//    qDebug() << "got response" << response;
#else
    KQOAuthRequest *req = new KQOAuthRequest();
    req->clearRequest();
    req->initRequest(KQOAuthRequest::AuthorizedRequest, QUrl("https://www.car2go.com/api/v2.1/bookings"));
    req->setConsumerKey(Car2goEngine::ConsumerKey);
    req->setConsumerSecretKey(Car2goEngine::ConsumerSecret);
    req->setToken(m_token);
    req->setTokenSecret(m_tokenSecret);
    req->setHttpMethod(KQOAuthRequest::GET);
    req->setEnableDebugOutput(true);
    KQOAuthParameters params;
    params.insert("format", "json");
    params.insert("loc", location->name().toUtf8());
    req->setAdditionalParameters(params);


    m_oauthManager->executeRequest(req);
//    qDebug() << "requsting bookings" << m_oauthRequest->requestBody() << m_oauthRequest->requestParameters();

    if(!waitForResponse()) {
        qDebug() << "getting bookings timed out";
        return retList;
    }
    delete req;

    QByteArray response = m_lastData;

#endif
    qDebug() << "got response:" << response;
    QVariantMap accountsMap = parseJson(response);

    foreach(const QVariant &booking, accountsMap.value("booking").toList()) {
        QVariantMap bookingData = booking.toMap();

        QDateTime reservationTime = QDateTime::fromTime_t(bookingData.value("reservationTime").toMap().value("timeInMillis").toLongLong()/1000);
        GMWVehicle *vehicle = parseBookedVehicle(bookingData.value("vehicle").toMap());
        GMWAccount account(bookingData.value("account").toMap().value("accountId").toInt(), bookingData.value("account").toMap().value("description").toString());
        GMWBooking *gmwbooking = new GMWBooking(bookingData.value("bookingId").toInt(), account);
        gmwbooking->setTime(reservationTime);
        gmwbooking->setExpirationTime(reservationTime.addSecs(15*60));
        vehicle->setBooking(gmwbooking);
        retList.append(vehicle);
        qDebug() << "booking:" << gmwbooking->account().description() << gmwbooking->time() << vehicle->vin();
    }

    qDebug() << "bookings are:" << retList;

    return retList;
}

bool Car2goEngine::cancelBooking(GMWVehicle *vehicle)
{
#if !defined Q_WS_S60 && QT_VERSION < 0x050000
    if(!vehicle->booking()->isValid()) {
        qDebug() << "Vehicle does not seem to be booked!";
        return false;
    }
    QByteArray requestUrl("https://www.car2go.com/api/v2.1/booking/");
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
    QNetworkReply *reply = m_network.sendCustomRequest(request, "DELETE");

    if(!waitForResponse()) {
        qDebug() << "timeout:" << reply->readAll();
        return false;
    }
    QByteArray response = reply->readAll();
#else
    m_oauthRequest->clearRequest();
    m_oauthRequest->initRequest(KQOAuthRequest::AuthorizedRequest, QUrl("https://www.car2go.com/api/v2.1/booking/" + QString::number(vehicle->booking()->id())));
    m_oauthRequest->setConsumerKey(Car2goEngine::ConsumerKey);
    m_oauthRequest->setConsumerSecretKey(Car2goEngine::ConsumerSecret);
    m_oauthRequest->setToken(m_token);
    m_oauthRequest->setTokenSecret(m_tokenSecret);
    m_oauthRequest->setHttpMethod(KQOAuthRequest::DELETE);
    m_oauthRequest->setEnableDebugOutput(false);
    KQOAuthParameters params;
    params.insert("format", "json");
    params.insert("bookingId", QString::number(vehicle->booking()->id()).toUtf8());
    m_oauthRequest->setAdditionalParameters(params);


    m_oauthManager->executeRequest(m_oauthRequest);

    if(!waitForResponse()) {
        qDebug() << "cancelling booking timed out";
        return false;
    }

    QByteArray response = m_lastData;

#endif
    qDebug() << "got response" << response;

    QVariantMap bookingResponseMap = parseJson(response);
    if(bookingResponseMap.isEmpty()) {
        return false;
    }

    if(bookingResponseMap.value("returnValue").toMap().value("code").toInt() != 0) {
        m_error = bookingResponseMap.value("returnValue").toMap().value("description").toString();
        qDebug() << "Booking failed:" << m_error;
        return false;
    }
    QVariantList bookingDataList = bookingResponseMap.value("cancelBooking").toList();
    qDebug() << "Cancelling booking successful";// << bookingData;
    foreach(const QVariant &tmp, bookingDataList) {
        qDebug() << tmp;
        QVariantMap bookingData = tmp.toMap();
    }
    vehicle->clearBooking();
    return true;
}

void Car2goEngine::receivedData(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "error:" << reply->errorString();
        return;
    }

    if (reply == m_networkReplyParkingSpots) {
        QByteArray response = reply->readAll();
        qDebug() << "received parking spots reply";
        parseParkingSpots(parseJson(response));

    } else if (reply == m_networkReplyGasStations) {
        QByteArray response = reply->readAll();
        qDebug() << "received gas stations reply";
        parseGasStations(parseJson(response));
    } else if (reply == m_networkReplyVehicles) {
        QByteArray response = reply->readAll();
        qDebug() << "received vehicles reply";
        parseVehicles(parseJson(response));
    } else {
        m_timeout = false;
        qDebug() << "stopping loop";
        m_loop.quit();
    }
}


void Car2goEngine::loadBusinessArea()
{
//    QFile file; //("/usr/share/getmewheels/data/" + m_locations.value(location()) + ".gpx");
//    qDebug() << "loading" << file.fileName();
//    file.open(QIODevice::ReadOnly);
//    QXmlStreamReader xml(&file);

//    QList<Area> areaList;
//    QList<Area> excludeList;
//    while(!xml.atEnd()) {
//        xml.readNext();
//        qDebug() << xml.tokenString() << " " << xml.name();

//        // Track
//        if (xml.name() == "trk") {
//            xml.readNext();

//            QList<QGeoCoordinate> path;

//            bool exclude = false;
//            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "trk")) {
//                  xml.readNext();
////                  qDebug() << xml.tokenString() << " " << xml.name();
//                  if(xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "name") {
//                      xml.readNext();
////                      qDebug() << "got track" << xml.text();
//                      if(xml.text().toString().startsWith("Exclude_")) {
//                          exclude = true;
//                      } else {
//                          exclude = false;
//                      }
//                  }
//                  if(xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "trkseg") {
//                      xml.readNext();
////                      qDebug() << "got track segment:" << xml.text();
//                      while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "trkseg")) {
//                          xml.readNext();
//                          if(xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "trkpt") {
//                              QGeoCoordinate coord;
//                              coord.setLongitude(xml.attributes().value("lon").toString().toDouble());
//                              coord.setLatitude(xml.attributes().value("lat").toString().toDouble());
//                              path.append(coord);
//                          }
//                      }
//                  }

//            }

//            Area area;
//            area.setPath(path);
////            qDebug() << "appending path" << path;
//            if(!exclude) {
//                areaList.append(area);
//            } else {
//                excludeList.append(area);
//            }
//        }

//    }
//    m_businessArea.setAreaList(areaList);
//    m_businessArea.setExcludes(excludeList);
    //    emit businessAreaChanged();
}

#if defined Q_WS_S60 || QT_VERSION >= 0x050000
void Car2goEngine::receivedToken(const QString &token, const QString &tokenSecret)
{


}

void Car2goEngine::temporaryTokenReceived(const QString &token, const QString &tokenSecret)
{
    qDebug() << "got token" << token << tokenSecret;
    m_temporaryTokenSecret = tokenSecret;
    m_temporaryToken = token;

    QString url = Car2GoAuthorizeURL;
    url.append( "?" );
    url.append( "&oauth_token=" + m_temporaryToken );
    qDebug() << "got url2:" << QUrl(url);

//    QDesktopServices::openUrl(QUrl::fromEncoded(url.toUtf8()));
    emit authUrlReceived(url);

}

void Car2goEngine::authorizationReceived(const QString &token, const QString &verifier)
{
    qDebug() << "got authorization token" << token << verifier;
}

void Car2goEngine::accessTokenReceived(const QString &token, const QString &tokenSecret)
{
    qDebug() << "got access token" << token << tokenSecret << m_oauthManager->lastError();

    if(m_oauthManager->lastError() == 0) {
        // Save informations
        m_token = token;
        m_tokenSecret = tokenSecret;

#if QT_VERSION < 0x050000
        QSettings settings("getmewheels", "getmewheels");
#else
        QSettings settings("com.ubuntu.developer.mzanetti.getmewheels2", "getmewheels2");
#endif
        settings.beginGroup("car2go");
        settings.setValue("OAuthAccessToken", m_token);
        settings.setValue("OAuthAccessTokenSecret", m_tokenSecret);
        settings.setValue("AuthExpirationDate", QDateTime::currentDateTime().addDays(31));
    }

    m_timeout = false;
    m_loop.quit();
}

void Car2goEngine::requestReady(const QByteArray &response)
{
    m_lastData = response;
    m_timeout = false;
    m_loop.quit();
    qDebug() << "oauth response received" << response << m_oauthManager->lastError();
}

void Car2goEngine::authorizedRequestDone()
{
    //    qDebug() << "oauth response received" << m_oauthManager->lastError();
}

#endif

QVariantMap Car2goEngine::parseJson(const QByteArray &data)
{
#if QT_VERSION < 0x050000
    QJson::Parser parser;
    bool ok = true;
    QVariantMap replyMap = parser.parse(data, &ok).toMap();
    if(!ok) {
        qDebug() << "failed to parse data" << data;
    }
#else
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);

    if(error.error != QJsonParseError::NoError) {
        qDebug() << "failed to parse data" << data << ":" << error.errorString();
    }
    QVariantMap replyMap = jsonDoc.toVariant().toMap();
#endif
    return replyMap;
}

bool Car2goEngine::waitForResponse()
{
    m_timeout = true;
    QTimer::singleShot(10000, &m_loop, SLOT(quit()));
    qDebug() << "looping" << m_loop.isRunning();
    m_loop.exec();
    return !m_timeout;
}

void Car2goEngine::parseVehicles(const QVariantMap &vehiclesMap)
{
//    qDebug() << "parsing vehicles" << vehiclesMap;
    QList<GMWVehicle*> itemList;
    foreach(const QVariant &vehicle, vehiclesMap.value("placemarks").toList()) {
        itemList.append(parseVehicle(vehicle.toMap()));
    }
    if(m_currentDownloadLocation != 0 && m_account.isValid()) {
        foreach(GMWVehicle *vehicle, bookings(m_currentDownloadLocation)) {
            itemList.append(vehicle);
        }
    }
    m_currentDownloadLocation = 0;
    emit vehiclesReceived(itemList);
}

GMWVehicle* Car2goEngine::parseVehicle(const QVariantMap &vehicleMap)
{
    QString address = vehicleMap.value("address").toString();
    QGeoCoordinate coordinate;
    if(vehicleMap.value("coordinates").toList().count() == 3) {
        coordinate.setLongitude(vehicleMap.value("coordinates").toList().first().toDouble());
        coordinate.setLatitude(vehicleMap.value("coordinates").toList().at(1).toDouble());
//        coordinate.setAltitude(vehicleMap.value("coordinates").toList().last().toDouble());
    }
    QString name = vehicleMap.value("name").toString();
    GMWVehicle::State exteriorState = GMWVehicle::vehicleStateFromString(vehicleMap.value("exterior").toString());
    GMWVehicle::State interiorState = GMWVehicle::vehicleStateFromString(vehicleMap.value("interior").toString());
    int fuel = vehicleMap.value("fuel").toInt();
    QString vin = vehicleMap.value("vin").toString();
    GMWVehicle::EngineType engineType = vehicleMap.value("engineType").toString() == "ED" ? GMWVehicle::EngineTypeED : GMWVehicle::EngineTypeCD;
    QPixmap pm = engineType == GMWVehicle::EngineTypeED ? m_imageVehiclesED : m_imageVehicles;
    QPixmap pm_l = engineType == GMWVehicle::EngineTypeED ? m_imageVehiclesEDL : m_imageVehiclesL;

    return new GMWVehicle(name, address, coordinate, pm, pm_l, fuel, interiorState, exteriorState, vin, engineType);
}

GMWVehicle* Car2goEngine::parseBookedVehicle(const QVariantMap &vehicleMap)
{
    qDebug() << "booked vehicle" << vehicleMap;
    QString name = vehicleMap.value("numberPlate").toString();
    QString address = vehicleMap.value("position").toMap().value("address").toString();
    QGeoCoordinate position;
    position.setLatitude(vehicleMap.value("position").toMap().value("latitude").toDouble());
    position.setLongitude(vehicleMap.value("position").toMap().value("longitude").toDouble());
    quint8 fuel = vehicleMap.value("fuel").toInt();
    GMWVehicle::State interiorState = GMWVehicle::vehicleStateFromString(vehicleMap.value("interior").toString());
    GMWVehicle::State exteriorState = GMWVehicle::vehicleStateFromString(vehicleMap.value("exterior").toString());
    QString vin = vehicleMap.value("vin").toString();
    GMWVehicle::EngineType engineType = vehicleMap.value("engineType").toString() == "ED" ? GMWVehicle::EngineTypeED : GMWVehicle::EngineTypeCD;
    QPixmap pm = engineType == GMWVehicle::EngineTypeED ? m_imageVehiclesED : m_imageVehicles;
    QPixmap pm_l = engineType == GMWVehicle::EngineTypeED ? m_imageVehiclesEDL : m_imageVehiclesL;

    return new GMWVehicle(name, address, position, pm, pm_l, fuel, interiorState, exteriorState, vin, engineType);
}

void Car2goEngine::parseGasStations(const QVariantMap &gasStationsMap)
{
//    qDebug() << "parsing gas stations" << gasStationsMap;
    QList<GMWGasStation*> itemList;
    foreach(const QVariant &gasStation, gasStationsMap.value("placemarks").toList()) {
        itemList.append(parseGasStation(gasStation.toMap()));
    }
    emit gasStationsReceived(itemList);
}

GMWGasStation* Car2goEngine::parseGasStation(const QVariantMap &gasStationMap) {
//    qDebug() << "parsing gas station" << gasStationMap;
    QString name = gasStationMap.value("name").toString();
    QGeoCoordinate position;
    position.setLongitude(gasStationMap.value("coordinates").toList().at(0).toDouble());
    position.setLatitude(gasStationMap.value("coordinates").toList().at(1).toDouble());
//    qDebug() << "got gas station" << name << position;
    return new GMWGasStation("car2go Gas Station", name, position, m_imageGasStations, m_imageGasStationsL);
}

void Car2goEngine::parseParkingSpots(const QVariantMap &parkingSpotsMap)
{
//    qDebug() << "parsing parking spots" << parkingSpotsMap;
    QList<GMWParkingSpot*> itemList;
    foreach(const QVariant &parkingSpot, parkingSpotsMap.value("placemarks").toList()) {
        itemList.append(parseParkingSpot(parkingSpot.toMap()));
    }
    emit parkingSpotsReceived(itemList);
}

GMWParkingSpot *Car2goEngine::parseParkingSpot(const QVariantMap &parkingSpotMap)
{
//    qDebug() << "parsing gas station" << parkingSpotMap;
    QString name = parkingSpotMap.value("name").toString();
    QGeoCoordinate position;
    position.setLongitude(parkingSpotMap.value("coordinates").toList().at(0).toDouble());
    position.setLatitude(parkingSpotMap.value("coordinates").toList().at(1).toDouble());
    qDebug() << "got gas station" << name << position;
    int totalCapacity = parkingSpotMap.value("totalCapacity").toInt();
    int usedCapacity = parkingSpotMap.value("usedCapacity").toInt();
    bool chargingPole = parkingSpotMap.value("chargingPole").toBool();
    QPixmap pm = chargingPole ? m_imageParkingSpotsCP : m_imageParkingSpots;
    QPixmap pm_l = chargingPole ? m_imageParkingSpotsCPL : m_imageParkingSpotsL;
    return new GMWParkingSpot("car2go Parking Spot", name, position, pm, pm_l, usedCapacity, totalCapacity, chargingPole);
}
