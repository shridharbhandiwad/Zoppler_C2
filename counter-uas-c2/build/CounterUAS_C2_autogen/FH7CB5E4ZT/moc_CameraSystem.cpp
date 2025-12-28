/****************************************************************************
** Meta object code from reading C++ file 'CameraSystem.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/sensors/CameraSystem.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CameraSystem.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__CameraSystem_t {
    uint offsetsAndSizes[42];
    char stringdata0[25];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[12];
    char stringdata5[16];
    char stringdata6[16];
    char stringdata7[10];
    char stringdata8[11];
    char stringdata9[15];
    char stringdata10[6];
    char stringdata11[13];
    char stringdata12[12];
    char stringdata13[7];
    char stringdata14[17];
    char stringdata15[10];
    char stringdata16[12];
    char stringdata17[6];
    char stringdata18[12];
    char stringdata19[16];
    char stringdata20[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__CameraSystem_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__CameraSystem_t qt_meta_stringdata_CounterUAS__CameraSystem = {
    {
        QT_MOC_LITERAL(0, 24),  // "CounterUAS::CameraSystem"
        QT_MOC_LITERAL(25, 10),  // "frameReady"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 5),  // "frame"
        QT_MOC_LITERAL(43, 11),  // "frameNumber"
        QT_MOC_LITERAL(55, 15),  // "cameraDetection"
        QT_MOC_LITERAL(71, 15),  // "CameraDetection"
        QT_MOC_LITERAL(87, 9),  // "detection"
        QT_MOC_LITERAL(97, 10),  // "ptzChanged"
        QT_MOC_LITERAL(108, 14),  // "CameraPTZState"
        QT_MOC_LITERAL(123, 5),  // "state"
        QT_MOC_LITERAL(129, 12),  // "slewComplete"
        QT_MOC_LITERAL(142, 11),  // "GeoPosition"
        QT_MOC_LITERAL(154, 6),  // "target"
        QT_MOC_LITERAL(161, 16),  // "recordingChanged"
        QT_MOC_LITERAL(178, 9),  // "recording"
        QT_MOC_LITERAL(188, 11),  // "streamError"
        QT_MOC_LITERAL(200, 5),  // "error"
        QT_MOC_LITERAL(206, 11),  // "processData"
        QT_MOC_LITERAL(218, 15),  // "onFrameReceived"
        QT_MOC_LITERAL(234, 18)   // "updateSlewProgress"
    },
    "CounterUAS::CameraSystem",
    "frameReady",
    "",
    "frame",
    "frameNumber",
    "cameraDetection",
    "CameraDetection",
    "detection",
    "ptzChanged",
    "CameraPTZState",
    "state",
    "slewComplete",
    "GeoPosition",
    "target",
    "recordingChanged",
    "recording",
    "streamError",
    "error",
    "processData",
    "onFrameReceived",
    "updateSlewProgress"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__CameraSystem[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   68,    2, 0x06,    1 /* Public */,
       5,    1,   73,    2, 0x06,    4 /* Public */,
       8,    1,   76,    2, 0x06,    6 /* Public */,
      11,    1,   79,    2, 0x06,    8 /* Public */,
      14,    1,   82,    2, 0x06,   10 /* Public */,
      16,    1,   85,    2, 0x06,   12 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      18,    0,   88,    2, 0x09,   14 /* Protected */,
      19,    1,   89,    2, 0x08,   15 /* Private */,
      20,    0,   92,    2, 0x08,   17 /* Private */,

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

Q_CONSTINIT const QMetaObject CounterUAS::CameraSystem::staticMetaObject = { {
    QMetaObject::SuperData::link<SensorInterface::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__CameraSystem.offsetsAndSizes,
    qt_meta_data_CounterUAS__CameraSystem,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__CameraSystem_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CameraSystem, std::true_type>,
        // method 'frameReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'cameraDetection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CameraDetection &, std::false_type>,
        // method 'ptzChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CameraPTZState &, std::false_type>,
        // method 'slewComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const GeoPosition &, std::false_type>,
        // method 'recordingChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'streamError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'processData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFrameReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        // method 'updateSlewProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CounterUAS::CameraSystem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CameraSystem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        case 1: _t->cameraDetection((*reinterpret_cast< std::add_pointer_t<CameraDetection>>(_a[1]))); break;
        case 2: _t->ptzChanged((*reinterpret_cast< std::add_pointer_t<CameraPTZState>>(_a[1]))); break;
        case 3: _t->slewComplete((*reinterpret_cast< std::add_pointer_t<GeoPosition>>(_a[1]))); break;
        case 4: _t->recordingChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->streamError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->processData(); break;
        case 7: _t->onFrameReceived((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1]))); break;
        case 8: _t->updateSlewProgress(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CameraSystem::*)(const QImage & , qint64 );
            if (_t _q_method = &CameraSystem::frameReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(const CameraDetection & );
            if (_t _q_method = &CameraSystem::cameraDetection; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(const CameraPTZState & );
            if (_t _q_method = &CameraSystem::ptzChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(const GeoPosition & );
            if (_t _q_method = &CameraSystem::slewComplete; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(bool );
            if (_t _q_method = &CameraSystem::recordingChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CameraSystem::*)(const QString & );
            if (_t _q_method = &CameraSystem::streamError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
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
