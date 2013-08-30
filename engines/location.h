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

#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include <QString>
#if QT_VERSION < 0x050000
  #include <QGeoBoundingBox>
  QTM_USE_NAMESPACE
  typedef QGeoBoundingBox QGeoRectangle;
#else
  #include <QGeoRectangle>
#endif


class Location: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
public:
    Location();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QGeoRectangle area() const;
    void setArea(const QGeoRectangle &area);

private:
    int m_id;
    QString m_name;
    QGeoRectangle m_area;

};

#endif // LOCATION_H
