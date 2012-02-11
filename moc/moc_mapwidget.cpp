/****************************************************************************
** Meta object code from reading C++ file 'mapwidget.h'
**
** Created: Sun Feb 5 18:08:48 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mapwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       4,  104, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   11,   10,   10, 0x05,
      44,   38,   10,   10, 0x05,
      73,   67,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      96,   10,   10,   10, 0x0a,
     123,   11,   10,   10, 0x0a,
     158,  141,   10,   10, 0x08,
     192,  141,   10,   10, 0x08,
     239,  234,   10,   10, 0x08,
     279,  273,   10,   10, 0x08,
     336,  312,   10,   10, 0x08,
     396,   10,   10,   10, 0x08,
     415,   10,   10,   10, 0x08,
     433,   10,   10,   10, 0x08,

 // methods: signature, parameters, type, tag, flags
     480,  450,   10,   10, 0x02,
     523,  516,   10,   10, 0x22,
     565,  554,   10,   10, 0x02,
     595,   11,   10,   10, 0x02,
     633,  619,   10,   10, 0x02,

 // properties: name, type, flags
     661,  654, 0x06095103,
     676,  654, 0x06095103,
     706,  692, 0x0009510b,
     717,  712, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_MapWidget[] = {
    "MapWidget\0\0item\0itemClicked(GMWItem*)\0"
    "items\0itemsClicked(QVariant)\0error\0"
    "routingFailed(QString)\0"
    "centerToStartCoordinates()\0routeTo(GMWItem*)\0"
    "parent,start,end\0rowsInserted(QModelIndex,int,int)\0"
    "rowsAboutToBeRemoved(QModelIndex,int,int)\0"
    "info\0positionUpdated(QGeoPositionInfo)\0"
    "reply\0routingFinished(QGeoRouteReply*)\0"
    "reply,error,errorString\0"
    "routingError(QGeoRouteReply*,QGeoRouteReply::Error,QString)\0"
    "mapEngineChanged()\0setBusinessArea()\0"
    "updateMapItems()\0bounds,preserveViewportCenter\0"
    "fitInViewport(QGeoBoundingBox,bool)\0"
    "bounds\0fitInViewport(QGeoBoundingBox)\0"
    "coordinage\0animatedPanTo(QGeoCoordinate)\0"
    "animatedPanTo(GMWItem*)\0mouseX,mouseY\0"
    "clicked(qreal,qreal)\0double\0centerLatitude\0"
    "centerLongitude\0GMWItemModel*\0model\0"
    "bool\0tracking\0"
};

const QMetaObject MapWidget::staticMetaObject = {
    { &QGraphicsGeoMap::staticMetaObject, qt_meta_stringdata_MapWidget,
      qt_meta_data_MapWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapWidget))
        return static_cast<void*>(const_cast< MapWidget*>(this));
    return QGraphicsGeoMap::qt_metacast(_clname);
}

int MapWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsGeoMap::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: itemClicked((*reinterpret_cast< GMWItem*(*)>(_a[1]))); break;
        case 1: itemsClicked((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 2: routingFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: centerToStartCoordinates(); break;
        case 4: routeTo((*reinterpret_cast< GMWItem*(*)>(_a[1]))); break;
        case 5: rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: rowsAboutToBeRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: positionUpdated((*reinterpret_cast< const QGeoPositionInfo(*)>(_a[1]))); break;
        case 8: routingFinished((*reinterpret_cast< QGeoRouteReply*(*)>(_a[1]))); break;
        case 9: routingError((*reinterpret_cast< QGeoRouteReply*(*)>(_a[1])),(*reinterpret_cast< QGeoRouteReply::Error(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 10: mapEngineChanged(); break;
        case 11: setBusinessArea(); break;
        case 12: updateMapItems(); break;
        case 13: fitInViewport((*reinterpret_cast< const QGeoBoundingBox(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 14: fitInViewport((*reinterpret_cast< const QGeoBoundingBox(*)>(_a[1]))); break;
        case 15: animatedPanTo((*reinterpret_cast< const QGeoCoordinate(*)>(_a[1]))); break;
        case 16: animatedPanTo((*reinterpret_cast< GMWItem*(*)>(_a[1]))); break;
        case 17: clicked((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 18;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = centerLatitude(); break;
        case 1: *reinterpret_cast< double*>(_v) = centerLongitude(); break;
        case 2: *reinterpret_cast< GMWItemModel**>(_v) = model(); break;
        case 3: *reinterpret_cast< bool*>(_v) = tracking(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setCenterLatitude(*reinterpret_cast< double*>(_v)); break;
        case 1: setCenterLongitude(*reinterpret_cast< double*>(_v)); break;
        case 2: setModel(*reinterpret_cast< GMWItemModel**>(_v)); break;
        case 3: setTracking(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MapWidget::itemClicked(GMWItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MapWidget::itemsClicked(const QVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MapWidget::routingFailed(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
