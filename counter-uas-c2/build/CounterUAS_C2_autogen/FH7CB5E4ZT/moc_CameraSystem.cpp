/****************************************************************************
** Meta object code from reading C++ file 'CameraSystem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/sensors/CameraSystem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CameraSystem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__CameraSystem_t {
    QByteArrayData data[21];
    char stringdata0[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__CameraSystem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__CameraSystem_t qt_meta_stringdata_CounterUAS__CameraSystem = {
    {
QT_MOC_LITERAL(0, 0, 24), // "CounterUAS::CameraSystem"
QT_MOC_LITERAL(1, 25, 10), // "frameReady"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 5), // "frame"
QT_MOC_LITERAL(4, 43, 11), // "frameNumber"
QT_MOC_LITERAL(5, 55, 15), // "cameraDetection"
QT_MOC_LITERAL(6, 71, 15), // "CameraDetection"
QT_MOC_LITERAL(7, 87, 9), // "detection"
QT_MOC_LITERAL(8, 97, 10), // "ptzChanged"
QT_MOC_LITERAL(9, 108, 14), // "CameraPTZState"
QT_MOC_LITERAL(10, 123, 5), // "state"
QT_MOC_LITERAL(11, 129, 12), // "slewComplete"
QT_MOC_LITERAL(12, 142, 11), // "GeoPosition"
QT_MOC_LITERAL(13, 154, 6), // "target"
QT_MOC_LITERAL(14, 161, 16), // "recordingChanged"
QT_MOC_LITERAL(15, 178, 9), // "recording"
QT_MOC_LITERAL(16, 188, 11), // "streamError"
QT_MOC_LITERAL(17, 200, 5), // "error"
QT_MOC_LITERAL(18, 206, 11), // "processData"
QT_MOC_LITERAL(19, 218, 15), // "onFrameReceived"
QT_MOC_LITERAL(20, 234, 18) // "updateSlewProgress"

    },
    "CounterUAS::CameraSystem\0frameReady\0"
    "\0frame\0frameNumber\0cameraDetection\0"
    "CameraDetection\0detection\0ptzChanged\0"
    "CameraPTZState\0state\0slewComplete\0"
    "GeoPosition\0target\0recordingChanged\0"
    "recording\0streamError\0error\0processData\0"
    "onFrameReceived\0updateSlewProgress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__CameraSystem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       5,    1,   64,    2, 0x06 /* Public */,
       8,    1,   67,    2, 0x06 /* Public */,
      11,    1,   70,    2, 0x06 /* Public */,
      14,    1,   73,    2, 0x06 /* Public */,
      16,    1,   76,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,   79,    2, 0x09 /* Protected */,
      19,    1,   80,    2, 0x08 /* Private */,
      20,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage, QMetaType::LongLong,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::QString,   17,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void,

       0        // eod
};

void CounterUAS::CameraSystem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CameraSystem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 1: _t->cameraDetection((*reinterpret_cast< const CameraDetection(*)>(_a[1]))); break;
        case 2: _t->ptzChanged((*reinterpret_cast< const CameraPTZState(*)>(_a[1]))); break;
        case 3: _t->slewComplete((*reinterpret_cast< const GeoPosition(*)>(_a[1]))); break;
        case 4: _t->recordingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->streamError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->processData(); break;
        case 7: _t->onFrameReceived((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 8: _t->updateSlewProgress(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CameraSystem::*)(const QImage & , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSystem::frameReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(const CameraDetection & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSystem::cameraDetection)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(const CameraPTZState & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSystem::ptzChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(const GeoPosition & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSystem::slewComplete)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSystem::recordingChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSystem::streamError)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::CameraSystem::staticMetaObject = { {
    QMetaObject::SuperData::link<SensorInterface::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__CameraSystem.data,
    qt_meta_data_CounterUAS__CameraSystem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::CameraSystem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::CameraSystem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__CameraSystem.stringdata0))
        return static_cast<void*>(this);
    return SensorInterface::qt_metacast(_clname);
}

int CounterUAS::CameraSystem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SensorInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::CameraSystem::frameReady(const QImage & _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::CameraSystem::cameraDetection(const CameraDetection & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::CameraSystem::ptzChanged(const CameraPTZState & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::CameraSystem::slewComplete(const GeoPosition & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::CameraSystem::recordingChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CounterUAS::CameraSystem::streamError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
