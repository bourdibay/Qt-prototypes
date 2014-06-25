
#ifndef __TREE_VIEW_MODEL_H__
#define __TREE_VIEW_MODEL_H__

// Qt
#include <QVector>
#include <QVariant>
#include <QTreeView>
#include <QMimeData>
#include <QModelIndex>
#include <QStringList>
#include <QSharedPointer>
#include <QAbstractItemModel>

// TODO: TO FIX: bug when we drag drop the same item in itself
// in another view. (see ModelSharing test)

class ATreeItem {
public:
    ATreeItem(ATreeItem *parent = nullptr);
    virtual ~ATreeItem();
    virtual ATreeItem *getChild(const int row) const;
    virtual ATreeItem *getParent() const;
    virtual void setParent(ATreeItem *parent);
    virtual int getChildrenNumber() const;
    virtual int getChildIndex() const;
    virtual bool insertChild(const int position, ATreeItem *child);
    virtual bool appendChild(ATreeItem *child);
    virtual bool removeChildren(const int position, const int count);
    virtual Qt::ItemFlags flags(const int /*column*/) const;
    virtual bool setData(int column, const QVariant &value, int role);

    // Pure methods
    virtual int columnCount() const = 0;
    virtual QVariant data(int column, int role) const = 0;
    virtual ATreeItem *clone() const = 0;

    // Callback methods called after receiving a specific signal
    virtual void expanded() {}
    virtual void collapsed() {}
    // activated = single/double click, return/enter
    virtual void activated() {}
    virtual void clicked() {}
    virtual void doubleClicked() {}
    // the mouse enters in the item
    virtual void entered() {}
    virtual void pressed() {}
    // context menu requested
    virtual void customContextMenu(const QPoint & /*pos*/) {}

protected:
    ATreeItem *_parent;
    QVector<ATreeItem *> _children;
};

/**
* \brief The internal representation of a model item.
* The model contains a list of TreeItem and performs actions on them
* (add/remove).
* To add a new element in a TreeViewModel, you have to subclass it and
* implement the methods (columnCount(), data() and setData()) specific for each
* data structure you want to display.
* The template T corresponds to the structure that contains the data to display.
* The subclass manipulates this structure and display/modify the correct
* variable in the structure.
*/
template <typename T>
class TreeItem : public ATreeItem {
public:
    TreeItem(T *data, ATreeItem *parent = nullptr)
    : ATreeItem(parent), _data(data) {}

    virtual ~TreeItem() { delete _data; }
    T *getData() const { return _data; }

protected:
    template <typename RealType>
    RealType *clone() const {
        T *elem = new T(*_data);
        RealType *item = new RealType(elem, nullptr);
        auto children = _children;
        for (const ATreeItem *child : children) {
            item->appendChild(child->clone());
        }
        return item;
    }

    T *_data;

private:
    Q_DISABLE_COPY(TreeItem);
};

class RootItem : public ATreeItem {
public:
    RootItem(QStringList const &headerData = QStringList());
    virtual ~RootItem() {}

    virtual int columnCount() const;
    virtual QVariant data(int /*column*/, int /*role*/) const {
        return QVariant();
    }
    virtual bool setData(int /*column*/, const QVariant & /*value*/,
                         int /*role*/) {
        return false;
    }
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual RootItem *clone() const;

private:
    const QStringList _headerData;
};

class TreeItemMimeData : public QMimeData {
    Q_OBJECT

public:
    TreeItemMimeData();
    virtual ~TreeItemMimeData() {}
    QVector<ATreeItem *> const &getItems() const;
    QStringList formats() const;
    void addItem(ATreeItem *item);

private:
    QVector<ATreeItem *> _items;
    QStringList _format;
};

/**
* \brief A basic TreeModel suitable for basic data structures.
* Subclassing it should not be necessary for basic usage.
* You should subclass it to manage Files/Directories transfert from
* the filesystem to the model. To do that you need to override:
*    - The mimeType() should specify "text/uri-list" and "text/directory"
*    - The dropMimeDate() should handle the date if data->hasUrl(),
* and then call the base method (to keep internal move).
*/
class TreeViewModel : public QAbstractItemModel {
public:
    TreeViewModel(QObject *parent = nullptr);
    TreeViewModel(QStringList const &headerData, QObject *parent = nullptr);
    virtual ~TreeViewModel() { delete _root; }

    ATreeItem *getRoot() const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;
    ATreeItem *getItem(const QModelIndex &index) const;
    template <typename T>
    T *getItem(const QModelIndex &index) const;
    virtual int
    columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value,
                         int role = Qt::EditRole) override;
    virtual QModelIndex
    index(int row, int column,
          const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, const ATreeItem *parent) const;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertItems(const int position, QVector<ATreeItem *> const &items,
                     const QModelIndex &parent = QModelIndex());
    bool insertItem(const int position, ATreeItem *item,
                    const QModelIndex &parent = QModelIndex());
    bool appendItems(QVector<ATreeItem *> items,
                     const QModelIndex &parent = QModelIndex());
    bool appendItem(ATreeItem *item, const QModelIndex &parent = QModelIndex());
    bool removeItems(const int position, const int count,
                     const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count,
                            const QModelIndex &parent = QModelIndex()) override;
    bool removeAll();
    QModelIndex index(const ATreeItem *item) const;

    // Drag/drop
    virtual Qt::DropActions supportedDropActions() const override;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
    virtual QStringList mimeTypes() const override;
    // performs only internal items drag/drop.
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column,
                              const QModelIndex &parent) override;

private:
    RootItem *_root;
};

class TreeView : public QTreeView {
public:
    TreeView(QWidget *parent = nullptr);
    virtual ~TreeView() {}
    void enableDragDrop(const bool st);
    void keyPressEvent(QKeyEvent *event);

    void setTreeViewModel(TreeViewModel *model);
};

#endif
