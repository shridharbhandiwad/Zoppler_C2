/****************************************************************************
** Meta object code from reading C++ file 'VideoSource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/VideoSource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoSource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__VideoSource_t {
    QByteArrayData data[21];
    char stringdata0[242];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__VideoSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__VideoSource_t qt_meta_stringdata_CounterUAS__VideoSource = {
    {
QT_MOC_LITERAL(0, 0, 23), // "CounterUAS::VideoSource"
QT_MOC_LITERAL(1, 24, 10), // "frameReady"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 5), // "frame"
QT_MOC_LITERAL(4, 42, 9), // "timestamp"
QT_MOC_LITERAL(5, 52, 13), // "statusChanged"
QT_MOC_LITERAL(6, 66, 17), // "VideoSourceStatus"
QT_MOC_LITERAL(7, 84, 6), // "status"
QT_MOC_LITERAL(8, 91, 16), // "streamingChanged"
QT_MOC_LITERAL(9, 108, 9), // "streaming"
QT_MOC_LITERAL(10, 118, 5), // "error"
QT_MOC_LITERAL(11, 124, 7), // "message"
QT_MOC_LITERAL(12, 132, 12), // "statsUpdated"
QT_MOC_LITERAL(13, 145, 16), // "VideoSourceStats"
QT_MOC_LITERAL(14, 162, 5), // "stats"
QT_MOC_LITERAL(15, 168, 9), // "connected"
QT_MOC_LITERAL(16, 178, 12), // "disconnected"
QT_MOC_LITERAL(17, 191, 12), // "processFrame"
QT_MOC_LITERAL(18, 204, 11), // "updateStats"
QT_MOC_LITERAL(19, 216, 16), // "attemptReconnect"
QT_MOC_LITERAL(20, 233, 8) // "sourceId"

    },
    "CounterUAS::VideoSource\0frameReady\0\0"
    "frame\0timestamp\0statusChanged\0"
    "VideoSourceStatus\0status\0streamingChanged\0"
    "streaming\0error\0message\0statsUpdated\0"
    "VideoSourceStats\0stats\0connected\0"
    "disconnected\0processFrame\0updateStats\0"
    "attemptReconnect\0sourceId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__VideoSource[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       3,   86, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       5,    1,   69,    2, 0x06 /* Public */,
       8,    1,   72,    2, 0x06 /* Public */,
      10,    1,   75,    2, 0x06 /* Public */,
      12,    1,   78,    2, 0x06 /* Public */,
      15,    0,   81,    2, 0x06 /* Public */,
      16,    0,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    0,   83,    2, 0x09 /* Protected */,
      18,    0,   84,    2, 0x09 /* Protected */,
      19,    0,   85,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage, QMetaType::LongLong,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      20, QMetaType::QString, 0x00095401,
       7, 0x80000000 | 6, 0x00495009,
       9, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void CounterUAS::VideoSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoSource *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< VideoSourceStatus(*)>(_a[1]))); break;
        case 2: _t->streamingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->statsUpdated((*reinterpret_cast< const VideoSourceStats(*)>(_a[1]))); break;
        case 5: _t->connected(); break;
        case 6: _t->disconnected(); break;
        case 7: _t->processFrame(); break;
        case 8: _t->updateStats(); break;
        case 9: _t->attemptReconnect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoSource::*)(const QImage & , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoSource::frameReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)(VideoSourceStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoSource::statusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoSource::streamingChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoSource::error)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)(const VideoSourceStats & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoSource::statsUpdated)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoSource::connected)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoSource::disconnected)) {
                *result = 6;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VideoSource *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->sourceId(); break;
        case 1: *reinterpret_cast< VideoSourceStatus*>(_v) = _t->status(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isStreaming(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::VideoSource::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__VideoSource.data,
    qt_meta_data_CounterUAS__VideoSource,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::VideoSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::VideoSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__VideoSource.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CounterUAS::VideoSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CounterUAS::VideoSource::frameReady(const QImage & _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::VideoSource::statusChanged(VideoSourceStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CounterUAS::VideoSource::streamingChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CounterUAS::VideoSource::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CounterUAS::VideoSource::statsUpdated(const VideoSourceStats & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CounterUAS::VideoSource::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CounterUAS::VideoSource::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
