/****************************************************************************
** Meta object code from reading C++ file 'gmwitemsortfilterproxymodel.h'
**
** Created: Sat Mar 3 18:03:17 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../data/gmwitemsortfilterproxymodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmwitemsortfilterproxymodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GMWItemSortFilterProxyModel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       2,   29, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   29,   28,   28, 0x05,
      65,   28,   28,   28, 0x05,

 // slots: signature, parameters, type, tag, flags
     105,   84,   28,   28, 0x08,

 // properties: name, type, flags
     161,  147, 0x0049500b,
     172,  167, 0x01095103,

 // properties: notify_signal_id
       1,
       0,

       0        // eod
};

static const char qt_meta_stringdata_GMWItemSortFilterProxyModel[] = {
    "GMWItemSortFilterProxyModel\0\0types\0"
    "filterChanged(GMWItem::Types)\0"
    "itemModelChanged()\0firstIndex,lastIndex\0"
    "modelDataChanged(QModelIndex,QModelIndex)\0"
    "GMWItemModel*\0model\0bool\0onlyBooked\0"
};

const QMetaObject GMWItemSortFilterProxyModel::staticMetaObject = {
    { &QSortFilterProxyModel::staticMetaObject, qt_meta_stringdata_GMWItemSortFilterProxyModel,
      qt_meta_data_GMWItemSortFilterProxyModel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GMWItemSortFilterProxyModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GMWItemSortFilterProxyModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GMWItemSortFilterProxyModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GMWItemSortFilterProxyModel))
        return static_cast<void*>(const_cast< GMWItemSortFilterProxyModel*>(this));
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int GMWItemSortFilterProxyModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: filterChanged((*reinterpret_cast< GMWItem::Types(*)>(_a[1]))); break;
        case 1: itemModelChanged(); break;
        case 2: modelDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< GMWItemModel**>(_v) = itemModel(); break;
        case 1: *reinterpret_cast< bool*>(_v) = onlyBooked(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setItemModel(*reinterpret_cast< GMWItemModel**>(_v)); break;
        case 1: setOnlyBooked(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GMWItemSortFilterProxyModel::filterChanged(GMWItem::Types _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GMWItemSortFilterProxyModel::itemModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
