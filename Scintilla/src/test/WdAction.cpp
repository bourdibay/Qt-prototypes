//
//   WdAction.cpp
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:46:20
//
//
//

// Qt classes
#include <QObject>
#include <QString>
#include <QAction>
#include <QIcon>

// public graphical class
#include "WdAction.h"

WdAction::WdAction(QAction *action, QString const &keyname)
: QAction(action), _key(keyname) {
}

WdAction::WdAction(QString const &keyname, QObject *parent)
: QAction(parent), _key(keyname) {
}

WdAction::WdAction(QString const &keyname, QString const &text, QObject *parent)
: QAction(text, parent), _key(keyname) {
}

WdAction::WdAction(QString const &keyname, QIcon const &icon,
                   QString const &text, QObject *parent)
: QAction(icon, text, parent), _key(keyname) {
}

QString const &WdAction::getKey() const {
    return _key;
}
