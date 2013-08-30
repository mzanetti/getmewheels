/****************************************************************************
** Meta object code from reading C++ file 'kqoauthmanager.h'
**
** Created: Sun Mar 11 21:46:08 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../kqoauthmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kqoauthmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KQOAuthManager[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   16,   15,   15, 0x05,
      85,   54,   15,   15, 0x05,
     116,   54,   15,   15, 0x05,
     183,  156,   15,   15, 0x05,
     222,   54,   15,   15, 0x05,
     259,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     289,  283,   15,   15, 0x08,
     337,  328,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_KQOAuthManager[] = {
    "KQOAuthManager\0\0networkReply\0"
    "requestReady(QByteArray)\0"
    "oauth_token,oauth_token_secret\0"
    "receivedToken(QString,QString)\0"
    "temporaryTokenReceived(QString,QString)\0"
    "oauth_token,oauth_verifier\0"
    "authorizationReceived(QString,QString)\0"
    "accessTokenReceived(QString,QString)\0"
    "authorizedRequestDone()\0reply\0"
    "onRequestReplyReceived(QNetworkReply*)\0"
    "response\0onVerificationReceived(QMultiMap<QString,QString>)\0"
};

const QMetaObject KQOAuthManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KQOAuthManager,
      qt_meta_data_KQOAuthManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KQOAuthManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KQOAuthManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KQOAuthManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KQOAuthManager))
        return static_cast<void*>(const_cast< KQOAuthManager*>(this));
    return QObject::qt_metacast(_clname);
}

int KQOAuthManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: requestReady((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: receivedToken((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: temporaryTokenReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: authorizationReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: accessTokenReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: authorizedRequestDone(); break;
        case 6: onRequestReplyReceived((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 7: onVerificationReceived((*reinterpret_cast< QMultiMap<QString,QString>(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void KQOAuthManager::requestReady(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KQOAuthManager::receivedToken(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KQOAuthManager::temporaryTokenReceived(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KQOAuthManager::authorizationReceived(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void KQOAuthManager::accessTokenReceived(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void KQOAuthManager::authorizedRequestDone()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
