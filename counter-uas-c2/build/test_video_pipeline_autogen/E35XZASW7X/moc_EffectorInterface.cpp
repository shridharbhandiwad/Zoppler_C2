/****************************************************************************
** Meta object code from reading C++ file 'EffectorInterface.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/effectors/EffectorInterface.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EffectorInterface.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__EffectorInterface_t {
    uint offsetsAndSizes[40];
    char stringdata0[30];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[7];
    char stringdata5[13];
    char stringdata6[6];
    char stringdata7[15];
    char stringdata8[8];
    char stringdata9[14];
    char stringdata10[15];
    char stringdata11[7];
    char stringdata12[18];
    char stringdata13[12];
    char stringdata14[7];
    char stringdata15[19];
    char stringdata16[8];
    char stringdata17[6];
    char stringdata18[8];
    char stringdata19[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__EffectorInterface_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__EffectorInterface_t qt_meta_stringdata_CounterUAS__EffectorInterface = {
    {
        QT_MOC_LITERAL(0, 29),  // "CounterUAS::EffectorInterface"
        QT_MOC_LITERAL(30, 13),  // "statusChanged"
        QT_MOC_LITERAL(44, 0),  // ""
        QT_MOC_LITERAL(45, 14),  // "EffectorStatus"
        QT_MOC_LITERAL(60, 6),  // "status"
        QT_MOC_LITERAL(67, 12),  // "readyChanged"
        QT_MOC_LITERAL(80, 5),  // "ready"
        QT_MOC_LITERAL(86, 14),  // "engagedChanged"
        QT_MOC_LITERAL(101, 7),  // "engaged"
        QT_MOC_LITERAL(109, 13),  // "healthUpdated"
        QT_MOC_LITERAL(123, 14),  // "EffectorHealth"
        QT_MOC_LITERAL(138, 6),  // "health"
        QT_MOC_LITERAL(145, 17),  // "engagementStarted"
        QT_MOC_LITERAL(163, 11),  // "GeoPosition"
        QT_MOC_LITERAL(175, 6),  // "target"
        QT_MOC_LITERAL(182, 18),  // "engagementComplete"
        QT_MOC_LITERAL(201, 7),  // "success"
        QT_MOC_LITERAL(209, 5),  // "fault"
        QT_MOC_LITERAL(215, 7),  // "message"
        QT_MOC_LITERAL(223, 10)   // "effectorId"
    },
    "CounterUAS::EffectorInterface",
    "statusChanged",
    "",
    "EffectorStatus",
    "status",
    "readyChanged",
    "ready",
    "engagedChanged",
    "engaged",
    "healthUpdated",
    "EffectorHealth",
    "health",
    "engagementStarted",
    "GeoPosition",
    "target",
    "engagementComplete",
    "success",
    "fault",
    "message",
    "effectorId"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__EffectorInterface[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       4,   77, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    5 /* Public */,
       5,    1,   59,    2, 0x06,    7 /* Public */,
       7,    1,   62,    2, 0x06,    9 /* Public */,
       9,    1,   65,    2, 0x06,   11 /* Public */,
      12,    1,   68,    2, 0x06,   13 /* Public */,
      15,    1,   71,    2, 0x06,   15 /* Public */,
      17,    1,   74,    2, 0x06,   17 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void, QMetaType::QString,   18,

 // properties: name, type, flags
      19, QMetaType::QString, 0x00015401, uint(-1), 0,
       4, 0x80000000 | 3, 0x00015009, uint(0), 0,
       6, QMetaType::Bool, 0x00015001, uint(1), 0,
       8, QMetaType::Bool, 0x00015001, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::EffectorInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__EffectorInterface.offsetsAndSizes,
    qt_meta_data_CounterUAS__EffectorInterface,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__EffectorInterface_t,
        // property 'effectorId'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'status'
        QtPrivate::TypeAndForceComplete<EffectorStatus, std::true_type>,
        // property 'ready'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'engaged'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<EffectorInterface, std::true_type>,
        // method 'statusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<EffectorStatus, std::false_type>,
        // method 'readyChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'engagedChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'healthUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const EffectorHealth &, std::false_type>,
        // method 'engagementStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const GeoPosition &, std::false_type>,
        // method 'engagementComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'fault'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void CounterUAS::EffectorInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EffectorInterface *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast< std::add_pointer_t<EffectorStatus>>(_a[1]))); break;
        case 1: _t->readyChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->engagedChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->healthUpdated((*reinterpret_cast< std::add_pointer_t<EffectorHealth>>(_a[1]))); break;
        case 4: _t->engagementStarted((*reinterpret_cast< std::add_pointer_t<GeoPosition>>(_a[1]))); break;
        case 5: _t->engagementComplete((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->fault((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EffectorInterface::*)(EffectorStatus );
            if (_t _q_method = &EffectorInterface::statusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (EffectorInterface::*)(bool );
            if (_t _q_method = &EffectorInterface::readyChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (EffectorInterface::*)(bool );
            if (_t _q_method = &EffectorInterface::engagedChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (EffectorInterface::*)(const EffectorHealth & );
            if (_t _q_method = &EffectorInterface::healthUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (EffectorInterface::*)(const GeoPosition & );
            if (_t _q_method = &EffectorInterface::engagementStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (EffectorInterface::*)(bool );
            if (_t _q_method = &EffectorInterface::engagementComplete; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (EffectorInterface::*)(const QString & );
            if (_t _q_method = &EffectorInterface::fault; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<EffectorInterface *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->effectorId(); break;
        case 1: *reinterpret_cast< EffectorStatus*>(_v) = _t->status(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isReady(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isEngaged(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *CounterUAS::EffectorInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::EffectorInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__EffectorInterface.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::EffectorInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CounterUAS::EffectorInterface::statusChanged(EffectorStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::EffectorInterface::readyChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::EffectorInterface::engagedChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::EffectorInterface::healthUpdated(const EffectorHealth & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::EffectorInterface::engagementStarted(const GeoPosition & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CounterUAS::EffectorInterface::engagementComplete(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CounterUAS::EffectorInterface::fault(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
