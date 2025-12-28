/****************************************************************************
** Meta object code from reading C++ file 'SensorInterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/sensors/SensorInterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SensorInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__SensorInterface_t {
    QByteArrayData data[18];
    char stringdata0[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__SensorInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__SensorInterface_t qt_meta_stringdata_CounterUAS__SensorInterface = {
    {
QT_MOC_LITERAL(0, 0, 27), // "CounterUAS::SensorInterface"
QT_MOC_LITERAL(1, 28, 9), // "detection"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 15), // "SensorDetection"
QT_MOC_LITERAL(4, 55, 13), // "statusChanged"
QT_MOC_LITERAL(5, 69, 12), // "SensorStatus"
QT_MOC_LITERAL(6, 82, 6), // "status"
QT_MOC_LITERAL(7, 89, 16), // "connectedChanged"
QT_MOC_LITERAL(8, 106, 9), // "connected"
QT_MOC_LITERAL(9, 116, 13), // "healthUpdated"
QT_MOC_LITERAL(10, 130, 12), // "SensorHealth"
QT_MOC_LITERAL(11, 143, 6), // "health"
QT_MOC_LITERAL(12, 150, 5), // "error"
QT_MOC_LITERAL(13, 156, 7), // "message"
QT_MOC_LITERAL(14, 164, 11), // "processData"
QT_MOC_LITERAL(15, 176, 12), // "updateHealth"
QT_MOC_LITERAL(16, 189, 8), // "sensorId"
QT_MOC_LITERAL(17, 198, 10) // "sensorType"

    },
    "CounterUAS::SensorInterface\0detection\0"
    "\0SensorDetection\0statusChanged\0"
    "SensorStatus\0status\0connectedChanged\0"
    "connected\0healthUpdated\0SensorHealth\0"
    "health\0error\0message\0processData\0"
    "updateHealth\0sensorId\0sensorType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__SensorInterface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       4,   66, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       7,    1,   55,    2, 0x06 /* Public */,
       9,    1,   58,    2, 0x06 /* Public */,
      12,    1,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   64,    2, 0x09 /* Protected */,
      15,    0,   65,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    1,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QString,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      16, QMetaType::QString, 0x00095401,
      17, QMetaType::QString, 0x00095401,
       6, 0x80000000 | 5, 0x00495009,
       8, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       0,
       1,
       2,

       0        // eod
};

void CounterUAS::SensorInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SensorInterface *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->detection((*reinterpret_cast< const SensorDetection(*)>(_a[1]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< SensorStatus(*)>(_a[1]))); break;
        case 2: _t->connectedChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->healthUpdated((*reinterpret_cast< const SensorHealth(*)>(_a[1]))); break;
        case 4: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->processData(); break;
        case 6: _t->updateHealth(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SensorInterface::*)(const SensorDetection & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorInterface::detection)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SensorInterface::*)(SensorStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorInterface::statusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SensorInterface::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorInterface::connectedChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SensorInterface::*)(const SensorHealth & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorInterface::healthUpdated)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SensorInterface::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorInterface::error)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SensorInterface *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->sensorId(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->sensorType(); break;
        case 2: *reinterpret_cast< SensorStatus*>(_v) = _t->status(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isConnected(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::SensorInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__SensorInterface.data,
    qt_meta_data_CounterUAS__SensorInterface,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::SensorInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::SensorInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__SensorInterface.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::SensorInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
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
void CounterUAS::SensorInterface::detection(const SensorDetection & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::SensorInterface::statusChanged(SensorStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::SensorInterface::connectedChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::SensorInterface::healthUpdated(const SensorHealth & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::SensorInterface::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
