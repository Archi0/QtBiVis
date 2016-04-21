/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtBiVis/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[21];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 4), // "draw"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 12), // "selectedList"
QT_MOC_LITERAL(4, 30, 5), // "index"
QT_MOC_LITERAL(5, 36, 13), // "drawBicluster"
QT_MOC_LITERAL(6, 50, 9), // "Bicluster"
QT_MOC_LITERAL(7, 60, 10), // "browseFile"
QT_MOC_LITERAL(8, 71, 9), // "showStats"
QT_MOC_LITERAL(9, 81, 13), // "quantityStats"
QT_MOC_LITERAL(10, 95, 9), // "percStats"
QT_MOC_LITERAL(11, 105, 13), // "calcSaveSigma"
QT_MOC_LITERAL(12, 119, 9), // "testStats"
QT_MOC_LITERAL(13, 129, 14), // "browseMainFile"
QT_MOC_LITERAL(14, 144, 16), // "openBiclusterWin"
QT_MOC_LITERAL(15, 161, 6), // "select"
QT_MOC_LITERAL(16, 168, 12), // "QMouseEvent*"
QT_MOC_LITERAL(17, 181, 5), // "event"
QT_MOC_LITERAL(18, 187, 8), // "setRange"
QT_MOC_LITERAL(19, 196, 8), // "QCPRange"
QT_MOC_LITERAL(20, 205, 8) // "newRange"

    },
    "MainWindow\0draw\0\0selectedList\0index\0"
    "drawBicluster\0Bicluster\0browseFile\0"
    "showStats\0quantityStats\0percStats\0"
    "calcSaveSigma\0testStats\0browseMainFile\0"
    "openBiclusterWin\0select\0QMouseEvent*\0"
    "event\0setRange\0QCPRange\0newRange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a /* Public */,
       3,    1,   80,    2, 0x0a /* Public */,
       5,    1,   83,    2, 0x0a /* Public */,
       7,    0,   86,    2, 0x0a /* Public */,
       8,    0,   87,    2, 0x0a /* Public */,
       9,    0,   88,    2, 0x0a /* Public */,
      10,    0,   89,    2, 0x0a /* Public */,
      11,    0,   90,    2, 0x0a /* Public */,
      12,    0,   91,    2, 0x0a /* Public */,
      13,    0,   92,    2, 0x0a /* Public */,
      14,    0,   93,    2, 0x0a /* Public */,
      15,    1,   94,    2, 0x0a /* Public */,
      18,    1,   97,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19,   20,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->draw(); break;
        case 1: _t->selectedList((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->drawBicluster((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->browseFile(); break;
        case 4: _t->showStats(); break;
        case 5: _t->quantityStats(); break;
        case 6: _t->percStats(); break;
        case 7: _t->calcSaveSigma(); break;
        case 8: _t->testStats(); break;
        case 9: _t->browseMainFile(); break;
        case 10: _t->openBiclusterWin(); break;
        case 11: _t->select((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 12: _t->setRange((*reinterpret_cast< QCPRange(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
