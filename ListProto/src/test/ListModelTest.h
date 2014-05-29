
#ifndef __LIST_MODEL_TEST_H__
#define __LIST_MODEL_TEST_H__

// Qt classes
#include <QDebug>
#include <QString>
#include <QVariant>

// model class
#include "../ListViewModel.h"

/**
* \brief The element stored in the model.
* We want to display the name and the number in two columns.
*/
struct ModelElem
{
    ModelElem(QString const &n, const int b) : name(n), nb(b) {}
    QString name;
    int nb;
};

/**
* \brief Specific subclass of ListViewModel to
* implement data() and setData() methods
* in order to display the correct variable
* of the structure ModelElem.
*/
class ListModel : public ListViewModel<ModelElem>
{
    QVariant data(const QModelIndex &index, int role) const {
        if (index.isValid() == false) {
            return QVariant();
        }
        if (role == Qt::DisplayRole) {
            return _content[index.row()]->name;
        }
        return QVariant();
    }

    bool setData(const QModelIndex &index, const QVariant &value,
        int role = Qt::EditRole) {
            if (index.isValid() == false) {
                return false;
            }
            if (role == Qt::EditRole) {
                const int row = index.row();
                _content[row]->name = value.toString();
                return true;
            }
            return false;
    }
};

#endif
