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

#ifndef GMWMARKER_H
#define GMWMARKER_H

#include "data/gmwitem.h"

#include <qgeomappixmapobject.h>

QTM_USE_NAMESPACE

class GMWMarkerPrivate;

class GMWMarker : public QGeoMapPixmapObject
{
    Q_OBJECT

public:
    explicit GMWMarker(GMWItem* item);

public slots:
    void setHighlight(bool highlight);

private:
    GMWMarkerPrivate *d;

private slots:
    void itemChanged();
    void calculatePixmap();

};

#endif // GMWMARKER_H
