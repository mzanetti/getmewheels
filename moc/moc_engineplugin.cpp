/****************************************************************************
** Meta object code from reading C++ file 'engineplugin.h'
**
** Created: Sun Feb 5 18:08:51 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../engines/engineplugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'engineplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EnginePlugin[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x05,
      63,   14,   13,   13, 0x05,
     108,   14,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_EnginePlugin[] = {
    "EnginePlugin\0\0items\0"
    "gasStationsReceived(QList<GMWGasStation*>)\0"
    "parkingSpotsReceived(QList<GMWParkingSpot*>)\0"
    "vehiclesReceived(QList<GMWVehicle*>)\0"
};

const QMetaObject EnginePlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EnginePlugin,
      qt_meta_data_EnginePlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EnginePlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EnginePlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EnginePlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EnginePlugin))
        return static_cast<void*>(const_cast< EnginePlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int EnginePlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: gasStationsReceived((*reinterpret_cast< QList<GMWGasStation*>(*)>(_a[1]))); break;
        case 1: parkingSpotsReceived((*reinterpret_cast< QList<GMWParkingSpot*>(*)>(_a[1]))); break;
        case 2: vehiclesReceived((*reinterpret_cast< QList<GMWVehicle*>(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void EnginePlugin::gasStationsReceived(QList<GMWGasStation*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EnginePlugin::parkingSpotsReceived(QList<GMWParkingSpot*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EnginePlugin::vehiclesReceived(QList<GMWVehicle*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
