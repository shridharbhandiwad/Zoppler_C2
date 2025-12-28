/****************************************************************************
** Meta object code from reading C++ file 'NetworkManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/network/NetworkManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__NetworkManager_t {
    QByteArrayData data[22];
    char stringdata0[317];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__NetworkManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__NetworkManager_t qt_meta_stringdata_CounterUAS__NetworkManager = {
    {
QT_MOC_LITERAL(0, 0, 26), // "CounterUAS::NetworkManager"
QT_MOC_LITERAL(1, 27, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 12), // "connectionId"
QT_MOC_LITERAL(4, 65, 16), // "ConnectionStatus"
QT_MOC_LITERAL(5, 82, 6), // "status"
QT_MOC_LITERAL(6, 89, 15), // "messageReceived"
QT_MOC_LITERAL(7, 105, 7), // "Message"
QT_MOC_LITERAL(8, 113, 7), // "message"
QT_MOC_LITERAL(9, 121, 15), // "connectionError"
QT_MOC_LITERAL(10, 137, 5), // "error"
QT_MOC_LITERAL(11, 143, 16), // "bandwidthUpdated"
QT_MOC_LITERAL(12, 160, 14), // "BandwidthStats"
QT_MOC_LITERAL(13, 175, 5), // "stats"
QT_MOC_LITERAL(14, 181, 14), // "onTcpConnected"
QT_MOC_LITERAL(15, 196, 17), // "onTcpDisconnected"
QT_MOC_LITERAL(16, 214, 14), // "onTcpReadyRead"
QT_MOC_LITERAL(17, 229, 10), // "onTcpError"
QT_MOC_LITERAL(18, 240, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(19, 269, 14), // "onUdpReadyRead"
QT_MOC_LITERAL(20, 284, 16), // "attemptReconnect"
QT_MOC_LITERAL(21, 301, 15) // "updateBandwidth"

    },
    "CounterUAS::NetworkManager\0"
    "connectionStatusChanged\0\0connectionId\0"
    "ConnectionStatus\0status\0messageReceived\0"
    "Message\0message\0connectionError\0error\0"
    "bandwidthUpdated\0BandwidthStats\0stats\0"
    "onTcpConnected\0onTcpDisconnected\0"
    "onTcpReadyRead\0onTcpError\0"
    "QAbstractSocket::SocketError\0"
    "onUdpReadyRead\0attemptReconnect\0"
    "updateBandwidth"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__NetworkManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,
       6,    2,   74,    2, 0x06 /* Public */,
       9,    2,   79,    2, 0x06 /* Public */,
      11,    1,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   87,    2, 0x08 /* Private */,
      15,    0,   88,    2, 0x08 /* Private */,
      16,    0,   89,    2, 0x08 /* Private */,
      17,    1,   90,    2, 0x08 /* Private */,
      19,    0,   93,    2, 0x08 /* Private */,
      20,    0,   94,    2, 0x08 /* Private */,
      21,    0,   95,    2, 0x08 /* Private */,

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

void CounterUAS::NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< ConnectionStatus(*)>(_a[2]))); break;
        case 1: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const Message(*)>(_a[2]))); break;
        case 2: _t->connectionError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->bandwidthUpdated((*reinterpret_cast< const BandwidthStats(*)>(_a[1]))); break;
        case 4: _t->onTcpConnected(); break;
        case 5: _t->onTcpDisconnected(); break;
        case 6: _t->onTcpReadyRead(); break;
        case 7: _t->onTcpError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 8: _t->onUdpReadyRead(); break;
        case 9: _t->attemptReconnect(); break;
        case 10: _t->updateBandwidth(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkManager::*)(const QString & , ConnectionStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::connectionStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & , const Message & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::messageReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::connectionError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const BandwidthStats & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::bandwidthUpdated)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::NetworkManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__NetworkManager.data,
    qt_meta_data_CounterUAS__NetworkManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
