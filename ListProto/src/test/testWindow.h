
#ifndef __WINDOW_TEST_H__
#define __WINDOW_TEST_H__

// Qt classes
#include <QMainWindow>
#include <QHBoxLayout>
#include <QDebug>
#include <QListView>
#include <QPoint>
#include <QMenu>
#include <QAction>

// my classes
#include "../ListViewModel.h"
#include "test/ListModelTest.h"

// Test window for the ListViewModel class.
// It implements a context menu to demonstrate
// very basic features (add/remove element).
class WindowTest : public QMainWindow {
    Q_OBJECT
public:
    WindowTest()
    : QMainWindow(), _view(new QListView()), _model(new ListModel()) {
        QWidget *cont = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        cont->setLayout(layout);

        layout->addWidget(_view);
        /*
        _view->setDragEnabled(true);
        _view->setAcceptDrops(true);
        _view->setDragDropMode(QListView::InternalMove);
        _view->setDragDropOverwriteMode(false);
        _view->setMovement(QListView::Snap);
        */
        _view->setDefaultDropAction(Qt::MoveAction);
        _view->setSelectionMode(QAbstractItemView::ExtendedSelection);
        _view->setDragEnabled(true);
        _view->setAcceptDrops(true);
        _view->setDropIndicatorShown(true);

        _view->setModel(_model);

        _view->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        connect(_view, SIGNAL(customContextMenuRequested(const QPoint &)), this,
                SLOT(slot_customContextMenuRequested(const QPoint &)));

        this->setCentralWidget(cont);

        populateList();
    }

private slots:
    void slot_customContextMenuRequested(const QPoint & /*pos*/) {
        QMenu menu;
        QAction *action_add = new QAction("add element", this);
        connect(action_add, SIGNAL(triggered()), this, SLOT(slot_addElement()));
        QAction *action_delete = new QAction("delete element", this);
        connect(action_delete, SIGNAL(triggered()), this,
                SLOT(slot_deleteElement()));
        menu.addAction(action_add);
        menu.addAction(action_delete);
        menu.exec(QCursor::pos());
    }

    void slot_addElement() {
        static int i = 0;
        ModelElem *elem = new ModelElem(QString("new_elem_%1").arg(i), i);
        ++i;
        const QModelIndex currentIndex = _view->currentIndex();
        _model->insertElement(elem, currentIndex.row());
    }

    void slot_deleteElement() {
        const QModelIndex currentIndex = _view->currentIndex();
        _model->removeElement(currentIndex);
    }

private:
    void populateList() {
        _model->appendElement(new ModelElem("First element", 1));
        _model->appendElement(new ModelElem("Second element", 2));
        _model->appendElement(new ModelElem("Third element", 3));
        _model->appendElement(new ModelElem("Forth element", 4));
        _model->appendElement(new ModelElem("Fifth element", 5));
    }

    QListView *_view;
    ListViewModel<ModelElem> *_model;
};

#endif
