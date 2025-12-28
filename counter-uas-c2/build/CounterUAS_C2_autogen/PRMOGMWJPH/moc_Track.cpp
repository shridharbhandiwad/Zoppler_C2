/****************************************************************************
** Meta object code from reading C++ file 'Track.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/Track.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Track.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__Track_t {
    QByteArrayData data[16];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__Track_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__Track_t qt_meta_stringdata_CounterUAS__Track = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CounterUAS::Track"
QT_MOC_LITERAL(1, 18, 15), // "positionChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 15), // "velocityChanged"
QT_MOC_LITERAL(4, 51, 21), // "classificationChanged"
QT_MOC_LITERAL(5, 73, 18), // "threatLevelChanged"
QT_MOC_LITERAL(6, 92, 5), // "level"
QT_MOC_LITERAL(7, 98, 12), // "stateChanged"
QT_MOC_LITERAL(8, 111, 10), // "TrackState"
QT_MOC_LITERAL(9, 122, 5), // "state"
QT_MOC_LITERAL(10, 128, 18), // "boundingBoxChanged"
QT_MOC_LITERAL(11, 147, 7), // "updated"
QT_MOC_LITERAL(12, 155, 7), // "trackId"
QT_MOC_LITERAL(13, 163, 11), // "threatLevel"
QT_MOC_LITERAL(14, 175, 14), // "classification"
QT_MOC_LITERAL(15, 190, 19) // "TrackClassification"

    },
    "CounterUAS::Track\0positionChanged\0\0"
    "velocityChanged\0classificationChanged\0"
    "threatLevelChanged\0level\0stateChanged\0"
    "TrackState\0state\0boundingBoxChanged\0"
    "updated\0trackId\0threatLevel\0classification\0"
    "TrackClassification"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__Track[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       3,   60, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    0,   51,    2, 0x06 /* Public */,
       5,    1,   52,    2, 0x06 /* Public */,
       7,    1,   55,    2, 0x06 /* Public */,
      10,    0,   58,    2, 0x06 /* Public */,
      11,    0,   59,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::QString, 0x00095401,
      13, QMetaType::Int, 0x00495103,
      14, 0x80000000 | 15, 0x0049510b,

 // properties: notify_signal_id
       0,
       3,
       2,

       0        // eod
};

void CounterUAS::Track::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Track *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->positionChanged(); break;
        case 1: _t->velocityChanged(); break;
        case 2: _t->classificationChanged(); break;
        case 3: _t->threatLevelChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->stateChanged((*reinterpret_cast< TrackState(*)>(_a[1]))); break;
        case 5: _t->boundingBoxChanged(); break;
        case 6: _t->updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Track::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Track::positionChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Track::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Track::velocityChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Track::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Track::classificationChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Track::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Track::threatLevelChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Track::*)(TrackState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Track::stateChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Track::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Track::boundingBoxChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Track::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Track::updated)) {
                *result = 6;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Track *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->trackId(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->threatLevel(); break;
        case 2: *reinterpret_cast< TrackClassification*>(_v) = _t->classification(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Track *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setThreatLevel(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setClassification(*reinterpret_cast< TrackClassification*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::Track::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__Track.data,
    qt_meta_data_CounterUAS__Track,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::Track::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::Track::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__Track.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::Track::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CounterUAS::Track::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CounterUAS::Track::velocityChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CounterUAS::Track::classificationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CounterUAS::Track::threatLevelChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::Track::stateChanged(TrackState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CounterUAS::Track::boundingBoxChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CounterUAS::Track::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
