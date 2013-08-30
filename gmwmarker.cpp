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
    int m_count;
};

GMWMarker::GMWMarker(GMWItem *item) :
    QGeoMapPixmapObject(),
    d(new GMWMarkerPrivate)
{
    d->m_item = item;
    d->m_highlight = false;
    d->m_count = 0;
    setCoordinate(item->location());

    QPixmap image = d->m_item->image();
    setOffset(QPoint(-image.width()/2, -image.height()));
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

int GMWMarker::count() const
{
    return d->m_count;
}

void GMWMarker::setCount(int count)
{
    d->m_count = count;
    calculatePixmap();
}

void GMWMarker::calculatePixmap()
{
//    qDebug() << "calculating pixmap for:" << d->m_item->name();
    bool booked = false;
    int timeLeft;
    if(d->m_item->objectType() == GMWItem::TypeVehicle) {
        GMWVehicle *vehicle = qobject_cast<GMWVehicle*>(d->m_item);
        if(vehicle->booking()->isValid()) {
            booked = true;
            timeLeft = vehicle->booking()->timeLeft();
        }
    }

    if (d->m_highlight || booked) {
        QPixmap pixmap = d->m_item->imageL();
        if(booked) {
            qDebug() << "car is booked:" << d->m_item->name();
            QImage image = pixmap.toImage();
            QPainter painter(&image);
            QPixmap overlay;
#ifdef MEEGO
            if(timeLeft > 900) {
                overlay = QPixmap("/usr/share/themes/blanco/meegotouch/icons/icon-s-common-presence-online.png");
            } else if(timeLeft > 0){
                overlay = QPixmap("/usr/share/themes/blanco/meegotouch/icons/icon-s-common-presence-away.png");
            } else {
                overlay = QPixmap("/usr/share/themes/blanco/meegotouch/icons/icon-s-common-presence-busy.png");
            }
#elif defined Q_WS_S60
            if(timeLeft > 900) {
                overlay = QPixmap("qml/symbian/images/bookingstate_green_s.png");
            } else if(timeLeft > 0){
                overlay = QPixmap("qml/symbian/images/bookingstate_yellow_s.png");
            } else {
                overlay = QPixmap("qml/symbian/images/bookingstate_red_s.png");
            }
#endif
            painter.drawPixmap(image.width() - overlay.width() - 5, 5, overlay);
            pixmap = QPixmap::fromImage(image);
        }
        setPixmap(pixmap);
        setOffset(QPoint(-pixmap.width()/2, -pixmap.height()));
        setZValue(1);
    } else {
        setPixmap(d->m_item->image());
        setOffset(QPoint(-pixmap().width()/2, -pixmap().height()));
        setZValue(0);
    }

    if(d->m_count > 0 && !booked) {
        QImage image = pixmap().toImage();
        QPainter painter(&image);

        QPixmap overlay = QPixmap(":qml/getmewheels2/harmattan/images/squircle_s_gray.png");
        painter.drawPixmap(image.width() - overlay.width(), 0, overlay);

        QPen pen(Qt::white);
        painter.setPen(pen);
        QFont font = painter.font();
        font.setPixelSize(14);
        painter.setFont(font);
        painter.drawText(image.width() - 13, 14, QString::number(d->m_count));
        setPixmap(QPixmap::fromImage(image));

    }

}

void GMWMarker::itemChanged()
{
    calculatePixmap();
}
