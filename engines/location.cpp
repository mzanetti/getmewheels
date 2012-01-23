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

#include "location.h"

Location::Location()
{
}

int Location::id() const
{
    return m_id;
}

void Location::setId(int id)
{
    m_id = id;
}

QString Location::name() const
{
    return m_name;
}

void Location::setName(const QString &name)
{
    m_name = name;
}

QGeoBoundingBox Location::area() const
{
    return m_area;
}

void Location::setArea(const QGeoBoundingBox &area)
{
    m_area = area;
}
