/****************************************************************************
** Meta object code from reading C++ file 'gmwengine.h'
**
** Created: Sat Mar 3 18:03:17 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../engines/gmwengine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmwengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GMWEngine[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       4,  174, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   11,   10,   10, 0x05,
      49,   10,   10,   10, 0x05,
      92,   67,   10,   10, 0x05,
     145,  137,   10,   10, 0x05,
     183,  168,   10,   10, 0x05,
     224,   10,   10,   10, 0x05,
     242,   10,   10,   10, 0x05,
     264,   10,   10,   10, 0x05,
     287,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     320,  311,   10,   10, 0x0a,
     344,   10,   10,   10, 0x2a,
     364,  311,   10,   10, 0x0a,
     386,   10,   10,   10, 0x2a,
     412,  404,   10,   10, 0x0a,
     442,   10,   10,   10, 0x0a,
     476,  464,   10,   10, 0x0a,
     528,  512,  507,   10, 0x0a,
     574,  566,  507,   10, 0x2a,
     601,  566,  507,   10, 0x0a,
     640,  628,   10,   10, 0x08,
     696,  683,   10,   10, 0x08,
     750,  741,   10,   10, 0x08,

 // methods: signature, parameters, type, tag, flags
     799,   10,  787,   10, 0x02,
     841,   10,  824,   10, 0x02,
     862,   10,  507,   10, 0x02,
     889,  884,  507,   10, 0x02,
     922,   10,  912,   10, 0x02,
     970,  961,  943,   10, 0x02,
     990,   10,  943,   10, 0x22,
    1001,  961,  787,   10, 0x02,
    1031,   10, 1023,   10, 0x02,
    1052,   10, 1023,   10, 0x02,

 // properties: name, type, flags
    1060, 1023, 0x0a495103,
    1073, 1023, 0x0a495103,
    1092,  507, 0x01495001,
    1106,  912, 0x10495001,

 // properties: notify_signal_id
       5,
       8,
       7,
       7,

       0        // eod
};

static const char qt_meta_stringdata_GMWEngine[] = {
    "GMWEngine\0\0list\0objectsReceived(QList<GMWItem*>)\0"
    "downloadStarted()\0type,success,errorString\0"
    "downloadFinished(GMWItem::Type,bool,QString)\0"
    "message\0downloadError(QString)\0"
    "type,cacheDate\0loadedFromCache(GMWItem::Type,QDateTime)\0"
    "locationChanged()\0businessAreaChanged()\0"
    "authenticatedChanged()\0defaultAccountChanged()\0"
    "useCache\0refreshStationary(bool)\0"
    "refreshStationary()\0refreshVehicles(bool)\0"
    "refreshVehicles()\0account\0"
    "setDefaultAccount(GMWAccount)\0"
    "clearDefaultAccount()\0accountName\0"
    "setDefaultAccountName(QString)\0bool\0"
    "vehicle,account\0createBooking(GMWVehicle*,GMWAccount)\0"
    "vehicle\0createBooking(GMWVehicle*)\0"
    "cancelBooking(GMWVehicle*)\0gasStations\0"
    "gasStationsReceived(QList<GMWGasStation*>)\0"
    "parkingSpots\0parkingSpotsReceived(QList<GMWParkingSpot*>)\0"
    "vehicles\0vehiclesReceived(QList<GMWVehicle*>)\0"
    "QStringList\0supportedLocationNames()\0"
    "QList<Location*>\0supportedLocations()\0"
    "startAuthentication()\0code\0"
    "setAccessCode(QString)\0QDateTime\0"
    "authExpirationDate()\0QList<GMWAccount>\0"
    "location\0accounts(Location*)\0accounts()\0"
    "accountNames(QString)\0QString\0"
    "defaultAccountName()\0error()\0locationName\0"
    "defaultAccountName\0authenticated\0"
    "authExpirationDate\0"
};

const QMetaObject GMWEngine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GMWEngine,
      qt_meta_data_GMWEngine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GMWEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GMWEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GMWEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GMWEngine))
        return static_cast<void*>(const_cast< GMWEngine*>(this));
    return QObject::qt_metacast(_clname);
}

int GMWEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: objectsReceived((*reinterpret_cast< QList<GMWItem*>(*)>(_a[1]))); break;
        case 1: downloadStarted(); break;
        case 2: downloadFinished((*reinterpret_cast< GMWItem::Type(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: downloadError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: loadedFromCache((*reinterpret_cast< GMWItem::Type(*)>(_a[1])),(*reinterpret_cast< const QDateTime(*)>(_a[2]))); break;
        case 5: locationChanged(); break;
        case 6: businessAreaChanged(); break;
        case 7: authenticatedChanged(); break;
        case 8: defaultAccountChanged(); break;
        case 9: refreshStationary((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: refreshStationary(); break;
        case 11: refreshVehicles((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: refreshVehicles(); break;
        case 13: setDefaultAccount((*reinterpret_cast< const GMWAccount(*)>(_a[1]))); break;
        case 14: clearDefaultAccount(); break;
        case 15: setDefaultAccountName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: { bool _r = createBooking((*reinterpret_cast< GMWVehicle*(*)>(_a[1])),(*reinterpret_cast< const GMWAccount(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = createBooking((*reinterpret_cast< GMWVehicle*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = cancelBooking((*reinterpret_cast< GMWVehicle*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: gasStationsReceived((*reinterpret_cast< QList<GMWGasStation*>(*)>(_a[1]))); break;
        case 20: parkingSpotsReceived((*reinterpret_cast< QList<GMWParkingSpot*>(*)>(_a[1]))); break;
        case 21: vehiclesReceived((*reinterpret_cast< QList<GMWVehicle*>(*)>(_a[1]))); break;
        case 22: { QStringList _r = supportedLocationNames();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 23: { QList<Location*> _r = supportedLocations();
            if (_a[0]) *reinterpret_cast< QList<Location*>*>(_a[0]) = _r; }  break;
        case 24: { bool _r = startAuthentication();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 25: { bool _r = setAccessCode((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 26: { QDateTime _r = authExpirationDate();
            if (_a[0]) *reinterpret_cast< QDateTime*>(_a[0]) = _r; }  break;
        case 27: { QList<GMWAccount> _r = accounts((*reinterpret_cast< Location*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QList<GMWAccount>*>(_a[0]) = _r; }  break;
        case 28: { QList<GMWAccount> _r = accounts();
            if (_a[0]) *reinterpret_cast< QList<GMWAccount>*>(_a[0]) = _r; }  break;
        case 29: { QStringList _r = accountNames((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 30: { QString _r = defaultAccountName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 31: { QString _r = error();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 32;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = locationName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = defaultAccountName(); break;
        case 2: *reinterpret_cast< bool*>(_v) = authenticated(); break;
        case 3: *reinterpret_cast< QDateTime*>(_v) = authExpirationDate(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLocationName(*reinterpret_cast< QString*>(_v)); break;
        case 1: setDefaultAccountName(*reinterpret_cast< QString*>(_v)); break;
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
void GMWEngine::objectsReceived(QList<GMWItem*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GMWEngine::downloadStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void GMWEngine::downloadFinished(GMWItem::Type _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GMWEngine::downloadError(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GMWEngine::loadedFromCache(GMWItem::Type _t1, const QDateTime & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GMWEngine::locationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void GMWEngine::businessAreaChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void GMWEngine::authenticatedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void GMWEngine::defaultAccountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}
QT_END_MOC_NAMESPACE
