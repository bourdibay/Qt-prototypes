#include <QKeyEvent>
#include <QMouseEvent>

#include "TreeViewModel.h"

/*****************************************************************************/
/* ATreeItem                                                                 */
/*****************************************************************************/

ATreeItem::ATreeItem(ATreeItem *parent) : _parent(parent), _children() {
}

ATreeItem::~ATreeItem() {
    qDeleteAll(_children);
}

ATreeItem *ATreeItem::getChild(const int row) const {
    if (row >= 0 && row < _children.size()) {
        return _children.at(row);
    }
    return nullptr;
}

ATreeItem *ATreeItem::getParent() const {
    return _parent;
}

void ATreeItem::setParent(ATreeItem *parent) {
    _parent = parent;
}

int ATreeItem::getChildrenNumber() const {
    return _children.size();
}

int ATreeItem::getChildIndex() const {
    if (_parent) {
        return _parent->_children.indexOf(const_cast<ATreeItem *>(this));
    }
    return 0;
}

bool ATreeItem::insertChild(const int position, ATreeItem *child) {
    if (position < 0 || position > _children.size()) {
        return false;
    }
    _children.insert(position, child);
    child->setParent(this);
    return true;
}

bool ATreeItem::appendChild(ATreeItem *child) {
    return insertChild(getChildrenNumber(), child);
}

bool ATreeItem::removeChildren(const int position, const int count) {
    if (position < 0 || position + count > _children.size()) {
        return false;
    }
    for (int i = 0; i < count; ++i) {
        ATreeItem *item = _children.takeAt(position);
        delete item;
        item = nullptr;
    }
    return true;
}

Qt::ItemFlags ATreeItem::flags(const QModelIndex & /*index*/) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool ATreeItem::setData(const QModelIndex & /*index*/,
                        const QVariant & /*value*/, int /*role*/) {
    return false;
}

/*****************************************************************************/
/* RootItem                                                                  */
/*****************************************************************************/

RootItem::RootItem(QStringList const &headerData)
: ATreeItem(nullptr), _headerData(headerData) {
}

int RootItem::columnCount() const {
    return _headerData.length();
}

QVariant RootItem::headerData(int section, Qt::Orientation /*orientation*/,
                              int role) const {
    if (role == Qt::DisplayRole) {
        return _headerData.at(section);
    }
    return QVariant();
}

RootItem *RootItem::clone() const {
    return new RootItem(_headerData);
}

/*****************************************************************************/
/* TreeItemMimeData                                                          */
/*****************************************************************************/
TreeItemMimeData::TreeItemMimeData() : _items(), _format("treeItem") {
}

QVector<ATreeItem *> const &TreeItemMimeData::getItems() const {
    return _items;
}

QVector<ATreeItem *> const &TreeItemMimeData::getItemsOrignal() const {
    return _itemsOriginal;
}

QStringList TreeItemMimeData::formats() const {
    return _format;
}

void TreeItemMimeData::addItem(ATreeItem *item) {
    _items.append(item);
}

void TreeItemMimeData::addItemOriginal(ATreeItem *item) {
    _itemsOriginal.append(item);
}

/*****************************************************************************/
/* TreeViewModel                                                             */
/*****************************************************************************/

TreeViewModel::TreeViewModel(QObject *parent)
: QAbstractItemModel(parent), _root(new RootItem()) {
}

TreeViewModel::TreeViewModel(QStringList const &headerData, QObject *parent)
: QAbstractItemModel(parent), _root(new RootItem(headerData)) {
}

ATreeItem *TreeViewModel::getRoot() const {
    return _root;
}

QVariant TreeViewModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const {
    return _root->headerData(section, orientation, role);
}

ATreeItem *TreeViewModel::getItem(const QModelIndex &index) const {
    return getItem<ATreeItem>(index);
}

template <typename T>
T *TreeViewModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        T *item = static_cast<T *>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return _root;
}

int TreeViewModel::columnCount(const QModelIndex & /*parent*/) const {
    return _root->columnCount();
}

