/****************************************************************************
** Meta object code from reading C++ file 'RFJammer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/effectors/RFJammer.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RFJammer.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__RFJammer_t {
    uint offsetsAndSizes[30];
    char stringdata0[21];
    char stringdata1[8];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[7];
    char stringdata5[17];
    char stringdata6[14];
    char stringdata7[12];
    char stringdata8[20];
    char stringdata9[19];
    char stringdata10[18];
    char stringdata11[21];
    char stringdata12[14];
    char stringdata13[29];
    char stringdata14[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__RFJammer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__RFJammer_t qt_meta_stringdata_CounterUAS__RFJammer = {
    {
        QT_MOC_LITERAL(0, 20),  // "CounterUAS::RFJammer"
        QT_MOC_LITERAL(21, 7),  // "jamming"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 6),  // "active"
        QT_MOC_LITERAL(37, 6),  // "powerW"
        QT_MOC_LITERAL(44, 16),  // "frequencyChanged"
        QT_MOC_LITERAL(61, 13),  // "QList<double>"
        QT_MOC_LITERAL(75, 11),  // "frequencies"
        QT_MOC_LITERAL(87, 19),  // "onEngagementTimeout"
        QT_MOC_LITERAL(107, 18),  // "onCooldownComplete"
        QT_MOC_LITERAL(126, 17),  // "onSocketConnected"
        QT_MOC_LITERAL(144, 20),  // "onSocketDisconnected"
        QT_MOC_LITERAL(165, 13),  // "onSocketError"
        QT_MOC_LITERAL(179, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(208, 5)   // "error"
    },
    "CounterUAS::RFJammer",
    "jamming",
    "",
    "active",
    "powerW",
    "frequencyChanged",
    "QList<double>",
    "frequencies",
    "onEngagementTimeout",
    "onCooldownComplete",
    "onSocketConnected",
    "onSocketDisconnected",
    "onSocketError",
    "QAbstractSocket::SocketError",
    "error"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__RFJammer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   56,    2, 0x06,    1 /* Public */,
       5,    1,   61,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   64,    2, 0x08,    6 /* Private */,
       9,    0,   65,    2, 0x08,    7 /* Private */,
      10,    0,   66,    2, 0x08,    8 /* Private */,
      11,    0,   67,    2, 0x08,    9 /* Private */,
      12,    1,   68,    2, 0x08,   10 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Double,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::RFJammer::staticMetaObject = { {
    QMetaObject::SuperData::link<EffectorInterface::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__RFJammer.offsetsAndSizes,
    qt_meta_data_CounterUAS__RFJammer,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__RFJammer_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RFJammer, std::true_type>,
        // method 'jamming'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'frequencyChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<double> &, std::false_type>,
        // method 'onEngagementTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCooldownComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSocketConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSocketDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSocketError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>
    >,
    nullptr
} };

void CounterUAS::RFJammer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RFJammer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->jamming((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 1: _t->frequencyChanged((*reinterpret_cast< std::add_pointer_t<QList<double>>>(_a[1]))); break;
        case 2: _t->onEngagementTimeout(); break;
        case 3: _t->onCooldownComplete(); break;
        case 4: _t->onSocketConnected(); break;
        case 5: _t->onSocketDisconnected(); break;
        case 6: _t->onSocketError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<double> >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RFJammer::*)(bool , double );
            if (_t _q_method = &RFJammer::jamming; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RFJammer::*)(const QList<double> & );
            if (_t _q_method = &RFJammer::frequencyChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *CounterUAS::RFJammer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::RFJammer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__RFJammer.stringdata0))
        return static_cast<void*>(this);
    return EffectorInterface::qt_metacast(_clname);
}

int CounterUAS::RFJammer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EffectorInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::RFJammer::jamming(bool _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::RFJammer::frequencyChanged(const QList<double> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
