/****************************************************************************
** Meta object code from reading C++ file 'SensorInterface.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/sensors/SensorInterface.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SensorInterface.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__SensorInterface_t {
    uint offsetsAndSizes[36];
    char stringdata0[28];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[14];
    char stringdata5[13];
    char stringdata6[7];
    char stringdata7[17];
    char stringdata8[10];
    char stringdata9[14];
    char stringdata10[13];
    char stringdata11[7];
    char stringdata12[6];
    char stringdata13[8];
    char stringdata14[12];
    char stringdata15[13];
    char stringdata16[9];
    char stringdata17[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__SensorInterface_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__SensorInterface_t qt_meta_stringdata_CounterUAS__SensorInterface = {
    {
        QT_MOC_LITERAL(0, 27),  // "CounterUAS::SensorInterface"
        QT_MOC_LITERAL(28, 9),  // "detection"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 15),  // "SensorDetection"
        QT_MOC_LITERAL(55, 13),  // "statusChanged"
        QT_MOC_LITERAL(69, 12),  // "SensorStatus"
        QT_MOC_LITERAL(82, 6),  // "status"
        QT_MOC_LITERAL(89, 16),  // "connectedChanged"
        QT_MOC_LITERAL(106, 9),  // "connected"
        QT_MOC_LITERAL(116, 13),  // "healthUpdated"
        QT_MOC_LITERAL(130, 12),  // "SensorHealth"
        QT_MOC_LITERAL(143, 6),  // "health"
        QT_MOC_LITERAL(150, 5),  // "error"
        QT_MOC_LITERAL(156, 7),  // "message"
        QT_MOC_LITERAL(164, 11),  // "processData"
        QT_MOC_LITERAL(176, 12),  // "updateHealth"
        QT_MOC_LITERAL(189, 8),  // "sensorId"
        QT_MOC_LITERAL(198, 10)   // "sensorType"
    },
    "CounterUAS::SensorInterface",
    "detection",
    "",
    "SensorDetection",
    "statusChanged",
    "SensorStatus",
    "status",
    "connectedChanged",
    "connected",
    "healthUpdated",
    "SensorHealth",
    "health",
    "error",
    "message",
    "processData",
    "updateHealth",
    "sensorId",
    "sensorType"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__SensorInterface[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       4,   73, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    5 /* Public */,
       4,    1,   59,    2, 0x06,    7 /* Public */,
       7,    1,   62,    2, 0x06,    9 /* Public */,
       9,    1,   65,    2, 0x06,   11 /* Public */,
      12,    1,   68,    2, 0x06,   13 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    0,   71,    2, 0x09,   15 /* Protected */,
      15,    0,   72,    2, 0x09,   16 /* Protected */,

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
      16, QMetaType::QString, 0x00015401, uint(-1), 0,
      17, QMetaType::QString, 0x00015401, uint(-1), 0,
       6, 0x80000000 | 5, 0x00015009, uint(1), 0,
       8, QMetaType::Bool, 0x00015001, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::SensorInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__SensorInterface.offsetsAndSizes,
    qt_meta_data_CounterUAS__SensorInterface,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__SensorInterface_t,
        // property 'sensorId'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'sensorType'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'status'
        QtPrivate::TypeAndForceComplete<SensorStatus, std::true_type>,
        // property 'connected'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SensorInterface, std::true_type>,
        // method 'detection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const SensorDetection &, std::false_type>,
        // method 'statusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<SensorStatus, std::false_type>,
        // method 'connectedChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'healthUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const SensorHealth &, std::false_type>,
        // method 'error'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'processData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateHealth'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CounterUAS::SensorInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SensorInterface *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->detection((*reinterpret_cast< std::add_pointer_t<SensorDetection>>(_a[1]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< std::add_pointer_t<SensorStatus>>(_a[1]))); break;
        case 2: _t->connectedChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->healthUpdated((*reinterpret_cast< std::add_pointer_t<SensorHealth>>(_a[1]))); break;
        case 4: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->processData(); break;
        case 6: _t->updateHealth(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SensorInterface::*)(const SensorDetection & );
            if (_t _q_method = &SensorInterface::detection; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SensorInterface::*)(SensorStatus );
            if (_t _q_method = &SensorInterface::statusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SensorInterface::*)(bool );
            if (_t _q_method = &SensorInterface::connectedChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SensorInterface::*)(const SensorHealth & );
            if (_t _q_method = &SensorInterface::healthUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SensorInterface::*)(const QString & );
            if (_t _q_method = &SensorInterface::error; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
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
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
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
