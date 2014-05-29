//
//   CallTip.h
//   Widev
//
//   Created by bourdi_b on 08 févr. 2014 19:27:12
//
//
//

#ifndef __CALL_TIP_H__
#define __CALL_TIP_H__

// STL
#include <string>
#include <vector>

// Qt classes
#include <QWidget>
#include <QAbstractTableModel>

// Qt classes
class QTableWidget;
class QTableView;

class QEvent;

struct CompletionData {
    enum Kind { Unknown, Variable, Function };

    CompletionData() : kind(Unknown) {}

    std::string         ret;
    std::string         word;
    std::vector<std::string> parameters;
    std::vector<std::string> optionalParameters;
    Kind                kind;
};

class CallTipModel : public QAbstractTableModel {
public:
    CallTipModel(std::vector<CompletionData> const &data);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const;

    void setFont(QFont const &font);
private:
    std::vector<CompletionData> _data;
    QFont _font;
};

class CallTip : public QWidget {
    Q_OBJECT
public:
    CallTip(QWidget *parent);

    // TODO: set list...

    void sendEvents(QKeyEvent *ev);
    void hideComplete();
    bool moveCurrentIndex(const int inc);
    QModelIndex getCurrentIndex() const;
    //        void setContent(std::vector<QString> const &content);
    void setContent(std::vector<CompletionData> const &content);

signals:
    void entryChosen(QString const &text);

private:
    Q_DISABLE_COPY(CallTip);

    QTableView *_list;
    CallTipModel *_model;
};

#endif
