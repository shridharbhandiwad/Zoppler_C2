/****************************************************************************
** Meta object code from reading C++ file 'EngagementManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/EngagementManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EngagementManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__EngagementManager_t {
    QByteArrayData data[34];
    char stringdata0[535];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__EngagementManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__EngagementManager_t qt_meta_stringdata_CounterUAS__EngagementManager = {
    {
QT_MOC_LITERAL(0, 0, 29), // "CounterUAS::EngagementManager"
QT_MOC_LITERAL(1, 30, 12), // "stateChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 15), // "EngagementState"
QT_MOC_LITERAL(4, 60, 5), // "state"
QT_MOC_LITERAL(5, 66, 17), // "engagementChanged"
QT_MOC_LITERAL(6, 84, 12), // "engagementId"
QT_MOC_LITERAL(7, 97, 13), // "trackSelected"
QT_MOC_LITERAL(8, 111, 7), // "trackId"
QT_MOC_LITERAL(9, 119, 16), // "effectorSelected"
QT_MOC_LITERAL(10, 136, 10), // "effectorId"
QT_MOC_LITERAL(11, 147, 19), // "effectorRecommended"
QT_MOC_LITERAL(12, 167, 6), // "reason"
QT_MOC_LITERAL(13, 174, 22), // "authorizationRequested"
QT_MOC_LITERAL(14, 197, 20), // "AuthorizationRequest"
QT_MOC_LITERAL(15, 218, 7), // "request"
QT_MOC_LITERAL(16, 226, 20), // "authorizationGranted"
QT_MOC_LITERAL(17, 247, 10), // "operatorId"
QT_MOC_LITERAL(18, 258, 19), // "authorizationDenied"
QT_MOC_LITERAL(19, 278, 20), // "authorizationTimeout"
QT_MOC_LITERAL(20, 299, 17), // "engagementStarted"
QT_MOC_LITERAL(21, 317, 19), // "engagementCompleted"
QT_MOC_LITERAL(22, 337, 9), // "BDAResult"
QT_MOC_LITERAL(23, 347, 6), // "result"
QT_MOC_LITERAL(24, 354, 17), // "engagementAborted"
QT_MOC_LITERAL(25, 372, 16), // "engagementFailed"
QT_MOC_LITERAL(26, 389, 15), // "switchVideoFeed"
QT_MOC_LITERAL(27, 405, 8), // "cameraId"
QT_MOC_LITERAL(28, 414, 23), // "onEffectorStatusChanged"
QT_MOC_LITERAL(29, 438, 14), // "onTrackDropped"
QT_MOC_LITERAL(30, 453, 22), // "onAuthorizationTimeout"
QT_MOC_LITERAL(31, 476, 25), // "checkEngagementCompletion"
QT_MOC_LITERAL(32, 502, 12), // "currentState"
QT_MOC_LITERAL(33, 515, 19) // "currentEngagementId"

    },
    "CounterUAS::EngagementManager\0"
    "stateChanged\0\0EngagementState\0state\0"
    "engagementChanged\0engagementId\0"
    "trackSelected\0trackId\0effectorSelected\0"
    "effectorId\0effectorRecommended\0reason\0"
    "authorizationRequested\0AuthorizationRequest\0"
    "request\0authorizationGranted\0operatorId\0"
    "authorizationDenied\0authorizationTimeout\0"
    "engagementStarted\0engagementCompleted\0"
    "BDAResult\0result\0engagementAborted\0"
    "engagementFailed\0switchVideoFeed\0"
    "cameraId\0onEffectorStatusChanged\0"
    "onTrackDropped\0onAuthorizationTimeout\0"
    "checkEngagementCompletion\0currentState\0"
    "currentEngagementId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__EngagementManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       2,  160, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,
       5,    1,  107,    2, 0x06 /* Public */,
       7,    1,  110,    2, 0x06 /* Public */,
       9,    1,  113,    2, 0x06 /* Public */,
      11,    2,  116,    2, 0x06 /* Public */,
      13,    1,  121,    2, 0x06 /* Public */,
      16,    1,  124,    2, 0x06 /* Public */,
      18,    1,  127,    2, 0x06 /* Public */,
      19,    0,  130,    2, 0x06 /* Public */,
      20,    1,  131,    2, 0x06 /* Public */,
      21,    2,  134,    2, 0x06 /* Public */,
      24,    2,  139,    2, 0x06 /* Public */,
      25,    2,  144,    2, 0x06 /* Public */,
      26,    1,  149,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      28,    1,  152,    2, 0x0a /* Public */,
      29,    1,  155,    2, 0x0a /* Public */,
      30,    0,  158,    2, 0x08 /* Private */,
      31,    0,  159,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 22,    6,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,   12,
    QMetaType::Void, QMetaType::QString,   27,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      32, 0x80000000 | 3, 0x00495009,
      33, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void CounterUAS::EngagementManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EngagementManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< EngagementState(*)>(_a[1]))); break;
        case 1: _t->engagementChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->trackSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->effectorSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->effectorRecommended((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->authorizationRequested((*reinterpret_cast< const AuthorizationRequest(*)>(_a[1]))); break;
        case 6: _t->authorizationGranted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->authorizationDenied((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->authorizationTimeout(); break;
        case 9: _t->engagementStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->engagementCompleted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< BDAResult(*)>(_a[2]))); break;
        case 11: _t->engagementAborted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 12: _t->engagementFailed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 13: _t->switchVideoFeed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->onEffectorStatusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->onTrackDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->onAuthorizationTimeout(); break;
        case 17: _t->checkEngagementCompletion(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EngagementManager::*)(EngagementState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::engagementChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::trackSelected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::effectorSelected)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::effectorRecommended)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const AuthorizationRequest & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::authorizationRequested)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::authorizationGranted)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::authorizationDenied)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::authorizationTimeout)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::engagementStarted)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & , BDAResult );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::engagementCompleted)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::engagementAborted)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::engagementFailed)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (EngagementManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EngagementManager::switchVideoFeed)) {
                *result = 13;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<EngagementManager *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< EngagementState*>(_v) = _t->currentState(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->currentEngagementId(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::EngagementManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__EngagementManager.data,
    qt_meta_data_CounterUAS__EngagementManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::EngagementManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::EngagementManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__EngagementManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::EngagementManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
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
void CounterUAS::EngagementManager::stateChanged(EngagementState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::EngagementManager::engagementChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::EngagementManager::trackSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::EngagementManager::effectorSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::EngagementManager::effectorRecommended(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CounterUAS::EngagementManager::authorizationRequested(const AuthorizationRequest & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CounterUAS::EngagementManager::authorizationGranted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CounterUAS::EngagementManager::authorizationDenied(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CounterUAS::EngagementManager::authorizationTimeout()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void CounterUAS::EngagementManager::engagementStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CounterUAS::EngagementManager::engagementCompleted(const QString & _t1, BDAResult _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CounterUAS::EngagementManager::engagementAborted(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CounterUAS::EngagementManager::engagementFailed(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CounterUAS::EngagementManager::switchVideoFeed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
