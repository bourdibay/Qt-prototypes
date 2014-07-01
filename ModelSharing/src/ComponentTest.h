
#ifndef __COMPONENT_TEST_H__
#define __COMPONENT_TEST_H__

#include <QTreeView>
#include <QMenu>
#include <QAction>

#include "Area/SideAreaComponent.h"
#include "TreeViewModel.h"

struct Data {
    Data(QString const &s, const int n) : str(s), nb(n) {}
    QString str;
    int nb;
};

class DataItem : public TreeItem<Data> {
public:
    DataItem(ATreeItem *parent = nullptr) : TreeItem(nullptr, parent) {}
    DataItem(std::unique_ptr<Data> data, ATreeItem *parent = nullptr)
    : TreeItem(std::move(data), parent) {}

    virtual DataItem *clone() const {
        return TreeItem<Data>::clone<DataItem>();
    }

    virtual int columnCount() const {
        return 2; // name + nb
    }

    virtual QVariant data(QModelIndex const &index, int role) const {
        if (role != Qt::DisplayRole) {
            return QVariant();
        }
        const int column = index.column();
        switch (column) {
            case 0:
                return _data->str;
            case 1:
                return _data->nb;
        }
        return QVariant();
    }

    virtual bool setData(QModelIndex const &index, const QVariant &value,
                         int role) override {
        if (!value.isValid()) {
            return false;
        }
        if (role != Qt::EditRole) {
            const int column = index.column();
            switch (column) {
                case 0:
                    _data->str = value.toString();
                    return true;
                case 1:
                    _data->nb = value.toInt();
                    return true;
            }
        }
        return false;
    }
};

#include <QSharedPointer>

class ComponentTest : public SideAreaComponent {
    Q_OBJECT
public:
    ComponentTest(QString const &key, QString const &name,
                  QWidget *parent = nullptr)
    : SideAreaComponent(key, name, parent), _view(new TreeView()) {
        _view->enableDragDrop(true);
        _model = QSharedPointer<TreeViewModel>(new TreeViewModel(QStringList()
                                                                 << "First"
                                                                 << "Second"));
        _view->setModel(_model.data());
        _layout->addWidget(_view);

        _view->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        connect(_view, SIGNAL(customContextMenuRequested(const QPoint &)), this,
                SLOT(slot_customContextMenuRequested(const QPoint &)));
    }
    ComponentTest(ComponentTest const &copy)
    : SideAreaComponent(copy), _view(new TreeView()), _model(copy._model) {
        _view->enableDragDrop(true);
        _view->setModel(_model.data());
        _layout->addWidget(_view);
        _view->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        connect(_view, SIGNAL(customContextMenuRequested(const QPoint &)), this,
                SLOT(slot_customContextMenuRequested(const QPoint &)));
    }

    virtual ~ComponentTest() {}

private slots:
    void slot_customContextMenuRequested(const QPoint & /*pos*/) {
        QMenu menu;
        QAction *action_add = new QAction("add element", this);
        connect(action_add, SIGNAL(triggered()), this, SLOT(slot_addElement()));
        QAction *action_add_inside = new QAction("add element inside", this);
        connect(action_add_inside, SIGNAL(triggered()), this,
                SLOT(slot_addElementInside()));
        QAction *action_delete = new QAction("delete element", this);
        connect(action_delete, SIGNAL(triggered()), this,
                SLOT(slot_deleteElement()));
        menu.addAction(action_add);
        menu.addAction(action_add_inside);
        menu.addAction(action_delete);
        menu.exec(QCursor::pos());
    }

    void slot_addElement() {
        std::unique_ptr<Data> elem = createData();
        const QModelIndex currentIndex = _view->currentIndex();
        const auto currentItem = _model->getItem(currentIndex);
        auto parent = currentItem->getParent();
        if (!parent) {
            parent = _model->getRoot();
        }
        const QModelIndex indexParent = _model->index(parent);
        DataItem *toAdd = new DataItem(std::move(elem), parent);
        int row = currentIndex.row();
        if (row < 0) {
            row = 0;
        }
        _model->insertItem(row, toAdd, indexParent);
    }

    void slot_addElementInside() {
        std::unique_ptr<Data> elem = createData();
        const QModelIndex currentIndex = _view->currentIndex();
        const auto currentItem = _model->getItem(currentIndex);
        DataItem *toAdd = new DataItem(std::move(elem), currentItem);
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
    std::unique_ptr<Data> createData() {
        static int i = 0;
        std::unique_ptr<Data> elem = std::unique_ptr<Data>(
            new Data(QString("new_elem_inside_%1").arg(i), i));
        ++i;
        return std::move(elem);
    }

    TreeView *_view;
    //    std::unique_ptr<TreeViewModel> _model;
    QSharedPointer<TreeViewModel> _model;
    //    std::unique_ptr<TreeViewModel> _model;
};

#endif
