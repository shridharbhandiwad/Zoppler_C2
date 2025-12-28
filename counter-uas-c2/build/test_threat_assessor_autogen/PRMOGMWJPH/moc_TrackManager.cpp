/****************************************************************************
** Meta object code from reading C++ file 'TrackManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/TrackManager.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TrackManager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__TrackManager_t {
    uint offsetsAndSizes[58];
    char stringdata0[25];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[13];
    char stringdata5[27];
    char stringdata6[20];
    char stringdata7[4];
    char stringdata8[24];
    char stringdata9[6];
    char stringdata10[18];
    char stringdata11[11];
    char stringdata12[6];
    char stringdata13[13];
    char stringdata14[18];
    char stringdata15[6];
    char stringdata16[19];
    char stringdata17[15];
    char stringdata18[8];
    char stringdata19[13];
    char stringdata20[12];
    char stringdata21[4];
    char stringdata22[15];
    char stringdata23[4];
    char stringdata24[16];
    char stringdata25[7];
    char stringdata26[10];
    char stringdata27[18];
    char stringdata28[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__TrackManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__TrackManager_t qt_meta_stringdata_CounterUAS__TrackManager = {
    {
        QT_MOC_LITERAL(0, 24),  // "CounterUAS::TrackManager"
        QT_MOC_LITERAL(25, 12),  // "trackCreated"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 7),  // "trackId"
        QT_MOC_LITERAL(47, 12),  // "trackUpdated"
        QT_MOC_LITERAL(60, 26),  // "trackClassificationChanged"
        QT_MOC_LITERAL(87, 19),  // "TrackClassification"
        QT_MOC_LITERAL(107, 3),  // "cls"
        QT_MOC_LITERAL(111, 23),  // "trackThreatLevelChanged"
        QT_MOC_LITERAL(135, 5),  // "level"
        QT_MOC_LITERAL(141, 17),  // "trackStateChanged"
        QT_MOC_LITERAL(159, 10),  // "TrackState"
        QT_MOC_LITERAL(170, 5),  // "state"
        QT_MOC_LITERAL(176, 12),  // "trackDropped"
        QT_MOC_LITERAL(189, 17),  // "trackCountChanged"
        QT_MOC_LITERAL(207, 5),  // "count"
        QT_MOC_LITERAL(213, 18),  // "highThreatDetected"
        QT_MOC_LITERAL(232, 14),  // "runningChanged"
        QT_MOC_LITERAL(247, 7),  // "running"
        QT_MOC_LITERAL(255, 12),  // "onSensorData"
        QT_MOC_LITERAL(268, 11),  // "GeoPosition"
        QT_MOC_LITERAL(280, 3),  // "pos"
        QT_MOC_LITERAL(284, 14),  // "VelocityVector"
        QT_MOC_LITERAL(299, 3),  // "vel"
        QT_MOC_LITERAL(303, 15),  // "DetectionSource"
        QT_MOC_LITERAL(319, 6),  // "source"
        QT_MOC_LITERAL(326, 9),  // "timestamp"
        QT_MOC_LITERAL(336, 17),  // "processTrackCycle"
        QT_MOC_LITERAL(354, 10)   // "trackCount"
    },
    "CounterUAS::TrackManager",
    "trackCreated",
    "",
    "trackId",
    "trackUpdated",
    "trackClassificationChanged",
    "TrackClassification",
    "cls",
    "trackThreatLevelChanged",
    "level",
    "trackStateChanged",
    "TrackState",
    "state",
    "trackDropped",
    "trackCountChanged",
    "count",
    "highThreatDetected",
    "runningChanged",
    "running",
    "onSensorData",
    "GeoPosition",
    "pos",
    "VelocityVector",
    "vel",
    "DetectionSource",
    "source",
    "timestamp",
    "processTrackCycle",
    "trackCount"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__TrackManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       2,  125, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    3 /* Public */,
       4,    1,   83,    2, 0x06,    5 /* Public */,
       5,    2,   86,    2, 0x06,    7 /* Public */,
       8,    2,   91,    2, 0x06,   10 /* Public */,
      10,    2,   96,    2, 0x06,   13 /* Public */,
      13,    1,  101,    2, 0x06,   16 /* Public */,
      14,    1,  104,    2, 0x06,   18 /* Public */,
      16,    2,  107,    2, 0x06,   20 /* Public */,
      17,    1,  112,    2, 0x06,   23 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      19,    4,  115,    2, 0x0a,   25 /* Public */,
      27,    0,  124,    2, 0x08,   30 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6,    3,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    9,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 11,    3,   12,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    9,
    QMetaType::Void, QMetaType::Bool,   18,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 20, 0x80000000 | 22, 0x80000000 | 24, QMetaType::LongLong,   21,   23,   25,   26,
    QMetaType::Void,

 // properties: name, type, flags
      28, QMetaType::Int, 0x00015001, uint(6), 0,
      18, QMetaType::Bool, 0x00015001, uint(8), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::TrackManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__TrackManager.offsetsAndSizes,
    qt_meta_data_CounterUAS__TrackManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__TrackManager_t,
        // property 'trackCount'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'running'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TrackManager, std::true_type>,
        // method 'trackCreated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'trackUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'trackClassificationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<TrackClassification, std::false_type>,
        // method 'trackThreatLevelChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'trackStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<TrackState, std::false_type>,
        // method 'trackDropped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'trackCountChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'highThreatDetected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'runningChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onSensorData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const GeoPosition &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const VelocityVector &, std::false_type>,
        QtPrivate::TypeAndForceComplete<DetectionSource, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'processTrackCycle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CounterUAS::TrackManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TrackManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->trackCreated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->trackUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->trackClassificationChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<TrackClassification>>(_a[2]))); break;
        case 3: _t->trackThreatLevelChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->trackStateChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<TrackState>>(_a[2]))); break;
        case 5: _t->trackDropped((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->trackCountChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->highThreatDetected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->runningChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->onSensorData((*reinterpret_cast< std::add_pointer_t<GeoPosition>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<VelocityVector>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<DetectionSource>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[4]))); break;
        case 10: _t->processTrackCycle(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TrackManager::*)(const QString & );
            if (_t _q_method = &TrackManager::trackCreated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & );
            if (_t _q_method = &TrackManager::trackUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & , TrackClassification );
            if (_t _q_method = &TrackManager::trackClassificationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & , int );
            if (_t _q_method = &TrackManager::trackThreatLevelChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & , TrackState );
            if (_t _q_method = &TrackManager::trackStateChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & );
            if (_t _q_method = &TrackManager::trackDropped; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(int );
            if (_t _q_method = &TrackManager::trackCountChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & , int );
            if (_t _q_method = &TrackManager::highThreatDetected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(bool );
            if (_t _q_method = &TrackManager::runningChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TrackManager *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->trackCount(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isRunning(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *CounterUAS::TrackManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::TrackManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__TrackManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::TrackManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::TrackManager::trackCreated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::TrackManager::trackUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::TrackManager::trackClassificationChanged(const QString & _t1, TrackClassification _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::TrackManager::trackThreatLevelChanged(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::TrackManager::trackStateChanged(const QString & _t1, TrackState _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CounterUAS::TrackManager::trackDropped(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CounterUAS::TrackManager::trackCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CounterUAS::TrackManager::highThreatDetected(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CounterUAS::TrackManager::runningChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
