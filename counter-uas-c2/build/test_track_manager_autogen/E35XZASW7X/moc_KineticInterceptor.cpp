/****************************************************************************
** Meta object code from reading C++ file 'KineticInterceptor.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/effectors/KineticInterceptor.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'KineticInterceptor.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__KineticInterceptor_t {
    uint offsetsAndSizes[32];
    char stringdata0[31];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[6];
    char stringdata5[12];
    char stringdata6[10];
    char stringdata7[14];
    char stringdata8[15];
    char stringdata9[16];
    char stringdata10[8];
    char stringdata11[17];
    char stringdata12[17];
    char stringdata13[16];
    char stringdata14[20];
    char stringdata15[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__KineticInterceptor_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__KineticInterceptor_t qt_meta_stringdata_CounterUAS__KineticInterceptor = {
    {
        QT_MOC_LITERAL(0, 30),  // "CounterUAS::KineticInterceptor"
        QT_MOC_LITERAL(31, 18),  // "launchPhaseChanged"
        QT_MOC_LITERAL(50, 0),  // ""
        QT_MOC_LITERAL(51, 11),  // "LaunchPhase"
        QT_MOC_LITERAL(63, 5),  // "phase"
        QT_MOC_LITERAL(69, 11),  // "roundsFired"
        QT_MOC_LITERAL(81, 9),  // "remaining"
        QT_MOC_LITERAL(91, 13),  // "reloadStarted"
        QT_MOC_LITERAL(105, 14),  // "reloadComplete"
        QT_MOC_LITERAL(120, 15),  // "interceptResult"
        QT_MOC_LITERAL(136, 7),  // "success"
        QT_MOC_LITERAL(144, 16),  // "onArmingComplete"
        QT_MOC_LITERAL(161, 16),  // "onLaunchComplete"
        QT_MOC_LITERAL(178, 15),  // "onTerminalPhase"
        QT_MOC_LITERAL(194, 19),  // "onInterceptComplete"
        QT_MOC_LITERAL(214, 16)   // "onReloadComplete"
    },
    "CounterUAS::KineticInterceptor",
    "launchPhaseChanged",
    "",
    "LaunchPhase",
    "phase",
    "roundsFired",
    "remaining",
    "reloadStarted",
    "reloadComplete",
    "interceptResult",
    "success",
    "onArmingComplete",
    "onLaunchComplete",
    "onTerminalPhase",
    "onInterceptComplete",
    "onReloadComplete"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__KineticInterceptor[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,
       5,    1,   77,    2, 0x06,    3 /* Public */,
       7,    0,   80,    2, 0x06,    5 /* Public */,
       8,    0,   81,    2, 0x06,    6 /* Public */,
       9,    1,   82,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    0,   85,    2, 0x08,    9 /* Private */,
      12,    0,   86,    2, 0x08,   10 /* Private */,
      13,    0,   87,    2, 0x08,   11 /* Private */,
      14,    0,   88,    2, 0x08,   12 /* Private */,
      15,    0,   89,    2, 0x08,   13 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::KineticInterceptor::staticMetaObject = { {
    QMetaObject::SuperData::link<EffectorInterface::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__KineticInterceptor.offsetsAndSizes,
    qt_meta_data_CounterUAS__KineticInterceptor,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__KineticInterceptor_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<KineticInterceptor, std::true_type>,
        // method 'launchPhaseChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<LaunchPhase, std::false_type>,
        // method 'roundsFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'reloadStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reloadComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'interceptResult'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onArmingComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLaunchComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTerminalPhase'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onInterceptComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReloadComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CounterUAS::KineticInterceptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KineticInterceptor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->launchPhaseChanged((*reinterpret_cast< std::add_pointer_t<LaunchPhase>>(_a[1]))); break;
        case 1: _t->roundsFired((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->reloadStarted(); break;
        case 3: _t->reloadComplete(); break;
        case 4: _t->interceptResult((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->onArmingComplete(); break;
        case 6: _t->onLaunchComplete(); break;
        case 7: _t->onTerminalPhase(); break;
        case 8: _t->onInterceptComplete(); break;
        case 9: _t->onReloadComplete(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KineticInterceptor::*)(LaunchPhase );
            if (_t _q_method = &KineticInterceptor::launchPhaseChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KineticInterceptor::*)(int );
            if (_t _q_method = &KineticInterceptor::roundsFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (KineticInterceptor::*)();
            if (_t _q_method = &KineticInterceptor::reloadStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (KineticInterceptor::*)();
            if (_t _q_method = &KineticInterceptor::reloadComplete; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (KineticInterceptor::*)(bool );
            if (_t _q_method = &KineticInterceptor::interceptResult; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *CounterUAS::KineticInterceptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::KineticInterceptor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__KineticInterceptor.stringdata0))
        return static_cast<void*>(this);
    return EffectorInterface::qt_metacast(_clname);
}

int CounterUAS::KineticInterceptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EffectorInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::KineticInterceptor::launchPhaseChanged(LaunchPhase _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::KineticInterceptor::roundsFired(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::KineticInterceptor::reloadStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CounterUAS::KineticInterceptor::reloadComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CounterUAS::KineticInterceptor::interceptResult(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
