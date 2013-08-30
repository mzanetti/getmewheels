/*****************************************************************************
 * Copyright: 2012 Michael Zanetti <michael_zanetti@gmx.net>                 *
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

#include "settings.h"
#include <QGeoCoordinate>

#if QT_VERSION < 0x050000
QTM_USE_NAMESPACE
#endif

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

    QGeoRectangle area;
    area.setTopLeft(topLeft);
    area.setBottomRight(bottomRight);

    location->setArea(area);
    return true;
}

