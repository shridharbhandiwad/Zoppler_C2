/****************************************************************************
** Meta object code from reading C++ file 'FileVideoSource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video/FileVideoSource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileVideoSource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__FileVideoSource_t {
    QByteArrayData data[13];
    char stringdata0[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__FileVideoSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__FileVideoSource_t qt_meta_stringdata_CounterUAS__FileVideoSource = {
    {
QT_MOC_LITERAL(0, 0, 27), // "CounterUAS::FileVideoSource"
QT_MOC_LITERAL(1, 28, 12), // "processFrame"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 16), // "onFrameAvailable"
QT_MOC_LITERAL(4, 59, 11), // "QVideoFrame"
QT_MOC_LITERAL(5, 71, 5), // "frame"
QT_MOC_LITERAL(6, 77, 20), // "onMediaStatusChanged"
QT_MOC_LITERAL(7, 98, 25), // "QMediaPlayer::MediaStatus"
QT_MOC_LITERAL(8, 124, 6), // "status"
QT_MOC_LITERAL(9, 131, 17), // "onPositionChanged"
QT_MOC_LITERAL(10, 149, 8), // "position"
QT_MOC_LITERAL(11, 158, 17), // "onDurationChanged"
QT_MOC_LITERAL(12, 176, 8) // "duration"

    },
    "CounterUAS::FileVideoSource\0processFrame\0"
    "\0onFrameAvailable\0QVideoFrame\0frame\0"
    "onMediaStatusChanged\0QMediaPlayer::MediaStatus\0"
    "status\0onPositionChanged\0position\0"
    "onDurationChanged\0duration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__FileVideoSource[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x09 /* Protected */,
       3,    1,   40,    2, 0x08 /* Private */,
       6,    1,   43,    2, 0x08 /* Private */,
       9,    1,   46,    2, 0x08 /* Private */,
      11,    1,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::LongLong,   10,
    QMetaType::Void, QMetaType::LongLong,   12,

       0        // eod
};

void CounterUAS::FileVideoSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FileVideoSource *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->processFrame(); break;
        case 1: _t->onFrameAvailable((*reinterpret_cast< const QVideoFrame(*)>(_a[1]))); break;
        case 2: _t->onMediaStatusChanged((*reinterpret_cast< QMediaPlayer::MediaStatus(*)>(_a[1]))); break;
        case 3: _t->onPositionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 4: _t->onDurationChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVideoFrame >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaPlayer::MediaStatus >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::FileVideoSource::staticMetaObject = { {
    QMetaObject::SuperData::link<VideoSource::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__FileVideoSource.data,
    qt_meta_data_CounterUAS__FileVideoSource,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
