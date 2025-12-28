/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ui/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CounterUAS__MainWindow_t {
    QByteArrayData data[15];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CounterUAS__MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CounterUAS__MainWindow_t qt_meta_stringdata_CounterUAS__MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 22), // "CounterUAS::MainWindow"
QT_MOC_LITERAL(1, 23, 15), // "startSimulation"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 14), // "stopSimulation"
QT_MOC_LITERAL(4, 55, 17), // "loadConfiguration"
QT_MOC_LITERAL(5, 73, 17), // "saveConfiguration"
QT_MOC_LITERAL(6, 91, 9), // "showAbout"
QT_MOC_LITERAL(7, 101, 15), // "updateStatusBar"
QT_MOC_LITERAL(8, 117, 15), // "onTrackSelected"
QT_MOC_LITERAL(9, 133, 7), // "trackId"
QT_MOC_LITERAL(10, 141, 17), // "onEngageRequested"
QT_MOC_LITERAL(11, 159, 21), // "onCameraSlewRequested"
QT_MOC_LITERAL(12, 181, 14), // "onAlertClicked"
QT_MOC_LITERAL(13, 196, 7), // "alertId"
QT_MOC_LITERAL(14, 204, 21) // "toggleFullScreenVideo"

    },
    "CounterUAS::MainWindow\0startSimulation\0"
    "\0stopSimulation\0loadConfiguration\0"
    "saveConfiguration\0showAbout\0updateStatusBar\0"
    "onTrackSelected\0trackId\0onEngageRequested\0"
    "onCameraSlewRequested\0onAlertClicked\0"
    "alertId\0toggleFullScreenVideo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CounterUAS__MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    0,   70,    2, 0x0a /* Public */,
       4,    0,   71,    2, 0x0a /* Public */,
       5,    0,   72,    2, 0x0a /* Public */,
       6,    0,   73,    2, 0x0a /* Public */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    1,   75,    2, 0x08 /* Private */,
      10,    1,   78,    2, 0x08 /* Private */,
      11,    1,   81,    2, 0x08 /* Private */,
      12,    1,   84,    2, 0x08 /* Private */,
      14,    0,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,

       0        // eod
};

void CounterUAS::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->startSimulation(); break;
        case 1: _t->stopSimulation(); break;
        case 2: _t->loadConfiguration(); break;
        case 3: _t->saveConfiguration(); break;
        case 4: _t->showAbout(); break;
        case 5: _t->updateStatusBar(); break;
        case 6: _t->onTrackSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->onEngageRequested((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onCameraSlewRequested((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->onAlertClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->toggleFullScreenVideo(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CounterUAS::MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CounterUAS__MainWindow.data,
    qt_meta_data_CounterUAS__MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CounterUAS::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CounterUAS::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CounterUAS__MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CounterUAS::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
