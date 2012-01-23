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

#include "gmwbooking.h"

#include <QObject>
#include <QTimerEvent>
#include <QDebug>

GMWBooking::GMWBooking(QObject *parent): QObject(parent), m_id(-1), m_account(GMWAccount()), m_timerId(-1)
{

}

GMWBooking::GMWBooking(int id, GMWAccount account, QObject *parent):
    QObject(parent),
    m_id(id),
    m_account(account),
    m_timerId(-1)
{
    if(isValid()) {
        m_timerId = startTimer(60000);
    }
}

void GMWBooking::timerEvent(QTimerEvent *event)
{
    qDebug() << "timer event for booking" << m_id;
    emit changed();
    if(m_expriationTime < QDateTime::currentDateTime()) {
        killTimer(event->timerId());
        m_timerId = -1;
    }
}

bool GMWBooking::isValid() const
{
    return m_id > -1 && m_expriationTime.isValid();
}

void GMWBooking::invalidate()
{
    m_id = -1;
    m_expriationTime = QDateTime();
    killTimer(m_timerId);
    m_timerId = -1;
}

int GMWBooking::id() const
{
    return m_id;
}

void GMWBooking::setId(int id)
{
    m_id = id;

    if(isValid() && m_timerId == -1) {
        m_timerId = startTimer(60000);
    }
    emit changed();
}

void GMWBooking::setAccount(const GMWAccount &account)
{
    m_account = account;
    if(isValid() && m_timerId == -1) {
        m_timerId = startTimer(60000);
    }
    emit changed();
}

GMWAccount GMWBooking::account() const
{
    return m_account;
}

void GMWBooking::setTime(const QDateTime &time)
{
    m_time = time;
    if(isValid() && m_timerId == -1) {
        m_timerId = startTimer(60000);
    }
    emit changed();
}

QDateTime GMWBooking::time() const
{
    return m_time;
}

void GMWBooking::setExpirationTime(const QDateTime &time)
{
    m_expriationTime = time;
    if(isValid() && m_timerId == -1) {
        m_timerId = startTimer(60000);
    }
    emit changed();
}

QDateTime GMWBooking::expirationTime() const
{
    return m_expriationTime;
}

QString GMWBooking::toString() const
{
    if(!isValid()) {
        return tr("Free for everyone");
    }
    if(QDateTime::currentDateTime() < m_expriationTime) {
        return QObject::tr("Booked - Expires: %1  (%2min)")
                .arg(m_expriationTime.toString("hh:mm"))
                .arg(QDateTime::currentDateTime().secsTo(m_expriationTime) / 60);
   } else {
        return QObject::tr("Booking expired at %1")
                .arg(m_expriationTime.toString("hh:mm"));

    }
}

bool GMWBooking::isExpired() const
{
    return QDateTime::currentDateTime() > m_expriationTime;
}

int GMWBooking::timeLeft() const
{
    return QDateTime::currentDateTime().secsTo(m_expriationTime);
}
