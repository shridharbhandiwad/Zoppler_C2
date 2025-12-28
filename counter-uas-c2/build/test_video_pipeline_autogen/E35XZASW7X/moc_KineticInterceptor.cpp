/****************************************************************************
** Meta object code from reading C++ file 'KineticInterceptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/effectors/KineticInterceptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'KineticInterceptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__KineticInterceptor_t {
    QByteArrayData data[16];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__KineticInterceptor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__KineticInterceptor_t qt_meta_stringdata_CounterUAS__KineticInterceptor = {
    {
QT_MOC_LITERAL(0, 0, 30), // "CounterUAS::KineticInterceptor"
QT_MOC_LITERAL(1, 31, 18), // "launchPhaseChanged"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 11), // "LaunchPhase"
QT_MOC_LITERAL(4, 63, 5), // "phase"
QT_MOC_LITERAL(5, 69, 11), // "roundsFired"
QT_MOC_LITERAL(6, 81, 9), // "remaining"
QT_MOC_LITERAL(7, 91, 13), // "reloadStarted"
QT_MOC_LITERAL(8, 105, 14), // "reloadComplete"
QT_MOC_LITERAL(9, 120, 15), // "interceptResult"
QT_MOC_LITERAL(10, 136, 7), // "success"
QT_MOC_LITERAL(11, 144, 16), // "onArmingComplete"
QT_MOC_LITERAL(12, 161, 16), // "onLaunchComplete"
QT_MOC_LITERAL(13, 178, 15), // "onTerminalPhase"
QT_MOC_LITERAL(14, 194, 19), // "onInterceptComplete"
QT_MOC_LITERAL(15, 214, 16) // "onReloadComplete"

    },
    "CounterUAS::KineticInterceptor\0"
    "launchPhaseChanged\0\0LaunchPhase\0phase\0"
    "roundsFired\0remaining\0reloadStarted\0"
    "reloadComplete\0interceptResult\0success\0"
    "onArmingComplete\0onLaunchComplete\0"
    "onTerminalPhase\0onInterceptComplete\0"
    "onReloadComplete"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__KineticInterceptor[] = {

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
       1,    1,   64,    2, 0x06 /* Public */,
       5,    1,   67,    2, 0x06 /* Public */,
       7,    0,   70,    2, 0x06 /* Public */,
       8,    0,   71,    2, 0x06 /* Public */,
       9,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   75,    2, 0x08 /* Private */,
      12,    0,   76,    2, 0x08 /* Private */,
      13,    0,   77,    2, 0x08 /* Private */,
      14,    0,   78,    2, 0x08 /* Private */,
      15,    0,   79,    2, 0x08 /* Private */,

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

void CounterUAS::KineticInterceptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KineticInterceptor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->launchPhaseChanged((*reinterpret_cast< LaunchPhase(*)>(_a[1]))); break;
        case 1: _t->roundsFired((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->reloadStarted(); break;
        case 3: _t->reloadComplete(); break;
        case 4: _t->interceptResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
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
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KineticInterceptor::launchPhaseChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KineticInterceptor::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KineticInterceptor::roundsFired)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (KineticInterceptor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KineticInterceptor::reloadStarted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (KineticInterceptor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KineticInterceptor::reloadComplete)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (KineticInterceptor::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KineticInterceptor::interceptResult)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::KineticInterceptor::staticMetaObject = { {
    QMetaObject::SuperData::link<EffectorInterface::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__KineticInterceptor.data,
    qt_meta_data_CounterUAS__KineticInterceptor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
            *reinterpret_cast<int*>(_a[0]) = -1;
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
