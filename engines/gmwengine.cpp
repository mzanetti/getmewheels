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

#include "gmwengine.h"
#include "car2go/car2goengine.h"
#include "settings.h"

#include <QtCore/QDebug>

GMWEngine::GMWEngine(): QObject()
{
    m_plugin = new Car2goEngine();
    connect(m_plugin, SIGNAL(gasStationsReceived(QList<GMWGasStation*>)), SLOT(gasStationsReceived(QList<GMWGasStation*>)));
    connect(m_plugin, SIGNAL(parkingSpotsReceived(QList<GMWParkingSpot*>)), SLOT(parkingSpotsReceived(QList<GMWParkingSpot*>)));
    connect(m_plugin, SIGNAL(vehiclesReceived(QList<GMWVehicle*>)), SLOT(vehiclesReceived(QList<GMWVehicle*>)));

    m_location = new Location();
    Settings settings;
    qDebug() << "reading location from settings:" << settings.readDefaultLocation(m_location);
}

GMWEngine::~GMWEngine()
{
    delete m_plugin;
}

void GMWEngine::setLocationName(const QString &locationName)
{
    qDebug() << "Setting location" << locationName;
    foreach(Location *location, supportedLocations()) {
        if(location->name() == locationName) {
            m_location = location;
            emit locationChanged();
            Settings settings;
            settings.writeDefaultLocation(m_location);
            refreshStationary(false);
            refreshVehicles(false);
            return;
        }
    }
    qDebug() << "Location" << locationName << "not available";
}

QString GMWEngine::locationName()
{
    return m_location->name();
}

Location *GMWEngine::location()
{
    return m_location;
}

QStringList GMWEngine::supportedLocationNames()
{
    QStringList retList;
    foreach(Location *location, supportedLocations()) {
        retList << location->name();
    }
    return retList;
}

QList<Location*> GMWEngine::supportedLocations()
{
    if(m_supportedLocations.isEmpty()) {
        m_supportedLocations = m_plugin->fetchLocations();
    }
    qDebug() << "supported locations:" << m_supportedLocations.count();
    return m_supportedLocations;
}

bool GMWEngine::startAuthentication()
{
    qDebug() << "starting auth";
    return m_plugin->startAuthentication();
}

bool GMWEngine::setAccessCode(const QString &code)
{
    qDebug() << "setting access code" << code;
    bool ret = m_plugin->setAccessCode(code);
    emit authenticatedChanged();
    // Clear accounts cache
    if(ret) {
        m_accounts.clear();
    }
    return ret;
}

QDateTime GMWEngine::authExpirationDate()
{
    return m_plugin->authExpirationDate();
}

bool GMWEngine::authenticated()
{
    return m_plugin->authenticated();
}

QStringList GMWEngine::accountNames(const QString &location)
{
    Location *loc = 0;
    foreach(Location *l, supportedLocations()) {
        if(l->name() == location) {
            loc = l;
        }
    }

    if(!loc) {
        qDebug() << "location not found";
        return QStringList();
    }

    QStringList ret;
    foreach(const GMWAccount &account, accounts(loc)) {
        ret.append(account.description());
    }
    return ret;
}

GMWAccount GMWEngine::defaultAccount()
{
    return m_plugin->account();
}

QString GMWEngine::defaultAccountName()
{
    return defaultAccount().description();
}

void GMWEngine::setDefaultAccountName(const QString &accountName)
{
// For some reason I can't figure out my simulators qoauth doesn't support HMAC(SHA1) and crashes. So disable everything to do with auth
#ifndef QT_SIMULATOR
    foreach(const GMWAccount &account, accounts()) {
        if(account.description() == accountName) {
            setDefaultAccount(account);
            return;
        }
    }
#endif
}

bool GMWEngine::createBooking(GMWVehicle *vehicle, const GMWAccount &account)
{
    return m_plugin->createBooking(m_location, vehicle, account);
}

bool GMWEngine::cancelBooking(GMWVehicle *vehicle)
{
    return m_plugin->cancelBooking(vehicle);
}

void GMWEngine::clearDefaultAccount()
{
    setDefaultAccount(GMWAccount());
}

void GMWEngine::clearAuthentication()
{
    m_plugin->removeAuthentication();
    emit authenticatedChanged();
}

void GMWEngine::refreshStationary(bool useCache)
{
    if(!m_location) {
        qDebug() << "Set location first";
        return;
    }
    if(useCache) {
        // load stuff from cache
    }
    m_plugin->fetchGasStations(m_location);
    m_plugin->fetchParkingSpots(m_location);
}

void GMWEngine::refreshVehicles(bool useCache)
{
    if(!m_location) {
        qDebug() << "Set location first";
        return;
    }
    if(useCache) {
        // TODO: load stuff from cache here
    }
    m_plugin->fetchVehicles(m_location);
}

void GMWEngine::setDefaultAccount(const GMWAccount &account)
{
// For some reason I can't figure out my simulators qoauth doesn't support HMAC(SHA1) and crashes. So disable everything to do with auth
#ifndef QT_SIMULATOR
    m_plugin->setAccount(account);
    emit defaultAccountChanged();
#endif
}

QList<GMWAccount> GMWEngine::accounts(Location *location)
{
    Location *loc = m_location;
    if(location != 0) {
        loc = location;
    }

    if(m_accounts.value(location).isEmpty()) {
        m_accounts[location] = m_plugin->accounts(loc);
    }
    return m_accounts.value(location);
}

QString GMWEngine::error()
{
    return m_plugin->error();
}

void GMWEngine::gasStationsReceived(QList<GMWGasStation *> gasStations)
{
    QList<GMWItem*> objectList;
    foreach(GMWGasStation* station, gasStations) {
        objectList.append(station);
    }
    emit objectsReceived(objectList);
}

void GMWEngine::parkingSpotsReceived(QList<GMWParkingSpot *> parkingSpots)
{
    QList<GMWItem*> objectList;
    foreach(GMWParkingSpot* spot, parkingSpots) {
        objectList.append(spot);
    }
    emit objectsReceived(objectList);
}

void GMWEngine::vehiclesReceived(QList<GMWVehicle *> vehicles)
{
    QList<GMWItem*> objectList;
    foreach(GMWVehicle* car, vehicles) {
        objectList.append(car);
    }
    emit objectsReceived(objectList);
}
