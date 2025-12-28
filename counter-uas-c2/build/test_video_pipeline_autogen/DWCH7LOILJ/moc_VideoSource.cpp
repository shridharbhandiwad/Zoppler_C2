/****************************************************************************
** Meta object code from reading C++ file 'VideoSource.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/VideoSource.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoSource.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__VideoSource_t {
    uint offsetsAndSizes[42];
    char stringdata0[24];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[10];
    char stringdata5[14];
    char stringdata6[18];
    char stringdata7[7];
    char stringdata8[17];
    char stringdata9[10];
    char stringdata10[6];
    char stringdata11[8];
    char stringdata12[13];
    char stringdata13[17];
    char stringdata14[6];
    char stringdata15[10];
    char stringdata16[13];
    char stringdata17[13];
    char stringdata18[12];
    char stringdata19[17];
    char stringdata20[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__VideoSource_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__VideoSource_t qt_meta_stringdata_CounterUAS__VideoSource = {
    {
        QT_MOC_LITERAL(0, 23),  // "CounterUAS::VideoSource"
        QT_MOC_LITERAL(24, 10),  // "frameReady"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 5),  // "frame"
        QT_MOC_LITERAL(42, 9),  // "timestamp"
        QT_MOC_LITERAL(52, 13),  // "statusChanged"
        QT_MOC_LITERAL(66, 17),  // "VideoSourceStatus"
        QT_MOC_LITERAL(84, 6),  // "status"
        QT_MOC_LITERAL(91, 16),  // "streamingChanged"
        QT_MOC_LITERAL(108, 9),  // "streaming"
        QT_MOC_LITERAL(118, 5),  // "error"
        QT_MOC_LITERAL(124, 7),  // "message"
        QT_MOC_LITERAL(132, 12),  // "statsUpdated"
        QT_MOC_LITERAL(145, 16),  // "VideoSourceStats"
        QT_MOC_LITERAL(162, 5),  // "stats"
        QT_MOC_LITERAL(168, 9),  // "connected"
        QT_MOC_LITERAL(178, 12),  // "disconnected"
        QT_MOC_LITERAL(191, 12),  // "processFrame"
        QT_MOC_LITERAL(204, 11),  // "updateStats"
        QT_MOC_LITERAL(216, 16),  // "attemptReconnect"
        QT_MOC_LITERAL(233, 8)   // "sourceId"
    },
    "CounterUAS::VideoSource",
    "frameReady",
    "",
    "frame",
    "timestamp",
    "statusChanged",
    "VideoSourceStatus",
    "status",
    "streamingChanged",
    "streaming",
    "error",
    "message",
    "statsUpdated",
    "VideoSourceStats",
    "stats",
    "connected",
    "disconnected",
    "processFrame",
    "updateStats",
    "attemptReconnect",
    "sourceId"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__VideoSource[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       3,   96, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   74,    2, 0x06,    4 /* Public */,
       5,    1,   79,    2, 0x06,    7 /* Public */,
       8,    1,   82,    2, 0x06,    9 /* Public */,
      10,    1,   85,    2, 0x06,   11 /* Public */,
      12,    1,   88,    2, 0x06,   13 /* Public */,
      15,    0,   91,    2, 0x06,   15 /* Public */,
      16,    0,   92,    2, 0x06,   16 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      17,    0,   93,    2, 0x09,   17 /* Protected */,
      18,    0,   94,    2, 0x09,   18 /* Protected */,
      19,    0,   95,    2, 0x09,   19 /* Protected */,

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
      20, QMetaType::QString, 0x00015401, uint(-1), 0,
       7, 0x80000000 | 6, 0x00015009, uint(1), 0,
       9, QMetaType::Bool, 0x00015001, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::VideoSource::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__VideoSource.offsetsAndSizes,
    qt_meta_data_CounterUAS__VideoSource,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__VideoSource_t,
        // property 'sourceId'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'status'
        QtPrivate::TypeAndForceComplete<VideoSourceStatus, std::true_type>,
        // property 'streaming'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VideoSource, std::true_type>,
        // method 'frameReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'statusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<VideoSourceStatus, std::false_type>,
        // method 'streamingChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'error'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'statsUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const VideoSourceStats &, std::false_type>,
        // method 'connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'attemptReconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CounterUAS::VideoSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoSource *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< std::add_pointer_t<VideoSourceStatus>>(_a[1]))); break;
        case 2: _t->streamingChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->statsUpdated((*reinterpret_cast< std::add_pointer_t<VideoSourceStats>>(_a[1]))); break;
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
            if (_t _q_method = &VideoSource::frameReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)(VideoSourceStatus );
            if (_t _q_method = &VideoSource::statusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)(bool );
            if (_t _q_method = &VideoSource::streamingChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)(const QString & );
            if (_t _q_method = &VideoSource::error; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)(const VideoSourceStats & );
            if (_t _q_method = &VideoSource::statsUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)();
            if (_t _q_method = &VideoSource::connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VideoSource::*)();
            if (_t _q_method = &VideoSource::disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
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
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
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
