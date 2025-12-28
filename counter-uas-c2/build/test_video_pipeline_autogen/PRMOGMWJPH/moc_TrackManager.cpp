/****************************************************************************
** Meta object code from reading C++ file 'TrackManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/TrackManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TrackManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__TrackManager_t {
    QByteArrayData data[29];
    char stringdata0[365];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__TrackManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__TrackManager_t qt_meta_stringdata_CounterUAS__TrackManager = {
    {
QT_MOC_LITERAL(0, 0, 24), // "CounterUAS::TrackManager"
QT_MOC_LITERAL(1, 25, 12), // "trackCreated"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 7), // "trackId"
QT_MOC_LITERAL(4, 47, 12), // "trackUpdated"
QT_MOC_LITERAL(5, 60, 26), // "trackClassificationChanged"
QT_MOC_LITERAL(6, 87, 19), // "TrackClassification"
QT_MOC_LITERAL(7, 107, 3), // "cls"
QT_MOC_LITERAL(8, 111, 23), // "trackThreatLevelChanged"
QT_MOC_LITERAL(9, 135, 5), // "level"
QT_MOC_LITERAL(10, 141, 17), // "trackStateChanged"
QT_MOC_LITERAL(11, 159, 10), // "TrackState"
QT_MOC_LITERAL(12, 170, 5), // "state"
QT_MOC_LITERAL(13, 176, 12), // "trackDropped"
QT_MOC_LITERAL(14, 189, 17), // "trackCountChanged"
QT_MOC_LITERAL(15, 207, 5), // "count"
QT_MOC_LITERAL(16, 213, 18), // "highThreatDetected"
QT_MOC_LITERAL(17, 232, 14), // "runningChanged"
QT_MOC_LITERAL(18, 247, 7), // "running"
QT_MOC_LITERAL(19, 255, 12), // "onSensorData"
QT_MOC_LITERAL(20, 268, 11), // "GeoPosition"
QT_MOC_LITERAL(21, 280, 3), // "pos"
QT_MOC_LITERAL(22, 284, 14), // "VelocityVector"
QT_MOC_LITERAL(23, 299, 3), // "vel"
QT_MOC_LITERAL(24, 303, 15), // "DetectionSource"
QT_MOC_LITERAL(25, 319, 6), // "source"
QT_MOC_LITERAL(26, 326, 9), // "timestamp"
QT_MOC_LITERAL(27, 336, 17), // "processTrackCycle"
QT_MOC_LITERAL(28, 354, 10) // "trackCount"

    },
    "CounterUAS::TrackManager\0trackCreated\0"
    "\0trackId\0trackUpdated\0trackClassificationChanged\0"
    "TrackClassification\0cls\0trackThreatLevelChanged\0"
    "level\0trackStateChanged\0TrackState\0"
    "state\0trackDropped\0trackCountChanged\0"
    "count\0highThreatDetected\0runningChanged\0"
    "running\0onSensorData\0GeoPosition\0pos\0"
    "VelocityVector\0vel\0DetectionSource\0"
    "source\0timestamp\0processTrackCycle\0"
    "trackCount"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__TrackManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       2,  114, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       5,    2,   75,    2, 0x06 /* Public */,
       8,    2,   80,    2, 0x06 /* Public */,
      10,    2,   85,    2, 0x06 /* Public */,
      13,    1,   90,    2, 0x06 /* Public */,
      14,    1,   93,    2, 0x06 /* Public */,
      16,    2,   96,    2, 0x06 /* Public */,
      17,    1,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    4,  104,    2, 0x0a /* Public */,
      27,    0,  113,    2, 0x08 /* Private */,

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
      28, QMetaType::Int, 0x00495001,
      18, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       6,
       8,

       0        // eod
};

void CounterUAS::TrackManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TrackManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->trackCreated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->trackUpdated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->trackClassificationChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< TrackClassification(*)>(_a[2]))); break;
        case 3: _t->trackThreatLevelChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->trackStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< TrackState(*)>(_a[2]))); break;
        case 5: _t->trackDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->trackCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->highThreatDetected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->runningChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->onSensorData((*reinterpret_cast< const GeoPosition(*)>(_a[1])),(*reinterpret_cast< const VelocityVector(*)>(_a[2])),(*reinterpret_cast< DetectionSource(*)>(_a[3])),(*reinterpret_cast< qint64(*)>(_a[4]))); break;
        case 10: _t->processTrackCycle(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TrackManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::trackCreated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::trackUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & , TrackClassification );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::trackClassificationChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::trackThreatLevelChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & , TrackState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::trackStateChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::trackDropped)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::trackCountChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::highThreatDetected)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TrackManager::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackManager::runningChanged)) {
                *result = 8;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
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
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::TrackManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__TrackManager.data,
    qt_meta_data_CounterUAS__TrackManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
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
