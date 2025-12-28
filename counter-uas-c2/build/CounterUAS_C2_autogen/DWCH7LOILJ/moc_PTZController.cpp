/****************************************************************************
** Meta object code from reading C++ file 'PTZController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/PTZController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PTZController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__PTZController_t {
    QByteArrayData data[17];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__PTZController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__PTZController_t qt_meta_stringdata_CounterUAS__PTZController = {
    {
QT_MOC_LITERAL(0, 0, 25), // "CounterUAS::PTZController"
QT_MOC_LITERAL(1, 26, 9), // "connected"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 12), // "disconnected"
QT_MOC_LITERAL(4, 50, 15), // "positionChanged"
QT_MOC_LITERAL(5, 66, 3), // "pan"
QT_MOC_LITERAL(6, 70, 4), // "tilt"
QT_MOC_LITERAL(7, 75, 4), // "zoom"
QT_MOC_LITERAL(8, 80, 16), // "movementComplete"
QT_MOC_LITERAL(9, 97, 5), // "error"
QT_MOC_LITERAL(10, 103, 7), // "message"
QT_MOC_LITERAL(11, 111, 17), // "onSocketConnected"
QT_MOC_LITERAL(12, 129, 20), // "onSocketDisconnected"
QT_MOC_LITERAL(13, 150, 17), // "onSocketReadyRead"
QT_MOC_LITERAL(14, 168, 13), // "onSocketError"
QT_MOC_LITERAL(15, 182, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(16, 211, 14) // "updatePosition"

    },
    "CounterUAS::PTZController\0connected\0"
    "\0disconnected\0positionChanged\0pan\0"
    "tilt\0zoom\0movementComplete\0error\0"
    "message\0onSocketConnected\0"
    "onSocketDisconnected\0onSocketReadyRead\0"
    "onSocketError\0QAbstractSocket::SocketError\0"
    "updatePosition"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__PTZController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    3,   66,    2, 0x06 /* Public */,
       8,    0,   73,    2, 0x06 /* Public */,
       9,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   77,    2, 0x08 /* Private */,
      12,    0,   78,    2, 0x08 /* Private */,
      13,    0,   79,    2, 0x08 /* Private */,
      14,    1,   80,    2, 0x08 /* Private */,
      16,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,    5,    6,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,    9,
    QMetaType::Void,

       0        // eod
};

void CounterUAS::PTZController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PTZController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->positionChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 3: _t->movementComplete(); break;
        case 4: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->onSocketConnected(); break;
        case 6: _t->onSocketDisconnected(); break;
        case 7: _t->onSocketReadyRead(); break;
        case 8: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 9: _t->updatePosition(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
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
            using _t = void (PTZController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PTZController::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PTZController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PTZController::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PTZController::*)(double , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PTZController::positionChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PTZController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PTZController::movementComplete)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (PTZController::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PTZController::error)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::PTZController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__PTZController.data,
    qt_meta_data_CounterUAS__PTZController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::PTZController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::PTZController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__PTZController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::PTZController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::PTZController::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CounterUAS::PTZController::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CounterUAS::PTZController::positionChanged(double _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::PTZController::movementComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CounterUAS::PTZController::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
