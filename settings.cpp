#include "settings.h"
#include <QGeoCoordinate>

QTM_USE_NAMESPACE

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    m_settings = new QSettings("getmewheels");

}

void Settings::writeDefaultLocation(const Location *location)
{
    m_settings->setValue("locationId", location->id());
    m_settings->setValue("locationName", location->name());
    m_settings->setValue("locationTopLeftLatitude", location->area().topLeft().latitude());
    m_settings->setValue("locationTopLeftLongitude", location->area().topLeft().longitude());
    m_settings->setValue("locationBottomRightLatitude", location->area().bottomRight().latitude());
    m_settings->setValue("locationBottomRightLongitude", location->area().bottomRight().longitude());
}

bool Settings::readDefaultLocation(Location *location)
{
    if(!m_settings->contains("locationId")) {
        return false;
    }

    location->setId(m_settings->value("locationId").toInt());
    location->setName(m_settings->value("locationName").toString());
    QGeoCoordinate topLeft;
    topLeft.setLatitude(m_settings->value("locationTopLeftLatitude").toDouble());
    topLeft.setLongitude(m_settings->value("locationTopLeftLongitude").toDouble());

    QGeoCoordinate bottomRight;
    bottomRight.setLatitude(m_settings->value("locationBottomRightLatitude").toDouble());
    bottomRight.setLongitude(m_settings->value("locationBottomRightLongitude").toDouble());

    QGeoBoundingBox area;
    area.setTopLeft(topLeft);
    area.setBottomRight(bottomRight);

    location->setArea(area);
    return true;
}

