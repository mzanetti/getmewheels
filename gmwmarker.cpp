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

#include "gmwmarker.h"
#include "data/gmwvehicle.h"

#include <QtCore/QDebug>

#include <QtGui/QPixmap>

#include <QtCore/QDebug>

#include <QtGui/QPixmap>
#include <QPainter>

class GMWMarkerPrivate
{
public:
    GMWItem *m_item;
    bool m_highlight;
};

GMWMarker::GMWMarker(GMWItem *item) :
    QGeoMapPixmapObject(),
    d(new GMWMarkerPrivate)
{
    d->m_item = item;
    d->m_highlight = false;
    setCoordinate(item->location());

    QPixmap image = d->m_item->image();
    setOffset(QPoint(0, -image.height()));
    setPixmap(image);
    setZValue(0);

    itemChanged();
    connect(item, SIGNAL(changed()), SLOT(itemChanged()));
}

void GMWMarker::setHighlight(bool highlight)
{
    if(d->m_highlight == highlight) {
        return;
    }
    d->m_highlight = highlight;
    calculatePixmap();
}

void GMWMarker::calculatePixmap()
{
    qDebug() << "calculating pixmap for:" << d->m_item->name();
    bool booked = false;
    if(d->m_item->objectType() == GMWItem::TypeVehicle) {
        GMWVehicle *vehicle = qobject_cast<GMWVehicle*>(d->m_item);
        if(vehicle->booking()->isValid() && !vehicle->booking()->isExpired()) {
            booked = true;
        }
    }

    QPixmap image = d->m_item->image();
    if (d->m_highlight || booked) {
        QSize newSize = QSize(image.width()*1.5, image.height()*1.5);
        setOffset(QPoint(0, -newSize.height()));
        setZValue(1);
        if(booked) {
            qDebug() << "car is booked:" << d->m_item->name();
            QImage overlay = d->m_item->image().toImage();
            QPainter painter(&overlay);
            painter.drawPixmap(overlay.width() - overlay.width() / 1.5, overlay.height() - overlay.height() / 1.5, QPixmap("/usr/share/themes/blanco/meegotouch/icons/icon-m-common-presence-away.png").scaled(overlay.size() / 1.5, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            image = QPixmap::fromImage(overlay);
        }
        setPixmap(image.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        setOffset(QPoint(0, -image.height()));
        setPixmap(image);
        setZValue(0);
    }

}

void GMWMarker::itemChanged()
{
    calculatePixmap();
}
