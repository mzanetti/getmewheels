/****************************************************************************
** Meta object code from reading C++ file 'gmwitem.h'
**
** Created: Sat Mar 3 18:03:17 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../data/gmwitem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmwitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GMWItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       5,   24, // properties
       1,   44, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      27,    8,    8,    8, 0x05,

 // properties: name, type, flags
      42,   37, 0x00495009,
      66,   51, 0x00495009,
      83,   75, 0x0a495001,
      88,   75, 0x0a495001,
      96,   75, 0x0a495001,

 // properties: notify_signal_id
       1,
       1,
       1,
       1,
       0,

 // enums: name, flags, count, data
      37, 0x0,    4,   48,

 // enum data: key, value
     105, uint(GMWItem::TypeUnknown),
     117, uint(GMWItem::TypeVehicle),
     129, uint(GMWItem::TypeGasStation),
     144, uint(GMWItem::TypeParkingSpot),

       0        // eod
};

static const char qt_meta_stringdata_GMWItem[] = {
    "GMWItem\0\0distanceChanged()\0changed()\0"
    "Type\0itemType\0QGeoCoordinate\0location\0"
    "QString\0name\0address\0distance\0TypeUnknown\0"
    "TypeVehicle\0TypeGasStation\0TypeParkingSpot\0"
};

const QMetaObject GMWItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GMWItem,
      qt_meta_data_GMWItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GMWItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GMWItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GMWItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GMWItem))
        return static_cast<void*>(const_cast< GMWItem*>(this));
    return QObject::qt_metacast(_clname);
}

int GMWItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: distanceChanged(); break;
        case 1: changed(); break;
        default: ;
        }
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Type*>(_v) = objectType(); break;
        case 1: *reinterpret_cast< QGeoCoordinate*>(_v) = location(); break;
        case 2: *reinterpret_cast< QString*>(_v) = name(); break;
        case 3: *reinterpret_cast< QString*>(_v) = address(); break;
        case 4: *reinterpret_cast< QString*>(_v) = distanceString(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GMWItem::distanceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void GMWItem::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
