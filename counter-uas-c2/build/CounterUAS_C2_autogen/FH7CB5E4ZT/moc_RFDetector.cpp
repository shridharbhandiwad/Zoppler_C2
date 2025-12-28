/****************************************************************************
** Meta object code from reading C++ file 'RFDetector.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/sensors/RFDetector.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RFDetector.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CounterUAS__RFDetector_t {
    uint offsetsAndSizes[28];
    char stringdata0[23];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[10];
    char stringdata5[19];
    char stringdata6[8];
    char stringdata7[9];
    char stringdata8[12];
    char stringdata9[15];
    char stringdata10[18];
    char stringdata11[14];
    char stringdata12[29];
    char stringdata13[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CounterUAS__RFDetector_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CounterUAS__RFDetector_t qt_meta_stringdata_CounterUAS__RFDetector = {
    {
        QT_MOC_LITERAL(0, 22),  // "CounterUAS::RFDetector"
        QT_MOC_LITERAL(23, 11),  // "rfDetection"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 11),  // "RFDetection"
        QT_MOC_LITERAL(48, 9),  // "detection"
        QT_MOC_LITERAL(58, 18),  // "protocolIdentified"
        QT_MOC_LITERAL(77, 7),  // "trackId"
        QT_MOC_LITERAL(85, 8),  // "protocol"
        QT_MOC_LITERAL(94, 11),  // "processData"
        QT_MOC_LITERAL(106, 14),  // "onUdpReadyRead"
        QT_MOC_LITERAL(121, 17),  // "onSerialReadyRead"
        QT_MOC_LITERAL(139, 13),  // "onSerialError"
        QT_MOC_LITERAL(153, 28),  // "QSerialPort::SerialPortError"
        QT_MOC_LITERAL(182, 5)   // "error"
    },
    "CounterUAS::RFDetector",
    "rfDetection",
    "",
    "RFDetection",
    "detection",
    "protocolIdentified",
    "trackId",
    "protocol",
    "processData",
    "onUdpReadyRead",
    "onSerialReadyRead",
    "onSerialError",
    "QSerialPort::SerialPortError",
    "error"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CounterUAS__RFDetector[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       5,    2,   53,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   58,    2, 0x09,    6 /* Protected */,
       9,    0,   59,    2, 0x08,    7 /* Private */,
      10,    0,   60,    2, 0x08,    8 /* Private */,
      11,    1,   61,    2, 0x08,    9 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

Q_CONSTINIT const QMetaObject CounterUAS::RFDetector::staticMetaObject = { {
    QMetaObject::SuperData::link<SensorInterface::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__RFDetector.offsetsAndSizes,
    qt_meta_data_CounterUAS__RFDetector,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CounterUAS__RFDetector_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RFDetector, std::true_type>,
        // method 'rfDetection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const RFDetection &, std::false_type>,
        // method 'protocolIdentified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'processData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUdpReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSerialReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSerialError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSerialPort::SerialPortError, std::false_type>
    >,
    nullptr
} };

void CounterUAS::RFDetector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RFDetector *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->rfDetection((*reinterpret_cast< std::add_pointer_t<RFDetection>>(_a[1]))); break;
        case 1: _t->protocolIdentified((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->processData(); break;
        case 3: _t->onUdpReadyRead(); break;
        case 4: _t->onSerialReadyRead(); break;
        case 5: _t->onSerialError((*reinterpret_cast< std::add_pointer_t<QSerialPort::SerialPortError>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RFDetector::*)(const RFDetection & );
            if (_t _q_method = &RFDetector::rfDetection; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RFDetector::*)(const QString & , const QString & );
            if (_t _q_method = &RFDetector::protocolIdentified; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *CounterUAS::RFDetector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::RFDetector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__RFDetector.stringdata0))
        return static_cast<void*>(this);
    return SensorInterface::qt_metacast(_clname);
}

int CounterUAS::RFDetector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SensorInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CounterUAS::RFDetector::rfDetection(const RFDetection & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CounterUAS::RFDetector::protocolIdentified(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