int TreeViewModel::rowCount(const QModelIndex &parent) const {
    const ATreeItem *parentItem = getItem(parent);
    return parentItem->getChildrenNumber();
}

QVariant TreeViewModel::data(const QModelIndex &index, int role) const {
    if (index.isValid()) {
        const ATreeItem *item = getItem(index);
        return item->data(index, role);
    }
    return QVariant();
}

bool TreeViewModel::setData(const QModelIndex &index, const QVariant &value,
                            int role) {
    if (index.isValid()) {
        ATreeItem *item = getItem(index);
        const bool result = item->setData(index, value, role);
        if (result) {
            emit dataChanged(index, index, QVector<int>() << role);
        }
        return result;
    }
    return false;
}

QModelIndex TreeViewModel::index(int row, int column,
                                 const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }
    const ATreeItem *parentItem = getItem(parent);
    return index(row, column, parentItem);
}

QModelIndex TreeViewModel::index(int row, int column,
                                 const ATreeItem *parent) const {
    ATreeItem *childItem = parent->getChild(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex TreeViewModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }
    const ATreeItem *childItem = getItem(index);
    ATreeItem *parentItem = childItem->getParent();
    if (parentItem == _root) {
        return QModelIndex();
    }
    return createIndex(parentItem->getChildIndex(), 0, parentItem);
}

Qt::ItemFlags TreeViewModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
    }
    const ATreeItem *item = getItem(index);
    return item->flags(index);
}

bool TreeViewModel::insertItems(const int position,
                                QVector<ATreeItem *> const &items,
                                const QModelIndex &parent) {
    Q_ASSERT(position >= 0);
    ATreeItem *parentItem = getItem(parent);
    beginInsertRows(parent, position, position + items.size() - 1);
    bool success = false;
    int i = 0;
    for (ATreeItem *item : items) {
        success = parentItem->insertChild(position + i, item);
        ++i;
    }
    endInsertRows();
    return success;
}

bool TreeViewModel::insertItem(const int position, ATreeItem *item,
                               const QModelIndex &parent) {
    return insertItems(position, QVector<ATreeItem *>() << item, parent);
}

bool TreeViewModel::appendItems(QVector<ATreeItem *> items,
                                const QModelIndex &parent) {
    if (items.empty()) {
        return false;
    }
    const ATreeItem *parentItem = getItem(parent);
    const int nbChildren = parentItem->getChildrenNumber();
    return insertItems(nbChildren, items, parent);
}

bool TreeViewModel::appendItem(ATreeItem *item, const QModelIndex &parent) {
    const ATreeItem *parentItem = getItem(parent);
    const int nbChildren = parentItem->getChildrenNumber();
    return insertItem(nbChildren, item, parent);
}

bool TreeViewModel::removeItems(const int position, const int count,
                                const QModelIndex &parent) {
    ATreeItem *parentItem = getItem(parent);
    beginRemoveRows(parent, position, position + count - 1);
    const bool success = parentItem->removeChildren(position, count);
    endRemoveRows();
    return success;
}

bool TreeViewModel::removeRows(int row, int count, const QModelIndex &parent) {
    return removeItems(row, count, parent);
}

bool TreeViewModel::removeAll() {
    const int nb = _root->getChildrenNumber();
    if (nb > 0) {
        return removeItems(0, nb);
    }
    return false;
}

QModelIndex TreeViewModel::index(const ATreeItem *item) const {
    // We can't process root
    if (!item || item == _root) {
        return QModelIndex();
    }
    ATreeItem *parent = item->getParent();
    if (parent) {
        for (int i = 0; i < parent->getChildrenNumber(); ++i) {
            ATreeItem *child = parent->getChild(i);
            if (child == item) {
                const QModelIndex indexChild = index(i, 0, parent);
                return indexChild;
            }
        }
    }
    return QModelIndex();
}

Qt::DropActions TreeViewModel::supportedDropActions() const {
    return Qt::MoveAction;
}

