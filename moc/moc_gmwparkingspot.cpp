/****************************************************************************
** Meta object code from reading C++ file 'gmwparkingspot.h'
**
** Created: Sun Feb 5 18:08:49 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../data/gmwparkingspot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmwparkingspot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GMWParkingSpot[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       3,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,

 // properties: name, type, flags
      34,   26, 0x85495001,
      47,   26, 0x85495001,
      66,   61, 0x01495001,

 // properties: notify_signal_id
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_GMWParkingSpot[] = {
    "GMWParkingSpot\0\0changed()\0quint16\0"
    "capacityUsed\0capacityTotal\0bool\0"
    "chargingPole\0"
};

const QMetaObject GMWParkingSpot::staticMetaObject = {
    { &GMWItem::staticMetaObject, qt_meta_stringdata_GMWParkingSpot,
      qt_meta_data_GMWParkingSpot, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GMWParkingSpot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GMWParkingSpot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GMWParkingSpot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GMWParkingSpot))
        return static_cast<void*>(const_cast< GMWParkingSpot*>(this));
    return GMWItem::qt_metacast(_clname);
}

int GMWParkingSpot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GMWItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changed(); break;
        default: ;
        }
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< quint16*>(_v) = capacityUsed(); break;
        case 1: *reinterpret_cast< quint16*>(_v) = capacityTotal(); break;
        case 2: *reinterpret_cast< bool*>(_v) = chargingPole(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GMWParkingSpot::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
