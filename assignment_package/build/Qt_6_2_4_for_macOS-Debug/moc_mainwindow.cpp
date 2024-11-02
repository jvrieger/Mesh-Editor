/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    const uint offsetsAndSize[46];
    char stringdata0[410];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 10), // "MainWindow"
QT_MOC_LITERAL(11, 21), // "updateHalfEdgeDisplay"
QT_MOC_LITERAL(33, 0), // ""
QT_MOC_LITERAL(34, 9), // "HalfEdge*"
QT_MOC_LITERAL(44, 16), // "selectedHalfEdge"
QT_MOC_LITERAL(61, 17), // "updateFaceDisplay"
QT_MOC_LITERAL(79, 5), // "Face*"
QT_MOC_LITERAL(85, 12), // "selectedFace"
QT_MOC_LITERAL(98, 19), // "updateVertexDisplay"
QT_MOC_LITERAL(118, 7), // "Vertex*"
QT_MOC_LITERAL(126, 14), // "selectedVertex"
QT_MOC_LITERAL(141, 23), // "on_actionQuit_triggered"
QT_MOC_LITERAL(165, 18), // "on_openOBJ_clicked"
QT_MOC_LITERAL(184, 30), // "on_vertsListWidget_itemClicked"
QT_MOC_LITERAL(215, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(232, 4), // "item"
QT_MOC_LITERAL(237, 34), // "on_halfEdgesListWidget_itemCl..."
QT_MOC_LITERAL(272, 30), // "on_facesListWidget_itemClicked"
QT_MOC_LITERAL(303, 20), // "on_splitEdge_clicked"
QT_MOC_LITERAL(324, 20), // "on_subdivide_clicked"
QT_MOC_LITERAL(345, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(367, 23), // "onVertexPositionChanged"
QT_MOC_LITERAL(391, 18) // "onFaceColorChanged"

    },
    "MainWindow\0updateHalfEdgeDisplay\0\0"
    "HalfEdge*\0selectedHalfEdge\0updateFaceDisplay\0"
    "Face*\0selectedFace\0updateVertexDisplay\0"
    "Vertex*\0selectedVertex\0on_actionQuit_triggered\0"
    "on_openOBJ_clicked\0on_vertsListWidget_itemClicked\0"
    "QListWidgetItem*\0item\0"
    "on_halfEdgesListWidget_itemClicked\0"
    "on_facesListWidget_itemClicked\0"
    "on_splitEdge_clicked\0on_subdivide_clicked\0"
    "on_pushButton_clicked\0onVertexPositionChanged\0"
    "onFaceColorChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   92,    2, 0x0a,    1 /* Public */,
       5,    1,   95,    2, 0x0a,    3 /* Public */,
       8,    1,   98,    2, 0x0a,    5 /* Public */,
      11,    0,  101,    2, 0x08,    7 /* Private */,
      12,    0,  102,    2, 0x08,    8 /* Private */,
      13,    1,  103,    2, 0x08,    9 /* Private */,
      16,    1,  106,    2, 0x08,   11 /* Private */,
      17,    1,  109,    2, 0x08,   13 /* Private */,
      18,    0,  112,    2, 0x08,   15 /* Private */,
      19,    0,  113,    2, 0x08,   16 /* Private */,
      20,    0,  114,    2, 0x08,   17 /* Private */,
      21,    0,  115,    2, 0x08,   18 /* Private */,
      22,    0,  116,    2, 0x08,   19 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateHalfEdgeDisplay((*reinterpret_cast< std::add_pointer_t<HalfEdge*>>(_a[1]))); break;
        case 1: _t->updateFaceDisplay((*reinterpret_cast< std::add_pointer_t<Face*>>(_a[1]))); break;
        case 2: _t->updateVertexDisplay((*reinterpret_cast< std::add_pointer_t<Vertex*>>(_a[1]))); break;
        case 3: _t->on_actionQuit_triggered(); break;
        case 4: _t->on_openOBJ_clicked(); break;
        case 5: _t->on_vertsListWidget_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 6: _t->on_halfEdgesListWidget_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 7: _t->on_facesListWidget_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 8: _t->on_splitEdge_clicked(); break;
        case 9: _t->on_subdivide_clicked(); break;
        case 10: _t->on_pushButton_clicked(); break;
        case 11: _t->onVertexPositionChanged(); break;
        case 12: _t->onFaceColorChanged(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSize,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t
, QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<HalfEdge *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Face *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Vertex *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
