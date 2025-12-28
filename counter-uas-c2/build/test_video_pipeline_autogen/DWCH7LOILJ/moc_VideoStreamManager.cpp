/****************************************************************************
** Meta object code from reading C++ file 'VideoStreamManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/VideoStreamManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoStreamManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__VideoStreamManager_t {
    QByteArrayData data[27];
    char stringdata0[378];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__VideoStreamManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__VideoStreamManager_t qt_meta_stringdata_CounterUAS__VideoStreamManager = {
    {
QT_MOC_LITERAL(0, 0, 30), // "CounterUAS::VideoStreamManager"
QT_MOC_LITERAL(1, 31, 11), // "streamAdded"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 8), // "cameraId"
QT_MOC_LITERAL(4, 53, 13), // "streamRemoved"
QT_MOC_LITERAL(5, 67, 19), // "streamStatusChanged"
QT_MOC_LITERAL(6, 87, 17), // "VideoSourceStatus"
QT_MOC_LITERAL(7, 105, 6), // "status"
QT_MOC_LITERAL(8, 112, 10), // "frameReady"
QT_MOC_LITERAL(9, 123, 5), // "frame"
QT_MOC_LITERAL(10, 129, 20), // "primaryStreamChanged"
QT_MOC_LITERAL(11, 150, 16), // "recordingStarted"
QT_MOC_LITERAL(12, 167, 16), // "recordingStopped"
QT_MOC_LITERAL(13, 184, 24), // "activeStreamCountChanged"
QT_MOC_LITERAL(14, 209, 5), // "count"
QT_MOC_LITERAL(15, 215, 13), // "cameraSlewing"
QT_MOC_LITERAL(16, 229, 11), // "GeoPosition"
QT_MOC_LITERAL(17, 241, 6), // "target"
QT_MOC_LITERAL(18, 248, 18), // "cameraSlewComplete"
QT_MOC_LITERAL(19, 267, 14), // "onTrackUpdated"
QT_MOC_LITERAL(20, 282, 7), // "trackId"
QT_MOC_LITERAL(21, 290, 3), // "pos"
QT_MOC_LITERAL(22, 294, 14), // "onTrackDropped"
QT_MOC_LITERAL(23, 309, 18), // "onStreamFrameReady"
QT_MOC_LITERAL(24, 328, 9), // "timestamp"
QT_MOC_LITERAL(25, 338, 21), // "onStreamStatusChanged"
QT_MOC_LITERAL(26, 360, 17) // "activeStreamCount"

    },
    "CounterUAS::VideoStreamManager\0"
    "streamAdded\0\0cameraId\0streamRemoved\0"
    "streamStatusChanged\0VideoSourceStatus\0"
    "status\0frameReady\0frame\0primaryStreamChanged\0"
    "recordingStarted\0recordingStopped\0"
    "activeStreamCountChanged\0count\0"
    "cameraSlewing\0GeoPosition\0target\0"
    "cameraSlewComplete\0onTrackUpdated\0"
    "trackId\0pos\0onTrackDropped\0"
    "onStreamFrameReady\0timestamp\0"
    "onStreamStatusChanged\0activeStreamCount"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__VideoStreamManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       1,  136, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       5,    2,   90,    2, 0x06 /* Public */,
       8,    2,   95,    2, 0x06 /* Public */,
      10,    1,  100,    2, 0x06 /* Public */,
      11,    1,  103,    2, 0x06 /* Public */,
      12,    1,  106,    2, 0x06 /* Public */,
      13,    1,  109,    2, 0x06 /* Public */,
      15,    2,  112,    2, 0x06 /* Public */,
      18,    1,  117,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    2,  120,    2, 0x0a /* Public */,
      22,    1,  125,    2, 0x0a /* Public */,
      23,    2,  128,    2, 0x08 /* Private */,
      25,    1,  133,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6,    3,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QImage,    3,    9,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 16,    3,   17,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 16,   20,   21,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QImage, QMetaType::LongLong,    9,   24,
    QMetaType::Void, 0x80000000 | 6,    7,

 // properties: name, type, flags
      26, QMetaType::Int, 0x00495001,

 // properties: notify_signal_id
       7,

       0        // eod
};

void CounterUAS::VideoStreamManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoStreamManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->streamAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->streamRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->streamStatusChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< VideoSourceStatus(*)>(_a[2]))); break;
        case 3: _t->frameReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QImage(*)>(_a[2]))); break;
        case 4: _t->primaryStreamChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->recordingStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->recordingStopped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->activeStreamCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->cameraSlewing((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const GeoPosition(*)>(_a[2]))); break;
        case 9: _t->cameraSlewComplete((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->onTrackUpdated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const GeoPosition(*)>(_a[2]))); break;
        case 11: _t->onTrackDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->onStreamFrameReady((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 13: _t->onStreamStatusChanged((*reinterpret_cast< VideoSourceStatus(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::streamAdded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::streamRemoved)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & , VideoSourceStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::streamStatusChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & , const QImage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::frameReady)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::primaryStreamChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::recordingStarted)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::recordingStopped)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::activeStreamCountChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & , const GeoPosition & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::cameraSlewing)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoStreamManager::cameraSlewComplete)) {
                *result = 9;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VideoStreamManager *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->activeStreamCount(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::VideoStreamManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__VideoStreamManager.data,
    qt_meta_data_CounterUAS__VideoStreamManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::VideoStreamManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::VideoStreamManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__VideoStreamManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::VideoStreamManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CounterUAS::VideoStreamManager::streamAdded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::VideoStreamManager::streamRemoved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::VideoStreamManager::streamStatusChanged(const QString & _t1, VideoSourceStatus _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::VideoStreamManager::frameReady(const QString & _t1, const QImage & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::VideoStreamManager::primaryStreamChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CounterUAS::VideoStreamManager::recordingStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CounterUAS::VideoStreamManager::recordingStopped(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CounterUAS::VideoStreamManager::activeStreamCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CounterUAS::VideoStreamManager::cameraSlewing(const QString & _t1, const GeoPosition & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CounterUAS::VideoStreamManager::cameraSlewComplete(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
