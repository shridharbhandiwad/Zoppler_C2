/****************************************************************************
** Meta object code from reading C++ file 'ThreatAssessor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/ThreatAssessor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ThreatAssessor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__ThreatAssessor_t {
    QByteArrayData data[23];
    char stringdata0[297];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__ThreatAssessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__ThreatAssessor_t qt_meta_stringdata_CounterUAS__ThreatAssessor = {
    {
QT_MOC_LITERAL(0, 0, 26), // "CounterUAS::ThreatAssessor"
QT_MOC_LITERAL(1, 27, 18), // "threatLevelChanged"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 7), // "trackId"
QT_MOC_LITERAL(4, 55, 8), // "oldLevel"
QT_MOC_LITERAL(5, 64, 8), // "newLevel"
QT_MOC_LITERAL(6, 73, 8), // "newAlert"
QT_MOC_LITERAL(7, 82, 11), // "ThreatAlert"
QT_MOC_LITERAL(8, 94, 5), // "alert"
QT_MOC_LITERAL(9, 100, 17), // "alertAcknowledged"
QT_MOC_LITERAL(10, 118, 7), // "alertId"
QT_MOC_LITERAL(11, 126, 18), // "highThreatDetected"
QT_MOC_LITERAL(12, 145, 14), // "metricsUpdated"
QT_MOC_LITERAL(13, 160, 13), // "ThreatMetrics"
QT_MOC_LITERAL(14, 174, 7), // "metrics"
QT_MOC_LITERAL(15, 182, 18), // "assessmentComplete"
QT_MOC_LITERAL(16, 201, 17), // "slewCameraRequest"
QT_MOC_LITERAL(17, 219, 8), // "cameraId"
QT_MOC_LITERAL(18, 228, 11), // "GeoPosition"
QT_MOC_LITERAL(19, 240, 3), // "pos"
QT_MOC_LITERAL(20, 244, 14), // "onTrackUpdated"
QT_MOC_LITERAL(21, 259, 14), // "onTrackCreated"
QT_MOC_LITERAL(22, 274, 22) // "performAssessmentCycle"

    },
    "CounterUAS::ThreatAssessor\0"
    "threatLevelChanged\0\0trackId\0oldLevel\0"
    "newLevel\0newAlert\0ThreatAlert\0alert\0"
    "alertAcknowledged\0alertId\0highThreatDetected\0"
    "metricsUpdated\0ThreatMetrics\0metrics\0"
    "assessmentComplete\0slewCameraRequest\0"
    "cameraId\0GeoPosition\0pos\0onTrackUpdated\0"
    "onTrackCreated\0performAssessmentCycle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__ThreatAssessor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   64,    2, 0x06 /* Public */,
       6,    1,   71,    2, 0x06 /* Public */,
       9,    1,   74,    2, 0x06 /* Public */,
      11,    1,   77,    2, 0x06 /* Public */,
      12,    1,   80,    2, 0x06 /* Public */,
      15,    0,   83,    2, 0x06 /* Public */,
      16,    2,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    1,   89,    2, 0x0a /* Public */,
      21,    1,   92,    2, 0x0a /* Public */,
      22,    0,   95,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 18,   17,   19,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void CounterUAS::ThreatAssessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ThreatAssessor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->threatLevelChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->newAlert((*reinterpret_cast< const ThreatAlert(*)>(_a[1]))); break;
        case 2: _t->alertAcknowledged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->highThreatDetected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->metricsUpdated((*reinterpret_cast< const ThreatMetrics(*)>(_a[1]))); break;
        case 5: _t->assessmentComplete(); break;
        case 6: _t->slewCameraRequest((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const GeoPosition(*)>(_a[2]))); break;
        case 7: _t->onTrackUpdated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onTrackCreated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->performAssessmentCycle(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ThreatAssessor::*)(const QString & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreatAssessor::threatLevelChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ThreatAssessor::*)(const ThreatAlert & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreatAssessor::newAlert)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ThreatAssessor::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreatAssessor::alertAcknowledged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ThreatAssessor::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreatAssessor::highThreatDetected)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ThreatAssessor::*)(const ThreatMetrics & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreatAssessor::metricsUpdated)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ThreatAssessor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreatAssessor::assessmentComplete)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ThreatAssessor::*)(const QString & , const GeoPosition & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreatAssessor::slewCameraRequest)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::ThreatAssessor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__ThreatAssessor.data,
    qt_meta_data_CounterUAS__ThreatAssessor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::ThreatAssessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::ThreatAssessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__ThreatAssessor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::ThreatAssessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CounterUAS::ThreatAssessor::threatLevelChanged(const QString & _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::ThreatAssessor::newAlert(const ThreatAlert & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::ThreatAssessor::alertAcknowledged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::ThreatAssessor::highThreatDetected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::ThreatAssessor::metricsUpdated(const ThreatMetrics & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CounterUAS::ThreatAssessor::assessmentComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CounterUAS::ThreatAssessor::slewCameraRequest(const QString & _t1, const GeoPosition & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
