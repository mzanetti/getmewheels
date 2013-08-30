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

#ifndef GMWACCOUNT_H
#define GMWACCOUNT_H

#include <QString>
#include <QMetaType>

class GMWAccount
{
public:
    GMWAccount(int id = -1, const QString &decription = QString());

    bool isValid() const;

    int id() const;
    QString description() const;

    bool operator==(const GMWAccount &other) const;
    bool operator!=(const GMWAccount &other) const;

private:
    int m_id;
    QString m_description;
};
Q_DECLARE_METATYPE(GMWAccount)
#endif // GMWACCOUNT_H
