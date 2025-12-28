/****************************************************************************
** Meta object code from reading C++ file 'Track.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/Track.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Track.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__Track_t {
    uint offsetsAndSizes[32];
    char stringdata0[18];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[22];
    char stringdata5[19];
    char stringdata6[6];
    char stringdata7[13];
    char stringdata8[11];
    char stringdata9[6];
    char stringdata10[19];
    char stringdata11[8];
    char stringdata12[8];
    char stringdata13[12];
    char stringdata14[15];
    char stringdata15[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__Track_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__Track_t qt_meta_stringdata_CounterUAS__Track = {
    {
        QT_MOC_LITERAL(0, 17),  // "CounterUAS::Track"
        QT_MOC_LITERAL(18, 15),  // "positionChanged"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 15),  // "velocityChanged"
        QT_MOC_LITERAL(51, 21),  // "classificationChanged"
        QT_MOC_LITERAL(73, 18),  // "threatLevelChanged"
        QT_MOC_LITERAL(92, 5),  // "level"
        QT_MOC_LITERAL(98, 12),  // "stateChanged"
        QT_MOC_LITERAL(111, 10),  // "TrackState"
        QT_MOC_LITERAL(122, 5),  // "state"
        QT_MOC_LITERAL(128, 18),  // "boundingBoxChanged"
        QT_MOC_LITERAL(147, 7),  // "updated"
        QT_MOC_LITERAL(155, 7),  // "trackId"
        QT_MOC_LITERAL(163, 11),  // "threatLevel"
        QT_MOC_LITERAL(175, 14),  // "classification"
        QT_MOC_LITERAL(190, 19)   // "TrackClassification"
    },
    "CounterUAS::Track",
    "positionChanged",
    "",
    "velocityChanged",
    "classificationChanged",
    "threatLevelChanged",
    "level",
    "stateChanged",
    "TrackState",
    "state",
    "boundingBoxChanged",
    "updated",
    "trackId",
    "threatLevel",
    "classification",
    "TrackClassification"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__Track[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       3,   67, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    4 /* Public */,
       3,    0,   57,    2, 0x06,    5 /* Public */,
       4,    0,   58,    2, 0x06,    6 /* Public */,
       5,    1,   59,    2, 0x06,    7 /* Public */,
       7,    1,   62,    2, 0x06,    9 /* Public */,
      10,    0,   65,    2, 0x06,   11 /* Public */,
      11,    0,   66,    2, 0x06,   12 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::QString, 0x00015401, uint(-1), 0,
      13, QMetaType::Int, 0x00015103, uint(3), 0,
      14, 0x80000000 | 15, 0x0001510b, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::Track::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__Track.offsetsAndSizes,
    qt_meta_data_CounterUAS__Track,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__Track_t,
        // property 'trackId'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'threatLevel'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'classification'
        QtPrivate::TypeAndForceComplete<TrackClassification, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Track, std::true_type>,
        // method 'positionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'velocityChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'classificationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'threatLevelChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<TrackState, std::false_type>,
        // method 'boundingBoxChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CounterUAS::Track::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Track *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->positionChanged(); break;
        case 1: _t->velocityChanged(); break;
        case 2: _t->classificationChanged(); break;
        case 3: _t->threatLevelChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->stateChanged((*reinterpret_cast< std::add_pointer_t<TrackState>>(_a[1]))); break;
        case 5: _t->boundingBoxChanged(); break;
        case 6: _t->updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Track::*)();
            if (_t _q_method = &Track::positionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Track::*)();
            if (_t _q_method = &Track::velocityChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Track::*)();
            if (_t _q_method = &Track::classificationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Track::*)(int );
            if (_t _q_method = &Track::threatLevelChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Track::*)(TrackState );
            if (_t _q_method = &Track::stateChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Track::*)();
            if (_t _q_method = &Track::boundingBoxChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Track::*)();
            if (_t _q_method = &Track::updated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
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
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
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
