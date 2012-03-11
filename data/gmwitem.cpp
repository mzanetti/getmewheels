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

#include "gmwitem.h"

#include <QApplication>

GMWItem::GMWItem(const QString &name, const QString &address, const QGeoCoordinate &location, const QPixmap &image, const QPixmap &imageL) :
    m_name(name),
    m_address(address),
    m_location(location),
    m_image(image),
    m_imageL(imageL),
    m_distance(-1),
    m_azimuth(-1)
{
}

GMWItem::Type GMWItem::objectType() const
{
    return GMWItem::TypeUnknown;
}

QString GMWItem::name() const
{
    return m_name;
}

QString GMWItem::address() const
{
    return m_address;
}

QGeoCoordinate GMWItem::location() const
{
    return m_location;
}

QPixmap GMWItem::image() const
{
    return m_image;
}

QPixmap GMWItem::imageL() const
{
    return m_imageL;
}

void GMWItem::setImage(const QPixmap &image)
{
    m_image = image;
    emit changed();
}

void GMWItem::calculateDistance(const QGeoCoordinate &currentPosition, qreal distance)
{
    m_distance = currentPosition.distanceTo(m_location);
    m_azimuth = currentPosition.azimuthTo(m_location) - distance;

    QPixmap arrow = QPixmap(":/draw-arrow-up.png");
    //arrow.fill(Qt::red);
    QTransform trans = QTransform();
    trans.rotate(m_azimuth);

    QPixmap rotArrow = arrow.transformed(trans);
    int x = (rotArrow.width() - arrow.width()) / 2;
    int y = (rotArrow.height() - arrow.height()) / 2;

    m_azimuthImage = rotArrow.copy(x, y, arrow.width(), arrow.height());

}

qreal GMWItem::distance() const
{
    return m_distance;
}

QString GMWItem::distanceString() const
{
    if (m_distance == -1) {
        return QApplication::tr("Waiting for GPS...");
    } else if (m_distance < 1000) {
        return QString::number(m_distance, 'f', 0) + " m";
    } else {
        return QString::number(m_distance / 1000, 'f', 1) + " km";
    }
}

qreal GMWItem::azimuth() const
{
    return m_azimuth;
}

QString GMWItem::azimuthString() const
{
    if (m_distance == -1) {
        return QApplication::tr("Waiting for GPS...");
    } else {
        return QString::number(m_azimuth, 'f', 0);
    }
}

QPixmap GMWItem::azimuthImage() const
{
    return m_azimuthImage;
}

QDataStream &operator<<(QDataStream& stream, const GMWItem& item)
{
    stream << item.m_name << item.m_address << item.m_location; // << item.m_image; // Stored in separate cache file
    return stream;
}

QDataStream &operator>>(QDataStream& stream, GMWItem& item)
{
    stream >> item.m_name;
    stream >> item.m_address;
    stream >> item.m_location;
//    stream >> item.m_image; // stored in separate cache file to save space
    return stream;
}
