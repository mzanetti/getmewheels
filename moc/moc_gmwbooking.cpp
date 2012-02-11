/****************************************************************************
** Meta object code from reading C++ file 'gmwbooking.h'
**
** Created: Sun Feb 5 18:08:49 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../data/gmwbooking.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmwbooking.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GMWBooking[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       4,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // properties: name, type, flags
      30,   22, 0x0a495001,
      40,   35, 0x01495001,
      46,   35, 0x01495001,
      58,   54, 0x02495001,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_GMWBooking[] = {
    "GMWBooking\0\0changed()\0QString\0text\0"
    "bool\0valid\0expired\0int\0timeLeft\0"
};

const QMetaObject GMWBooking::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GMWBooking,
      qt_meta_data_GMWBooking, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GMWBooking::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GMWBooking::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GMWBooking::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GMWBooking))
        return static_cast<void*>(const_cast< GMWBooking*>(this));
    return QObject::qt_metacast(_clname);
}

int GMWBooking::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
        case 0: *reinterpret_cast< QString*>(_v) = toString(); break;
        case 1: *reinterpret_cast< bool*>(_v) = isValid(); break;
        case 2: *reinterpret_cast< bool*>(_v) = isExpired(); break;
        case 3: *reinterpret_cast< int*>(_v) = timeLeft(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
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
void GMWBooking::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
