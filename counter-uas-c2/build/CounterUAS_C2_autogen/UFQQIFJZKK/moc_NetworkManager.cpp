/****************************************************************************
** Meta object code from reading C++ file 'NetworkManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/network/NetworkManager.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_CounterUAS__NetworkManager_t {
    uint offsetsAndSizes[44];
    char stringdata0[27];
    char stringdata1[24];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[17];
    char stringdata5[7];
    char stringdata6[16];
    char stringdata7[8];
    char stringdata8[8];
    char stringdata9[16];
    char stringdata10[6];
    char stringdata11[17];
    char stringdata12[15];
    char stringdata13[6];
    char stringdata14[15];
    char stringdata15[18];
    char stringdata16[15];
    char stringdata17[11];
    char stringdata18[29];
    char stringdata19[15];
    char stringdata20[17];
    char stringdata21[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__NetworkManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__NetworkManager_t qt_meta_stringdata_CounterUAS__NetworkManager = {
    {
        QT_MOC_LITERAL(0, 26),  // "CounterUAS::NetworkManager"
        QT_MOC_LITERAL(27, 23),  // "connectionStatusChanged"
        QT_MOC_LITERAL(51, 0),  // ""
        QT_MOC_LITERAL(52, 12),  // "connectionId"
        QT_MOC_LITERAL(65, 16),  // "ConnectionStatus"
        QT_MOC_LITERAL(82, 6),  // "status"
        QT_MOC_LITERAL(89, 15),  // "messageReceived"
        QT_MOC_LITERAL(105, 7),  // "Message"
        QT_MOC_LITERAL(113, 7),  // "message"
        QT_MOC_LITERAL(121, 15),  // "connectionError"
        QT_MOC_LITERAL(137, 5),  // "error"
        QT_MOC_LITERAL(143, 16),  // "bandwidthUpdated"
        QT_MOC_LITERAL(160, 14),  // "BandwidthStats"
        QT_MOC_LITERAL(175, 5),  // "stats"
        QT_MOC_LITERAL(181, 14),  // "onTcpConnected"
        QT_MOC_LITERAL(196, 17),  // "onTcpDisconnected"
        QT_MOC_LITERAL(214, 14),  // "onTcpReadyRead"
        QT_MOC_LITERAL(229, 10),  // "onTcpError"
        QT_MOC_LITERAL(240, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(269, 14),  // "onUdpReadyRead"
        QT_MOC_LITERAL(284, 16),  // "attemptReconnect"
        QT_MOC_LITERAL(301, 15)   // "updateBandwidth"
    },
    "CounterUAS::NetworkManager",
    "connectionStatusChanged",
    "",
    "connectionId",
    "ConnectionStatus",
    "status",
    "messageReceived",
    "Message",
    "message",
    "connectionError",
    "error",
    "bandwidthUpdated",
    "BandwidthStats",
    "stats",
    "onTcpConnected",
    "onTcpDisconnected",
    "onTcpReadyRead",
    "onTcpError",
    "QAbstractSocket::SocketError",
    "onUdpReadyRead",
    "attemptReconnect",
    "updateBandwidth"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__NetworkManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   80,    2, 0x06,    1 /* Public */,
       6,    2,   85,    2, 0x06,    4 /* Public */,
       9,    2,   90,    2, 0x06,    7 /* Public */,
      11,    1,   95,    2, 0x06,   10 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    0,   98,    2, 0x08,   12 /* Private */,
      15,    0,   99,    2, 0x08,   13 /* Private */,
      16,    0,  100,    2, 0x08,   14 /* Private */,
      17,    1,  101,    2, 0x08,   15 /* Private */,
      19,    0,  104,    2, 0x08,   17 /* Private */,
      20,    0,  105,    2, 0x08,   18 /* Private */,
      21,    0,  106,    2, 0x08,   19 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 7,    3,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,   10,
    QMetaType::Void, 0x80000000 | 12,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::NetworkManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__NetworkManager.offsetsAndSizes,
    qt_meta_data_CounterUAS__NetworkManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__NetworkManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<NetworkManager, std::true_type>,
        // method 'connectionStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<ConnectionStatus, std::false_type>,
        // method 'messageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Message &, std::false_type>,
        // method 'connectionError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'bandwidthUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const BandwidthStats &, std::false_type>,
        // method 'onTcpConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTcpDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTcpReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTcpError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>,
        // method 'onUdpReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'attemptReconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateBandwidth'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CounterUAS::NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ConnectionStatus>>(_a[2]))); break;
        case 1: _t->messageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Message>>(_a[2]))); break;
        case 2: _t->connectionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->bandwidthUpdated((*reinterpret_cast< std::add_pointer_t<BandwidthStats>>(_a[1]))); break;
        case 4: _t->onTcpConnected(); break;
        case 5: _t->onTcpDisconnected(); break;
        case 6: _t->onTcpReadyRead(); break;
        case 7: _t->onTcpError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 8: _t->onUdpReadyRead(); break;
        case 9: _t->attemptReconnect(); break;
        case 10: _t->updateBandwidth(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkManager::*)(const QString & , ConnectionStatus );
            if (_t _q_method = &NetworkManager::connectionStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & , const Message & );
            if (_t _q_method = &NetworkManager::messageReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & , const QString & );
            if (_t _q_method = &NetworkManager::connectionError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const BandwidthStats & );
            if (_t _q_method = &NetworkManager::bandwidthUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *CounterUAS::NetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::NetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__NetworkManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::NetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::NetworkManager::connectionStatusChanged(const QString & _t1, ConnectionStatus _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::NetworkManager::messageReceived(const QString & _t1, const Message & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::NetworkManager::connectionError(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::NetworkManager::bandwidthUpdated(const BandwidthStats & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
