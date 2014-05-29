
#ifndef __LIST_VIEW_MODEL_H__
#define __LIST_VIEW_MODEL_H__

#include <QAbstractListModel>
#include <QVector>

/**
* \brief Ease the use of Qt's list model.
* It takes as a template a structure/class that 
* is going to be displayed via the data() method.
* The variables of this template will be changed
* via setData().
* data() and setData() must be implemented by subclasses.
* Only those two methods need to be implemented.
*/
// T = element to store
template<typename T>
class ListViewModel : public QAbstractListModel
{
public:
    ListViewModel() 
        : QAbstractListModel()
    { }

    virtual ~ListViewModel() { qDeleteAll(_content); }
    virtual QVariant data(const QModelIndex &index, int role) const = 0;
    virtual bool setData(const QModelIndex &index, const QVariant &value,
        int role = Qt::EditRole) = 0;
        
    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        return _content.size();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled |
            Qt::ItemIsEditable;
    }

    void removeRow(const int row) {
        beginRemoveRows(QModelIndex(), row, row);
        delete _content.takeAt(row);
        endRemoveRows();
    }

    void removeElement(T *data) {
        const auto &end = std::end(_content);
        const auto &begin = std::begin(_content);
        const auto &it = std::find(begin, end, data);
        if (it != end) {
            const int row = std::distance(begin, it);
            removeRow(row);
        }
    }

    void removeElement(QModelIndex const &index) {
        removeElement(getElement(index));
    }

    void insertElement(T *data, int idx) {
        if (idx < 0) {
            idx = 0;
        }
        beginInsertRows(QModelIndex(), idx, idx);
        _content.insert(idx, data);
        endInsertRows();
    }

    void appendElement(T *data) {
        insertElement(data, _content.size());
    }

    T *getElement(QModelIndex const &index) const {
        if (index.isValid() == false) {
            return nullptr;
        }
        const int row = index.row();
        if (row >= _content.size()) {
            return nullptr;
        }
        return _content.at(row);
    }

    T *getElement(const int row) const {
        const QModelIndex index = this->index(row);
        return getElement(index);
    }

    bool setData(const int row, QVariant const &data) {
        const QModelIndex index = this->index(row);
        return setData(index, data);
    }

protected:
    QVector<T*> _content;
};

#endif
