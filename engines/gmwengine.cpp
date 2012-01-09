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

#include "gmwengine.h"

#include <QtCore/QDebug>

GMWEngine::GMWEngine()
{
}

GMWEngine::~GMWEngine()
{

}

void GMWEngine::setLocation(const QString &location) {
    qDebug() << "should set location to" << location;
    if(supportedLocations().contains(location)) {
        m_location = location;
        emit locationChanged();
    }
}

QString GMWEngine::location()
{
    return m_location;
}

QStringList GMWEngine::accountNames()
{
    QStringList ret;
    foreach(const GMWAccount &account, accounts()) {
        ret.append(account.description());
    }
    return ret;
}

QString GMWEngine::defaultAccountName()
{
    return defaultAccount().description();
}

void GMWEngine::setDefaultAccountName(const QString &accountName)
{
    foreach(const GMWAccount &account, accounts()) {
        if(account.description() == accountName) {
            setDefaultAccount(account);
            return;
        }
    }
}

void GMWEngine::clearDefaultAccount()
{
    setDefaultAccount(GMWAccount());
}
