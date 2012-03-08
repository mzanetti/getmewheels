/****************************************************************************
** Meta object code from reading C++ file 'gmwvehicle.h'
**
** Created: Sat Mar 3 18:03:17 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../data/gmwvehicle.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmwvehicle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GMWVehicle[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       6,   19, // properties
       2,   43, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // properties: name, type, flags
      29,   22, 0x86495001,
      45,   39, 0x00495009,
      59,   39, 0x00495009,
      81,   73, 0x0a495001,
      97,   85, 0x00495009,
     116,  105, 0x00495009,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       0,
       0,

 // enums: name, flags, count, data
      39, 0x0,    6,   51,
     105, 0x0,    2,   63,

 // enum data: key, value
     127, uint(GMWVehicle::StateUnknown),
     140, uint(GMWVehicle::StateIssuesExist),
     157, uint(GMWVehicle::StateUnacceptable),
     175, uint(GMWVehicle::StateSatisfied),
     190, uint(GMWVehicle::StateGood),
     200, uint(GMWVehicle::StateExcellent),
     215, uint(GMWVehicle::EngineTypeED),
     228, uint(GMWVehicle::EngineTypeCD),

       0        // eod
};

static const char qt_meta_stringdata_GMWVehicle[] = {
    "GMWVehicle\0\0changed()\0quint8\0fuelLevel\0"
    "State\0interiorState\0exteriorState\0"
    "QString\0vin\0GMWBooking*\0booking\0"
    "EngineType\0engineType\0StateUnknown\0"
    "StateIssuesExist\0StateUnacceptable\0"
    "StateSatisfied\0StateGood\0StateExcellent\0"
    "EngineTypeED\0EngineTypeCD\0"
};

const QMetaObject GMWVehicle::staticMetaObject = {
    { &GMWItem::staticMetaObject, qt_meta_stringdata_GMWVehicle,
      qt_meta_data_GMWVehicle, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GMWVehicle::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GMWVehicle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GMWVehicle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GMWVehicle))
        return static_cast<void*>(const_cast< GMWVehicle*>(this));
    return GMWItem::qt_metacast(_clname);
}

int GMWVehicle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 0: *reinterpret_cast< quint8*>(_v) = fuelLevel(); break;
        case 1: *reinterpret_cast< State*>(_v) = interiorState(); break;
        case 2: *reinterpret_cast< State*>(_v) = exteriorState(); break;
        case 3: *reinterpret_cast< QString*>(_v) = vin(); break;
        case 4: *reinterpret_cast< GMWBooking**>(_v) = booking(); break;
        case 5: *reinterpret_cast< EngineType*>(_v) = engineType(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GMWVehicle::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
