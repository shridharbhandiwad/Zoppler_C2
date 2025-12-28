/****************************************************************************
** Meta object code from reading C++ file 'VideoStreamManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/VideoStreamManager.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoStreamManager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__VideoStreamManager_t {
    uint offsetsAndSizes[54];
    char stringdata0[31];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[14];
    char stringdata5[20];
    char stringdata6[18];
    char stringdata7[7];
    char stringdata8[11];
    char stringdata9[6];
    char stringdata10[21];
    char stringdata11[17];
    char stringdata12[17];
    char stringdata13[25];
    char stringdata14[6];
    char stringdata15[14];
    char stringdata16[12];
    char stringdata17[7];
    char stringdata18[19];
    char stringdata19[15];
    char stringdata20[8];
    char stringdata21[4];
    char stringdata22[15];
    char stringdata23[19];
    char stringdata24[10];
    char stringdata25[22];
    char stringdata26[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__VideoStreamManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__VideoStreamManager_t qt_meta_stringdata_CounterUAS__VideoStreamManager = {
    {
        QT_MOC_LITERAL(0, 30),  // "CounterUAS::VideoStreamManager"
        QT_MOC_LITERAL(31, 11),  // "streamAdded"
        QT_MOC_LITERAL(43, 0),  // ""
        QT_MOC_LITERAL(44, 8),  // "cameraId"
        QT_MOC_LITERAL(53, 13),  // "streamRemoved"
        QT_MOC_LITERAL(67, 19),  // "streamStatusChanged"
        QT_MOC_LITERAL(87, 17),  // "VideoSourceStatus"
        QT_MOC_LITERAL(105, 6),  // "status"
        QT_MOC_LITERAL(112, 10),  // "frameReady"
        QT_MOC_LITERAL(123, 5),  // "frame"
        QT_MOC_LITERAL(129, 20),  // "primaryStreamChanged"
        QT_MOC_LITERAL(150, 16),  // "recordingStarted"
        QT_MOC_LITERAL(167, 16),  // "recordingStopped"
        QT_MOC_LITERAL(184, 24),  // "activeStreamCountChanged"
        QT_MOC_LITERAL(209, 5),  // "count"
        QT_MOC_LITERAL(215, 13),  // "cameraSlewing"
        QT_MOC_LITERAL(229, 11),  // "GeoPosition"
        QT_MOC_LITERAL(241, 6),  // "target"
        QT_MOC_LITERAL(248, 18),  // "cameraSlewComplete"
        QT_MOC_LITERAL(267, 14),  // "onTrackUpdated"
        QT_MOC_LITERAL(282, 7),  // "trackId"
        QT_MOC_LITERAL(290, 3),  // "pos"
        QT_MOC_LITERAL(294, 14),  // "onTrackDropped"
        QT_MOC_LITERAL(309, 18),  // "onStreamFrameReady"
        QT_MOC_LITERAL(328, 9),  // "timestamp"
        QT_MOC_LITERAL(338, 21),  // "onStreamStatusChanged"
        QT_MOC_LITERAL(360, 17)   // "activeStreamCount"
    },
    "CounterUAS::VideoStreamManager",
    "streamAdded",
    "",
    "cameraId",
    "streamRemoved",
    "streamStatusChanged",
    "VideoSourceStatus",
    "status",
    "frameReady",
    "frame",
    "primaryStreamChanged",
    "recordingStarted",
    "recordingStopped",
    "activeStreamCountChanged",
    "count",
    "cameraSlewing",
    "GeoPosition",
    "target",
    "cameraSlewComplete",
    "onTrackUpdated",
    "trackId",
    "pos",
    "onTrackDropped",
    "onStreamFrameReady",
    "timestamp",
    "onStreamStatusChanged",
    "activeStreamCount"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__VideoStreamManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       1,  150, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   98,    2, 0x06,    2 /* Public */,
       4,    1,  101,    2, 0x06,    4 /* Public */,
       5,    2,  104,    2, 0x06,    6 /* Public */,
       8,    2,  109,    2, 0x06,    9 /* Public */,
      10,    1,  114,    2, 0x06,   12 /* Public */,
      11,    1,  117,    2, 0x06,   14 /* Public */,
      12,    1,  120,    2, 0x06,   16 /* Public */,
      13,    1,  123,    2, 0x06,   18 /* Public */,
      15,    2,  126,    2, 0x06,   20 /* Public */,
      18,    1,  131,    2, 0x06,   23 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      19,    2,  134,    2, 0x0a,   25 /* Public */,
      22,    1,  139,    2, 0x0a,   28 /* Public */,
      23,    2,  142,    2, 0x08,   30 /* Private */,
      25,    1,  147,    2, 0x08,   33 /* Private */,

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
      26, QMetaType::Int, 0x00015001, uint(7), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::VideoStreamManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__VideoStreamManager.offsetsAndSizes,
    qt_meta_data_CounterUAS__VideoStreamManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__VideoStreamManager_t,
        // property 'activeStreamCount'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VideoStreamManager, std::true_type>,
        // method 'streamAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'streamRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'streamStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<VideoSourceStatus, std::false_type>,
        // method 'frameReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        // method 'primaryStreamChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'recordingStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'recordingStopped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'activeStreamCountChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cameraSlewing'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const GeoPosition &, std::false_type>,
        // method 'cameraSlewComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTrackUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const GeoPosition &, std::false_type>,
        // method 'onTrackDropped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onStreamFrameReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'onStreamStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<VideoSourceStatus, std::false_type>
    >,
    nullptr
} };

void CounterUAS::VideoStreamManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoStreamManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->streamAdded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->streamRemoved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->streamStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<VideoSourceStatus>>(_a[2]))); break;
        case 3: _t->frameReady((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QImage>>(_a[2]))); break;
        case 4: _t->primaryStreamChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->recordingStarted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->recordingStopped((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->activeStreamCountChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->cameraSlewing((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GeoPosition>>(_a[2]))); break;
        case 9: _t->cameraSlewComplete((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->onTrackUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GeoPosition>>(_a[2]))); break;
        case 11: _t->onTrackDropped((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->onStreamFrameReady((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        case 13: _t->onStreamStatusChanged((*reinterpret_cast< std::add_pointer_t<VideoSourceStatus>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (_t _q_method = &VideoStreamManager::streamAdded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (_t _q_method = &VideoStreamManager::streamRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & , VideoSourceStatus );
            if (_t _q_method = &VideoStreamManager::streamStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & , const QImage & );
            if (_t _q_method = &VideoStreamManager::frameReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (_t _q_method = &VideoStreamManager::primaryStreamChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (_t _q_method = &VideoStreamManager::recordingStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (_t _q_method = &VideoStreamManager::recordingStopped; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(int );
            if (_t _q_method = &VideoStreamManager::activeStreamCountChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & , const GeoPosition & );
            if (_t _q_method = &VideoStreamManager::cameraSlewing; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (VideoStreamManager::*)(const QString & );
            if (_t _q_method = &VideoStreamManager::cameraSlewComplete; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VideoStreamManager *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->activeStreamCount(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
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
