
#ifndef __WINDOW_TEST_H__
#define __WINDOW_TEST_H__

// Qt classes
#include <QMainWindow>
#include <QHBoxLayout>
#include <QDebug>
#include <QTreeView>
#include <QMenu>
#include <QPoint>
#include <QAction>

// my classes
#include "../TreeViewModel.h"
#include "test/TreeModelTest.h"

// Test window for the TreeViewModel class.
// It implements a context menu to demonstrate
// very basic features (add/remove element).
class WindowTest : public QMainWindow {
    Q_OBJECT
public:
    WindowTest()
    : QMainWindow()
    , _view(new TreeView())
    , _model(new TreeViewModel(QStringList() << "First"
                                             << "Second")) {
        _view->enableDragDrop(true);

        QWidget *cont = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout();
        cont->setLayout(layout);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(_view);
        _view->setModel(_model);

        _view->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        connect(_view, SIGNAL(customContextMenuRequested(const QPoint &)), this,
                SLOT(slot_customContextMenuRequested(const QPoint &)));

        this->setCentralWidget(cont);

        populateTree();
    }

private slots:
    void slot_customContextMenuRequested(const QPoint & /*pos*/) {
        QMenu menu;
        QAction *action_add_category = new QAction("add category", this);
        connect(action_add_category, SIGNAL(triggered()), this,
                SLOT(slot_add_category()));
        QAction *action_add = new QAction("add element", this);
        connect(action_add, SIGNAL(triggered()), this, SLOT(slot_addElement()));
        QAction *action_add_inside = new QAction("add element inside", this);
        connect(action_add_inside, SIGNAL(triggered()), this,
                SLOT(slot_addElementInside()));
        QAction *action_delete = new QAction("delete element", this);
        connect(action_delete, SIGNAL(triggered()), this,
                SLOT(slot_deleteElement()));
        QAction *action_append = new QAction("append element", this);
        connect(action_append, SIGNAL(triggered()), this,
                SLOT(slot_appendElement()));
        QAction *action_removeAll = new QAction("remove all elements", this);
        connect(action_removeAll, SIGNAL(triggered()), this,
                SLOT(slot_removeAll()));
        menu.addAction(action_add_category);
        menu.addAction(action_add);
        menu.addAction(action_add_inside);
        menu.addAction(action_delete);
        menu.addAction(action_append);
        menu.addAction(action_removeAll);
        menu.exec(QCursor::pos());
    }

    void slot_removeAll() { _model->removeAll(); }

    void slot_add_category() {
        CategoryElem *elem = createCategoryElem();
        const QModelIndex currentIndex = _view->currentIndex();
        const auto currentItem = _model->getItem(currentIndex);
        auto parent = currentItem->getParent();
        if (!parent) {
            parent = _model->getRoot();
        }
        const QModelIndex indexParent = _model->index(parent);
        CategoryItem *toAdd = new CategoryItem(elem, parent);
        int row = currentIndex.row();
        if (row < 0) {
            row = 0;
        }
        _model->insertItem(row, toAdd, indexParent);
    }

    void slot_addElement() {
        ModelElem *elem = createModelElem();
        const QModelIndex currentIndex = _view->currentIndex();
        const auto currentItem = _model->getItem(currentIndex);
        auto parent = currentItem->getParent();
        if (!parent) {
            parent = _model->getRoot();
        }
        const QModelIndex indexParent = _model->index(parent);
        TestItem *toAdd = new TestItem(elem, parent);
        int row = currentIndex.row();
        if (row < 0) {
            row = 0;
        }
        _model->insertItem(row, toAdd, indexParent);
    }

    void slot_appendElement() {
        ModelElem *elem = createModelElem();
        const QModelIndex currentIndex = _view->currentIndex();
        TestItem *toAdd = new TestItem(elem, _model->getItem(currentIndex));
        _model->appendItem(toAdd, currentIndex);
    }

    void slot_addElementInside() {
        ModelElem *elem = createModelElem();
        const QModelIndex currentIndex = _view->currentIndex();
        const auto currentItem = _model->getItem(currentIndex);
        TestItem *toAdd = new TestItem(elem, currentItem);
        _model->insertItem(0, toAdd, currentIndex);
    }

    void slot_deleteElement() {
        const QModelIndex currentIndex = _view->currentIndex();
        if (currentIndex.isValid()) {
            auto toRm = _model->getItem(currentIndex);
            auto parent = toRm->getParent();
            _model->removeItems(currentIndex.row(), 1, _model->index(parent));
        }
    }

private:
    void populateTree() {
        auto root = _model->getRoot();
        root->appendChild(
            new TestItem(new ModelElem("First element after root", 1), root));
        _model->insertItem(
            0, new TestItem(new ModelElem("Next element added at idx 0", 1)));
        QModelIndex idx = _model->index(0, 0, root);

        QModelIndex i = _model->index(root);
        ATreeItem *f = root->getChild(0);
        QModelIndex idx_f = _model->index(f);

        _model->insertItem(0, new TestItem(new ModelElem("First", 1)));
        _model->insertItems(1, QVector<ATreeItem *>()
                                   << new TestItem(new ModelElem("Two", 2))
                                   << new TestItem(new ModelElem("Three", 3))
                                   << new TestItem(new ModelElem("Four", 4))
                                   << new TestItem(new ModelElem("Five", 5)));

        _model->insertItems(0, QVector<ATreeItem *>()
                                   << new TestItem(new ModelElem("__two", 2))
                                   << new TestItem(new ModelElem("__three", 3))
                                   << new TestItem(new ModelElem("__four", 4))
                                   << new TestItem(new ModelElem("__five", 5)),
                            _model->index(2, 0));
    }

    ModelElem *createModelElem() {
        static int i = 0;
        ModelElem *elem =
            new ModelElem(QString("new_elem_inside_%1").arg(i), i);
        ++i;
        return elem;
    }
    CategoryElem *createCategoryElem() {
        static int i = 0;
        CategoryElem *elem = new CategoryElem(QString("Category_%1").arg(i));
        ++i;
        return elem;
    }

    TreeView *_view;
    TreeViewModel *_model;
};

#endif
