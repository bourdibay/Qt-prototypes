
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

    virtual Qt::ItemFlags flags(const int column) const {
        return ATreeItem::flags(column) | Qt::ItemFlag::ItemIsEditable |
               Qt::ItemIsDragEnabled;
    }

    virtual QVariant data(int column, int role) const {
        if (role != Qt::DisplayRole) {
            return QVariant();
        }
        switch (column) {
            case 0:
                return _data->name;
            case 1:
                return _data->nb;
        }
        return QVariant();
    }

    virtual bool setData(int column, const QVariant &value, int role) {
        if (!value.isValid()) {
            return false;
        }
        if (role == Qt::EditRole) {
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

    virtual QVariant data(int column, int role) const {
        if (role != Qt::DisplayRole) {
            return QVariant();
        }
        switch (column) {
            case 0:
                return _data->name;
        }
        return QVariant();
    }

    virtual bool setData(int column, const QVariant &value, int role) {
        if (!value.isValid()) {
            return false;
        }
        if (role == Qt::EditRole) {
            switch (column) {
                case 0:
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
