//
//   CallTip.cpp
//   Widev
//
//   Created by bourdi_b on 08 févr. 2014 19:28:31
//
//
//

// STL
#include <algorithm>
#include <sstream>

// Qt classes
#include <QVBoxLayout>
#include <QTableWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QFont>
#include <QDebug>
#include <QTableView>
#include <QHeaderView>
#include <QScrollBar>

// widev graphical classes
#include "TextEditor/CallTip.h"

CallTipModel::CallTipModel(std::vector<CompletionData> &data)
    : QAbstractTableModel(),
      _data(std::move(data)) {
}

int CallTipModel::rowCount(const QModelIndex &/*parent*/) const {
    return _data.size();
}

int CallTipModel::columnCount(const QModelIndex &/*parent*/) const {
    return 5;
}

QVariant CallTipModel::headerData(int /*section*/, Qt::Orientation /*orientation*/,
        int /*role*/) const {
    // no header visible
    return QVariant();
}

QVariant CallTipModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == Qt::FontRole) {
        return _font;
    }

    if (role == Qt::DisplayRole) {
        const int row = index.row();
        CompletionData d = _data[row];
        switch (index.column()) {
        case 0: // type of symbol
            switch (d.kind) {
            case CompletionData::Kind::Function:
                return QVariant("FCT");
            case CompletionData::Kind::Variable:
                return QVariant("VAR");
            case CompletionData::Kind::Unknown:
                return QVariant("");
            }
        case 1: // return type
            return QVariant(QString::fromStdString(d.ret));
        case 2: // name symbol
            return QVariant(QString::fromStdString(d.word));
        case 3: // params
            if (d.kind == CompletionData::Kind::Function) {
                std::ostringstream oss;
                if (d.parameters.empty() == false) {
                    std::copy(std::begin(d.parameters), std::end(d.parameters) - 1,
                              std::ostream_iterator<std::string>(oss, ", "));
                    oss << d.parameters.back();
                }
                QString s = QString::fromStdString(oss.str());
                return QVariant(s);
            }
            return QVariant();
        case 4: // opt params
            if (d.kind == CompletionData::Kind::Function) {
                std::ostringstream oss;
                if (d.optionalParameters.empty() == false) {
                    std::copy(std::begin(d.optionalParameters),
                              std::end(d.optionalParameters) - 1,
                              std::ostream_iterator<std::string>(oss, ", "));
                    oss << d.optionalParameters.back();
                }
                QString s = QString::fromStdString(oss.str());
                return QVariant(s);
            }
            return QVariant();
        default:
            return QVariant();
        }

    }
    return QVariant();
}

void CallTipModel::setFont(QFont const &font) {
    _font = font;
}

///////////// CALL TIP ///////////////

CallTip::CallTip(QWidget *parent)
    : QWidget(parent),
      _list(new QTableView()),
      _model(nullptr) {
    this->setMaximumHeight(100);
    this->setMinimumHeight(0);

    this->setFocusProxy(parent);
    _list->setFocusProxy(this);

    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    layout->addWidget(_list);

    this->hide();

    setContentsMargins(0, 0, 0, 0);
    layout->setContentsMargins(0, 0, 0, 0);

    _list->verticalHeader()->hide();
    _list->horizontalHeader()->hide();
    _list->horizontalHeader()->setMinimumSectionSize(0);
    _list->setShowGrid(false);

    _list->verticalScrollBar()->setMaximumWidth(20);
    _list->horizontalScrollBar()->setMaximumHeight(20);

    _list->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    _list->setTabKeyNavigation(true);

    //_list->horizontalHeader()->setStretchLastSection(true);
    //_list->verticalHeader()->setStretchLastSection(true);
}

void CallTip::setContent(std::vector<CompletionData> &content) {
    if (_model) {
        delete _model;
    }
    _model = new CallTipModel(content);
    _model->setFont(parentWidget()->font());
    _list->setModel(_model);

    _list->resizeColumnsToContents();
    //_list->resizeRowsToContents();
    _list->verticalHeader()->setDefaultSectionSize(_list->verticalHeader()->minimumSectionSize());

    int widthToSet = _list->horizontalHeader()->length();
    int heightToSet = _list->verticalHeader()->length();

    if (heightToSet > maximumHeight()) {
        widthToSet += _list->verticalScrollBar()->width();
    }
    if (widthToSet > maximumWidth()) {
        heightToSet += _list->horizontalScrollBar()->height();
    }
    resize(widthToSet + 2, heightToSet + 2);
}

void CallTip::hideComplete() {
    _list->setCurrentIndex(QModelIndex());
    hide();
}

QModelIndex CallTip::getCurrentIndex() const {
    return _list->currentIndex();
}

bool CallTip::moveCurrentIndex(const int inc) {
    const QModelIndex idx = _list->currentIndex();
    QModelIndex nextIdx;
    if (idx.isValid() == false) {
        nextIdx = _model->index(0, 0);
    }
    else {
        nextIdx = _model->index(idx.row() + inc, idx.column());
    }
    if (nextIdx.isValid()) {
        _list->setCurrentIndex(nextIdx);
        return true;
    }
    return false;
}


void CallTip::sendEvents(QKeyEvent *ev) {
    ev->ignore();
    const int key = ev->key();
    switch (key) {
    case Qt::Key_Up:
        moveCurrentIndex(-1);
        ev->accept();
        break;
    case Qt::Key_Down:
        moveCurrentIndex(1);
        ev->accept();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return: {
        qDebug() << "enter ou return calltip";
        const QModelIndex idx = _list->currentIndex();
        if (idx.isValid()) {
            emit entryChosen(idx.data().toString());
            ev->accept();
        }
        hideComplete();
        break;
    }
    default:
        break;
    }
}
