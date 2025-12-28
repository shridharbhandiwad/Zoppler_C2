/****************************************************************************
** Meta object code from reading C++ file 'FileVideoSource.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/FileVideoSource.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileVideoSource.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__FileVideoSource_t {
    uint offsetsAndSizes[26];
    char stringdata0[28];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[12];
    char stringdata5[6];
    char stringdata6[21];
    char stringdata7[26];
    char stringdata8[7];
    char stringdata9[18];
    char stringdata10[9];
    char stringdata11[18];
    char stringdata12[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__FileVideoSource_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__FileVideoSource_t qt_meta_stringdata_CounterUAS__FileVideoSource = {
    {
        QT_MOC_LITERAL(0, 27),  // "CounterUAS::FileVideoSource"
        QT_MOC_LITERAL(28, 12),  // "processFrame"
        QT_MOC_LITERAL(41, 0),  // ""
        QT_MOC_LITERAL(42, 19),  // "onVideoFrameChanged"
        QT_MOC_LITERAL(62, 11),  // "QVideoFrame"
        QT_MOC_LITERAL(74, 5),  // "frame"
        QT_MOC_LITERAL(80, 20),  // "onMediaStatusChanged"
        QT_MOC_LITERAL(101, 25),  // "QMediaPlayer::MediaStatus"
        QT_MOC_LITERAL(127, 6),  // "status"
        QT_MOC_LITERAL(134, 17),  // "onPositionChanged"
        QT_MOC_LITERAL(152, 8),  // "position"
        QT_MOC_LITERAL(161, 17),  // "onDurationChanged"
        QT_MOC_LITERAL(179, 8)   // "duration"
    },
    "CounterUAS::FileVideoSource",
    "processFrame",
    "",
    "onVideoFrameChanged",
    "QVideoFrame",
    "frame",
    "onMediaStatusChanged",
    "QMediaPlayer::MediaStatus",
    "status",
    "onPositionChanged",
    "position",
    "onDurationChanged",
    "duration"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__FileVideoSource[] = {

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
       9,    1,   51,    2, 0x08,    6 /* Private */,
      11,    1,   54,    2, 0x08,    8 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::LongLong,   10,
    QMetaType::Void, QMetaType::LongLong,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::FileVideoSource::staticMetaObject = { {
    QMetaObject::SuperData::link<VideoSource::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__FileVideoSource.offsetsAndSizes,
    qt_meta_data_CounterUAS__FileVideoSource,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__FileVideoSource_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<FileVideoSource, std::true_type>,
        // method 'processFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onVideoFrameChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVideoFrame &, std::false_type>,
        // method 'onMediaStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMediaPlayer::MediaStatus, std::false_type>,
        // method 'onPositionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'onDurationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>
    >,
    nullptr
} };

void CounterUAS::FileVideoSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FileVideoSource *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->processFrame(); break;
        case 1: _t->onVideoFrameChanged((*reinterpret_cast< std::add_pointer_t<QVideoFrame>>(_a[1]))); break;
        case 2: _t->onMediaStatusChanged((*reinterpret_cast< std::add_pointer_t<QMediaPlayer::MediaStatus>>(_a[1]))); break;
        case 3: _t->onPositionChanged((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 4: _t->onDurationChanged((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QVideoFrame >(); break;
            }
            break;
        }
    }
}

const QMetaObject *CounterUAS::FileVideoSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::FileVideoSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__FileVideoSource.stringdata0))
        return static_cast<void*>(this);
    return VideoSource::qt_metacast(_clname);
}

int CounterUAS::FileVideoSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
