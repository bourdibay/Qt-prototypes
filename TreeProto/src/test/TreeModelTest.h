
#ifndef __TREE_MODEL_TEST_H__
#define __TREE_MODEL_TEST_H__

#include <QDebug>
#include "TreeViewModel.h"

/**
* \brief Structure that we want to display.
*/
struct ModelElem {
    ModelElem(QString const &n, const int b) : name(n), nb(b) {}
    QString name;
    int nb;
};

struct CategoryElem {
    CategoryElem(QString const &n) : name(n) {}
    QString name;
};

/**
* \brief TreeModel's item that handles ModelElem structure.
*/
class TestItem : public TreeItem<ModelElem> {
public:
    TestItem(ATreeItem *parent = nullptr) : TreeItem(nullptr, parent) {}
    TestItem(ModelElem *data, ATreeItem *parent = nullptr)
    : TreeItem(data, parent) {}

    virtual TestItem *clone() const {
        return TreeItem<ModelElem>::clone<TestItem>();
    }

    virtual int columnCount() const {
        return 2; // name + nb
    }

    virtual Qt::ItemFlags flags(const QModelIndex &idx) const {
        return ATreeItem::flags(idx) | Qt::ItemFlag::ItemIsEditable |
               Qt::ItemIsDragEnabled;
    }

    virtual QVariant data(QModelIndex const &index, int role) const {
        if (role != Qt::DisplayRole) {
            return QVariant();
        }
        const int column = index.column();
        switch (column) {
            case 0:
                return _data->name;
            case 1:
                return _data->nb;
        }
        return QVariant();
    }

    virtual bool setData(QModelIndex const &index, const QVariant &value,
                         int role) {
        if (!value.isValid()) {
            return false;
        }
        if (role == Qt::EditRole) {
            const int column = index.column();
            switch (column) {
                case 0:
                    _data->name = value.toString();
                    return true;
                case 1:
                    _data->nb = value.toInt();
                    return true;
            }
        }
        return false;
    }
};

class CategoryItem : public TreeItem<CategoryElem> {
public:
    CategoryItem(ATreeItem *parent = nullptr) : TreeItem(nullptr, parent) {}
    CategoryItem(CategoryElem *data, ATreeItem *parent = nullptr)
    : TreeItem(data, parent) {}

    virtual CategoryItem *clone() const {
        return TreeItem<CategoryElem>::clone<CategoryItem>();
    }

    virtual int columnCount() const { return 1; }

    virtual Qt::ItemFlags flags(const QModelIndex &idx) const {
        return ATreeItem::flags(idx) | Qt::ItemFlag::ItemIsEditable |
               Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }

    virtual QVariant data(QModelIndex const &index, int role) const {
        if (role == Qt::BackgroundRole) {
            return QColor("red");
        } else if (role == Qt::DisplayRole) {
            if (index.column() == 0) {
                return _data->name;
            }
        }
        return QVariant();
    }

    virtual bool setData(QModelIndex const &index, const QVariant &value,
                         int role) override {
        if (!value.isValid()) {
            return false;
        }
        if (role == Qt::EditRole) {
            if (index.column() == 0) {
                _data->name = value.toString();
                return true;
            }
        }
        return false;
    }

    virtual void expanded() override {
        qDebug() << "As a categoryItem, I am expanded.";
    }
};

#endif
