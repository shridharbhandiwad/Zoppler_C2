/****************************************************************************
** Meta object code from reading C++ file 'RFJammer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/effectors/RFJammer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RFJammer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__RFJammer_t {
    QByteArrayData data[15];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__RFJammer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__RFJammer_t qt_meta_stringdata_CounterUAS__RFJammer = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CounterUAS::RFJammer"
QT_MOC_LITERAL(1, 21, 7), // "jamming"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 6), // "active"
QT_MOC_LITERAL(4, 37, 6), // "powerW"
QT_MOC_LITERAL(5, 44, 16), // "frequencyChanged"
QT_MOC_LITERAL(6, 61, 13), // "QList<double>"
QT_MOC_LITERAL(7, 75, 11), // "frequencies"
QT_MOC_LITERAL(8, 87, 19), // "onEngagementTimeout"
QT_MOC_LITERAL(9, 107, 18), // "onCooldownComplete"
QT_MOC_LITERAL(10, 126, 17), // "onSocketConnected"
QT_MOC_LITERAL(11, 144, 20), // "onSocketDisconnected"
QT_MOC_LITERAL(12, 165, 13), // "onSocketError"
QT_MOC_LITERAL(13, 179, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(14, 208, 5) // "error"

    },
    "CounterUAS::RFJammer\0jamming\0\0active\0"
    "powerW\0frequencyChanged\0QList<double>\0"
    "frequencies\0onEngagementTimeout\0"
    "onCooldownComplete\0onSocketConnected\0"
    "onSocketDisconnected\0onSocketError\0"
    "QAbstractSocket::SocketError\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__RFJammer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       5,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   57,    2, 0x08 /* Private */,
       9,    0,   58,    2, 0x08 /* Private */,
      10,    0,   59,    2, 0x08 /* Private */,
      11,    0,   60,    2, 0x08 /* Private */,
      12,    1,   61,    2, 0x08 /* Private */,

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

void CounterUAS::RFJammer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RFJammer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->jamming((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->frequencyChanged((*reinterpret_cast< const QList<double>(*)>(_a[1]))); break;
        case 2: _t->onEngagementTimeout(); break;
        case 3: _t->onCooldownComplete(); break;
        case 4: _t->onSocketConnected(); break;
        case 5: _t->onSocketDisconnected(); break;
        case 6: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<double> >(); break;
            }
            break;
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
            using _t = void (RFJammer::*)(bool , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RFJammer::jamming)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RFJammer::*)(const QList<double> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RFJammer::frequencyChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::RFJammer::staticMetaObject = { {
    QMetaObject::SuperData::link<EffectorInterface::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__RFJammer.data,
    qt_meta_data_CounterUAS__RFJammer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
