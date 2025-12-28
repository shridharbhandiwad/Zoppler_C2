/****************************************************************************
** Meta object code from reading C++ file 'RTSPVideoSource.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/RTSPVideoSource.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RTSPVideoSource.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__RTSPVideoSource_t {
    uint offsetsAndSizes[32];
    char stringdata0[28];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[21];
    char stringdata4[26];
    char stringdata5[7];
    char stringdata6[23];
    char stringdata7[28];
    char stringdata8[6];
    char stringdata9[16];
    char stringdata10[20];
    char stringdata11[6];
    char stringdata12[12];
    char stringdata13[20];
    char stringdata14[12];
    char stringdata15[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__RTSPVideoSource_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__RTSPVideoSource_t qt_meta_stringdata_CounterUAS__RTSPVideoSource = {
    {
        QT_MOC_LITERAL(0, 27),  // "CounterUAS::RTSPVideoSource"
        QT_MOC_LITERAL(28, 12),  // "processFrame"
        QT_MOC_LITERAL(41, 0),  // ""
        QT_MOC_LITERAL(42, 20),  // "onMediaStatusChanged"
        QT_MOC_LITERAL(63, 25),  // "QMediaPlayer::MediaStatus"
        QT_MOC_LITERAL(89, 6),  // "status"
        QT_MOC_LITERAL(96, 22),  // "onPlaybackStateChanged"
        QT_MOC_LITERAL(119, 27),  // "QMediaPlayer::PlaybackState"
        QT_MOC_LITERAL(147, 5),  // "state"
        QT_MOC_LITERAL(153, 15),  // "onErrorOccurred"
        QT_MOC_LITERAL(169, 19),  // "QMediaPlayer::Error"
        QT_MOC_LITERAL(189, 5),  // "error"
        QT_MOC_LITERAL(195, 11),  // "errorString"
        QT_MOC_LITERAL(207, 19),  // "onVideoFrameChanged"
        QT_MOC_LITERAL(227, 11),  // "QVideoFrame"
        QT_MOC_LITERAL(239, 5)   // "frame"
    },
    "CounterUAS::RTSPVideoSource",
    "processFrame",
    "",
    "onMediaStatusChanged",
    "QMediaPlayer::MediaStatus",
    "status",
    "onPlaybackStateChanged",
    "QMediaPlayer::PlaybackState",
    "state",
    "onErrorOccurred",
    "QMediaPlayer::Error",
    "error",
    "errorString",
    "onVideoFrameChanged",
    "QVideoFrame",
    "frame"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__RTSPVideoSource[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x09,    1 /* Protected */,
       3,    1,   45,    2, 0x08,    2 /* Private */,
       6,    1,   48,    2, 0x08,    4 /* Private */,
       9,    2,   51,    2, 0x08,    6 /* Private */,
      13,    1,   56,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::QString,   11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::RTSPVideoSource::staticMetaObject = { {
    QMetaObject::SuperData::link<VideoSource::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__RTSPVideoSource.offsetsAndSizes,
    qt_meta_data_CounterUAS__RTSPVideoSource,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__RTSPVideoSource_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RTSPVideoSource, std::true_type>,
        // method 'processFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onMediaStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMediaPlayer::MediaStatus, std::false_type>,
        // method 'onPlaybackStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMediaPlayer::PlaybackState, std::false_type>,
        // method 'onErrorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMediaPlayer::Error, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onVideoFrameChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVideoFrame &, std::false_type>
    >,
    nullptr
} };

void CounterUAS::RTSPVideoSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RTSPVideoSource *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->processFrame(); break;
        case 1: _t->onMediaStatusChanged((*reinterpret_cast< std::add_pointer_t<QMediaPlayer::MediaStatus>>(_a[1]))); break;
        case 2: _t->onPlaybackStateChanged((*reinterpret_cast< std::add_pointer_t<QMediaPlayer::PlaybackState>>(_a[1]))); break;
        case 3: _t->onErrorOccurred((*reinterpret_cast< std::add_pointer_t<QMediaPlayer::Error>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->onVideoFrameChanged((*reinterpret_cast< std::add_pointer_t<QVideoFrame>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QVideoFrame >(); break;
            }
            break;
        }
    }
}

const QMetaObject *CounterUAS::RTSPVideoSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::RTSPVideoSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__RTSPVideoSource.stringdata0))
        return static_cast<void*>(this);
    return VideoSource::qt_metacast(_clname);
}

int CounterUAS::RTSPVideoSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VideoSource::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
