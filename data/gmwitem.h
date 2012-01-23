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

#ifndef GMWITEM_H
#define GMWITEM_H

#include <QString>
#include <QPixmap>
#include <QGeoPositionInfo>
#include <QMetaType>

QTM_USE_NAMESPACE

class GMWItem: public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(Type itemType READ objectType NOTIFY changed)
    Q_PROPERTY(QGeoCoordinate location READ location NOTIFY changed)
    Q_PROPERTY(QString name READ name NOTIFY changed)
    Q_PROPERTY(QString address READ address NOTIFY changed)
    Q_PROPERTY(QString distance READ distanceString NOTIFY distanceChanged)

public:
    enum Type {
        TypeUnknown    = 0x01,
        TypeVehicle    = 0x02,
        TypeGasStation = 0x04,
        TypeParkingSpot = 0x08
    };
    Q_DECLARE_FLAGS(Types, Type)

    friend QDataStream &operator<<(QDataStream& stream, const GMWItem& item);
    friend QDataStream &operator>>(QDataStream& stream, GMWItem& item);

    GMWItem(const QString &name, const QString &address, const QGeoCoordinate &location, const QPixmap &image, const QPixmap &imageL);

    virtual GMWItem::Type objectType() const;

    QString name() const;
    QString address() const;
    QGeoCoordinate location() const;

    QPixmap image() const;
    QPixmap imageL() const;
    void setImage(const QPixmap &image);

    void calculateDistance(const QGeoCoordinate &currentPosition, qreal direction);
    qreal distance() const;
    QString distanceString() const;
    qreal azimuth() const;
    QString azimuthString() const;
    QPixmap azimuthImage() const;

private:
    QString m_name;
    QString m_address;
    QGeoCoordinate m_location;
    QPixmap m_image;
    QPixmap m_imageL;
    QPixmap m_azimuthImage;

    qreal m_distance;
    qreal m_azimuth;

signals:
    void distanceChanged();
    void changed();
};

QDataStream &operator<<(QDataStream& stream, const GMWItem& item);
QDataStream &operator>>(QDataStream& stream, GMWItem& item);

Q_DECLARE_OPERATORS_FOR_FLAGS(GMWItem::Types)
Q_DECLARE_METATYPE(GMWItem*)

#endif // GMWITEM_H
