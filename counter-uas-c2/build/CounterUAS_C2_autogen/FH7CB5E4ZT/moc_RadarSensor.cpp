/****************************************************************************
** Meta object code from reading C++ file 'RadarSensor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/sensors/RadarSensor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadarSensor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__RadarSensor_t {
    QByteArrayData data[15];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__RadarSensor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__RadarSensor_t qt_meta_stringdata_CounterUAS__RadarSensor = {
    {
QT_MOC_LITERAL(0, 0, 23), // "CounterUAS::RadarSensor"
QT_MOC_LITERAL(1, 24, 19), // "trackReportReceived"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 16), // "RadarTrackReport"
QT_MOC_LITERAL(4, 62, 6), // "report"
QT_MOC_LITERAL(5, 69, 14), // "statusReceived"
QT_MOC_LITERAL(6, 84, 6), // "status"
QT_MOC_LITERAL(7, 91, 11), // "processData"
QT_MOC_LITERAL(8, 103, 11), // "onConnected"
QT_MOC_LITERAL(9, 115, 14), // "onDisconnected"
QT_MOC_LITERAL(10, 130, 11), // "onReadyRead"
QT_MOC_LITERAL(11, 142, 7), // "onError"
QT_MOC_LITERAL(12, 150, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(13, 179, 5), // "error"
QT_MOC_LITERAL(14, 185, 16) // "attemptReconnect"

    },
    "CounterUAS::RadarSensor\0trackReportReceived\0"
    "\0RadarTrackReport\0report\0statusReceived\0"
    "status\0processData\0onConnected\0"
    "onDisconnected\0onReadyRead\0onError\0"
    "QAbstractSocket::SocketError\0error\0"
    "attemptReconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__RadarSensor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       5,    1,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   60,    2, 0x09 /* Protected */,
       8,    0,   61,    2, 0x08 /* Private */,
       9,    0,   62,    2, 0x08 /* Private */,
      10,    0,   63,    2, 0x08 /* Private */,
      11,    1,   64,    2, 0x08 /* Private */,
      14,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QByteArray,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,

       0        // eod
};

void CounterUAS::RadarSensor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RadarSensor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->trackReportReceived((*reinterpret_cast< const RadarTrackReport(*)>(_a[1]))); break;
        case 1: _t->statusReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->processData(); break;
        case 3: _t->onConnected(); break;
        case 4: _t->onDisconnected(); break;
        case 5: _t->onReadyRead(); break;
        case 6: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 7: _t->attemptReconnect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
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
            using _t = void (RadarSensor::*)(const RadarTrackReport & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadarSensor::trackReportReceived)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RadarSensor::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadarSensor::statusReceived)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::RadarSensor::staticMetaObject = { {
    QMetaObject::SuperData::link<SensorInterface::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__RadarSensor.data,
    qt_meta_data_CounterUAS__RadarSensor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::RadarSensor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::RadarSensor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__RadarSensor.stringdata0))
        return static_cast<void*>(this);
    return SensorInterface::qt_metacast(_clname);
}

int CounterUAS::RadarSensor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SensorInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::RadarSensor::trackReportReceived(const RadarTrackReport & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::RadarSensor::statusReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