QMimeData *TreeViewModel::mimeData(const QModelIndexList &indexes) const {
    TreeItemMimeData *mimeData = new TreeItemMimeData();
    QVector<ATreeItem *> items;
    for (const QModelIndex &idx : indexes) {
        if (idx.isValid()) {
            ATreeItem *item = getItem(idx);
            if (items.contains(item) == false) {
                items.append(item);
                mimeData->addItem(item->clone());
                mimeData->addItemOriginal(item);
            }
        }
    }
    return mimeData;
}

QStringList TreeViewModel::mimeTypes() const {
    return QAbstractItemModel::mimeTypes() << "treeItem";
}

bool TreeViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                 int row, int /*column*/,
                                 const QModelIndex &parent) {
    if (action == Qt::IgnoreAction)
        return true;
    const TreeItemMimeData *mimeData =
        qobject_cast<const TreeItemMimeData *>(data);
    if (mimeData) {
        bool ret;
        QVector<ATreeItem *> items = mimeData->getItems();
        if (row != -1) {
            ret = insertItems(row, items, parent);
        } else if (parent.isValid()) {
            ret = appendItems(items, parent);
        } else {
            ret = insertItems(rowCount(QModelIndex()), items);
        }
        QVector<ATreeItem *> itemsOriginal = mimeData->getItemsOrignal();
        auto itOriginal = std::begin(itemsOriginal);
        for (auto it = std::begin(items); it != std::end(items);
             ++it, ++itOriginal) {
            (*it)->dropped(index(*itOriginal), index(*it));
        }
        return ret;
    }
    return false;
}

/*****************************************************************************/
/* TreeView                                                                  */
/*****************************************************************************/

TreeView::TreeView(QWidget *parent) : QTreeView(parent) {
    setDefaultDropAction(Qt::MoveAction);
}

void TreeView::enableDragDrop(const bool st) {
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(st);
    setAcceptDrops(st);
    setDropIndicatorShown(st);
}

void TreeView::keyPressEvent(QKeyEvent *event) {
    QTreeView::keyPressEvent(event);
    if ((event->key() == Qt::Key_Up) || (event->key() == Qt::Key_Down)) {
        emit clicked(currentIndex());
    }
}

void TreeView::setTreeViewModel(TreeViewModel *model) {
    QTreeView::setModel(model);
    QObject::connect(this, &QTreeView::expanded, this,
                     [model](const QModelIndex &index) {
        ATreeItem *item = model->getItem(index);
        if (item) {
            item->expanded();
        }
    });
    QObject::connect(this, &QTreeView::collapsed, this,
                     [model](const QModelIndex &index) {
        ATreeItem *item = model->getItem(index);
        if (item) {
            item->collapsed();
        }
    });
    QObject::connect(this, &QTreeView::activated, this,
                     [model](const QModelIndex &index) {
        ATreeItem *item = model->getItem(index);
        if (item) {
            item->activated();
        }
    });
    QObject::connect(this, &QTreeView::clicked, this,
                     [this, model](const QModelIndex &index) {
        ATreeItem *item = model->getItem(index);
        if (item) {
            item->clicked();
        }
    });
    QObject::connect(this, &QTreeView::doubleClicked, this,
                     [model](const QModelIndex &index) {
        ATreeItem *item = model->getItem(index);
        if (item) {
            item->doubleClicked();
        }
    });
    QObject::connect(this, &QTreeView::entered, this,
                     [model](const QModelIndex &index) {
        ATreeItem *item = model->getItem(index);
        if (item) {
            item->entered();
        }
    });
    QObject::connect(this, &QTreeView::pressed, this,
                     [model](const QModelIndex &index) {
        ATreeItem *item = model->getItem(index);
        if (item) {
            item->pressed();
        }
    });
    QObject::connect(this, &QTreeView::customContextMenuRequested, this,
                     [this, model](const QPoint &pos) {
        const QModelIndex index = this->indexAt(pos);
        ATreeItem *item = model->getItem(index);
        if (item) {
            item->customContextMenu(pos);
        }
    });
}

void TreeView::mousePressEvent(QMouseEvent *ev) {
    QModelIndex idx = indexAt(ev->pos());
    if (!idx.isValid()) {
        this->clearSelection();
    }
    return QTreeView::mousePressEvent(ev);
}
