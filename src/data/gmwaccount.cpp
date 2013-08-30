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

#include "gmwaccount.h"

GMWAccount::GMWAccount(int id, const QString &decription):
    m_id(id), m_description(decription)
{
}

bool GMWAccount::isValid() const
{
    return m_id != -1 && !m_description.isEmpty();
}

int GMWAccount::id() const
{
    return m_id;
}

QString GMWAccount::description() const
{
    return m_description;
}

bool GMWAccount::operator ==(const GMWAccount &other) const
{
    return m_id == other.id();
}

bool GMWAccount::operator !=(const GMWAccount &other) const
{
    return m_id != other.id();
}
