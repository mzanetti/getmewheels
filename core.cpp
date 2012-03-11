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

#include "core.h"
#include "engines/car2go/car2goengine.h"

#include <QDir>
#include <QDebug>

Core *Core::s_instance = 0;
Core *Core::instance()
{
    if(!s_instance) {
        s_instance = new Core();
    }
    return s_instance;
}

Core::Core(QObject *parent) :
    QObject(parent)
{
#if !defined QT_SIMULATOR && !defined Q_WS_S60
    m_serviceProvider = new QGeoServiceProvider("openstreetmap");
    qDebug() << "created GeoServiceProvider" << m_serviceProvider;
#else
    m_serviceProvider = new QGeoServiceProvider("nokia");
#endif
}

QGeoServiceProvider *Core::serviceProvider()
{
    return m_serviceProvider;
}
