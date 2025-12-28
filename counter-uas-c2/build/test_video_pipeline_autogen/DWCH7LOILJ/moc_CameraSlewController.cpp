/****************************************************************************
** Meta object code from reading C++ file 'CameraSlewController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/CameraSlewController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CameraSlewController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__CameraSlewController_t {
    QByteArrayData data[14];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__CameraSlewController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__CameraSlewController_t qt_meta_stringdata_CounterUAS__CameraSlewController = {
    {
QT_MOC_LITERAL(0, 0, 32), // "CounterUAS::CameraSlewController"
QT_MOC_LITERAL(1, 33, 11), // "slewStarted"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 8), // "cameraId"
QT_MOC_LITERAL(4, 55, 11), // "GeoPosition"
QT_MOC_LITERAL(5, 67, 6), // "target"
QT_MOC_LITERAL(6, 74, 12), // "slewComplete"
QT_MOC_LITERAL(7, 87, 15), // "trackingStarted"
QT_MOC_LITERAL(8, 103, 7), // "trackId"
QT_MOC_LITERAL(9, 111, 15), // "trackingStopped"
QT_MOC_LITERAL(10, 127, 9), // "trackLost"
QT_MOC_LITERAL(11, 137, 14), // "onTrackUpdated"
QT_MOC_LITERAL(12, 152, 14), // "onTrackDropped"
QT_MOC_LITERAL(13, 167, 14) // "updateTracking"

    },
    "CounterUAS::CameraSlewController\0"
    "slewStarted\0\0cameraId\0GeoPosition\0"
    "target\0slewComplete\0trackingStarted\0"
    "trackId\0trackingStopped\0trackLost\0"
    "onTrackUpdated\0onTrackDropped\0"
    "updateTracking"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__CameraSlewController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       6,    1,   59,    2, 0x06 /* Public */,
       7,    2,   62,    2, 0x06 /* Public */,
       9,    1,   67,    2, 0x06 /* Public */,
      10,    2,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   75,    2, 0x08 /* Private */,
      12,    1,   78,    2, 0x08 /* Private */,
      13,    0,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    8,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

void CounterUAS::CameraSlewController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CameraSlewController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slewStarted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const GeoPosition(*)>(_a[2]))); break;
        case 1: _t->slewComplete((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->trackingStarted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->trackingStopped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->trackLost((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->onTrackUpdated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onTrackDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->updateTracking(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CameraSlewController::*)(const QString & , const GeoPosition & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSlewController::slewStarted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CameraSlewController::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSlewController::slewComplete)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CameraSlewController::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSlewController::trackingStarted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CameraSlewController::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSlewController::trackingStopped)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CameraSlewController::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraSlewController::trackLost)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::CameraSlewController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__CameraSlewController.data,
    qt_meta_data_CounterUAS__CameraSlewController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::CameraSlewController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::CameraSlewController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__CameraSlewController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::CameraSlewController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::CameraSlewController::slewStarted(const QString & _t1, const GeoPosition & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::CameraSlewController::slewComplete(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::CameraSlewController::trackingStarted(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::CameraSlewController::trackingStopped(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::CameraSlewController::trackLost(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
