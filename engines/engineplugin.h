#ifndef ENGINEPLUGIN_H
#define ENGINEPLUGIN_H

#include "location.h"
#include "data/gmwaccount.h"
#include "data/gmwvehicle.h"
#include "data/gmwgasstation.h"
#include "data/gmwparkingspot.h"

#include <QObject>

class EnginePlugin : public QObject
{
    Q_OBJECT
public:
    explicit EnginePlugin(QObject *parent = 0);
    ~EnginePlugin() {}
    
    virtual QList<Location*> fetchLocations() = 0;

    virtual void fetchGasStations(Location *location) = 0;
    virtual void fetchParkingSpots(Location *location) = 0;
    virtual void fetchVehicles(Location *location) = 0;

    virtual bool startAuthentication() = 0;
    virtual bool setAccessCode(const QString &accessCode) = 0;
    virtual bool authenticated() = 0;
    virtual QDateTime authExpirationDate() = 0;
    virtual QList<GMWAccount> accounts(Location *location) = 0;
    virtual GMWAccount account() = 0;
    virtual void setAccount(const GMWAccount &account) = 0;

    virtual bool createBooking(Location *location, GMWVehicle *vehicle, const GMWAccount &account) = 0;
    virtual bool cancelBooking(GMWVehicle *vehicle) = 0;


    virtual QString error() = 0;

signals:
    void gasStationsReceived(QList<GMWGasStation*> items);
    void parkingSpotsReceived(QList<GMWParkingSpot*> items);
    void vehiclesReceived(QList<GMWVehicle*> items);

};

#endif // ENGINEPLUGIN_H
