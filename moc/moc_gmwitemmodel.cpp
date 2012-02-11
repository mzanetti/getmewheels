/****************************************************************************
** Meta object code from reading C++ file 'gmwitemmodel.h'
**
** Created: Sun Feb 5 18:08:48 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../data/gmwitemmodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmwitemmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GMWItemModel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       1,   69, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      32,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   48,   13,   13, 0x0a,
      79,   73,   13,   13, 0x0a,
     107,   13,   13,   13, 0x0a,
     118,   13,   13,   13, 0x0a,
     153,  134,   13,   13, 0x0a,
     209,  202,  198,   13, 0x0a,
     231,   13,  198,   13, 0x2a,
     242,  202,  198,   13, 0x0a,
     267,   13,   13,   13, 0x08,

 // properties: name, type, flags
     292,  281, 0x0049510b,

 // properties: notify_signal_id
       1,

       0        // eod
};

static const char qt_meta_stringdata_GMWItemModel[] = {
    "GMWItemModel\0\0locationChanged()\0"
    "engineChanged()\0item\0addObject(GMWItem*)\0"
    "items\0addObjects(QList<GMWItem*>)\0"
    "clearAll()\0clearVehicles()\0"
    "position,direction\0"
    "currentPositionChanged(QGeoCoordinate,qreal)\0"
    "int\0parent\0rowCount(QModelIndex)\0"
    "rowCount()\0columnCount(QModelIndex)\0"
    "itemChanged()\0GMWEngine*\0engine\0"
};

const QMetaObject GMWItemModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_GMWItemModel,
      qt_meta_data_GMWItemModel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GMWItemModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GMWItemModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GMWItemModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GMWItemModel))
        return static_cast<void*>(const_cast< GMWItemModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int GMWItemModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: locationChanged(); break;
        case 1: engineChanged(); break;
        case 2: addObject((*reinterpret_cast< GMWItem*(*)>(_a[1]))); break;
        case 3: addObjects((*reinterpret_cast< QList<GMWItem*>(*)>(_a[1]))); break;
        case 4: clearAll(); break;
        case 5: clearVehicles(); break;
        case 6: currentPositionChanged((*reinterpret_cast< const QGeoCoordinate(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 7: { int _r = rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: { int _r = rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { int _r = columnCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: itemChanged(); break;
        default: ;
        }
        _id -= 11;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< GMWEngine**>(_v) = engine(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setEngine(*reinterpret_cast< GMWEngine**>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GMWItemModel::locationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void GMWItemModel::engineChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
