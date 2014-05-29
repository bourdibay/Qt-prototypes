//
//   WdMenu.cpp
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:46:27
//
//
//

// for qwarning
#include <QDebug>

// Qt classes
#include <QString>
#include <QWidget>
#include <QIcon>
#include <QAction>

// public graphical classes
#include "WdMenu.h"
#include "WdAction.h"

WdMenu::WdMenu(QString const &keyname, QWidget *parent)
    : QMenu(parent), _key(keyname), _actions(), _menus() {
}

WdMenu::WdMenu(QString const &keyname,
                          QString const &title,
                          QWidget *parent)
    : QMenu(title, parent),
      _key(keyname), _actions(), _menus() {
}

WdMenu::~WdMenu() {
    for (auto it = std::begin(_menus);
         it != std::end(_menus); ++it) {
        WdMenu *menu = it.value();
        menu->removeAction(menu->menuAction());
        delete menu;
    }
    for (auto it = std::begin(_actions);
         it != std::end(_actions); ++it) {
        WdAction *action = it.value();
        this->removeAction(action);
        delete action;
    }
    _actions.clear();
    _menus.clear();
}

void WdMenu::addWdAction(WdAction *action) {
    if (keyExists(action->getKey())) {
        // already exists, warning
        qWarning() << "The key " << action->getKey() << "already exists";
    }
    else {
        this->addAction(action);
        _actions[action->getKey()] = action;
    }
}

WdAction *WdMenu::addWdAction(const QString &keyname,
        QString const &text) {
    if (keyExists(keyname)) {
        // already exists, warning
        qWarning() << "The key " << keyname << "already exists";
    }
    else {
        WdAction *action = new WdAction(keyname, text, this);
        this->addWdAction(action);
        return action;
    }
    return nullptr;
}

WdAction *WdMenu::addWdAction(QString const &keyname,
        QIcon const &icon, QString const &text) {
    auto action = addWdAction(keyname, text);
    if (action) {
        action->setIcon(icon);
    }
    return action;
}

WdAction *WdMenu::addWdAction(QString const &keyname,
        QString const &text, QObject const *receiver,
        const char *slotName, const QKeySequence &shortcut) {
    auto action = addWdAction(keyname, text);
    if (action) {
        QObject::connect(action, SIGNAL(triggered(bool)), receiver, slotName);
        action->setShortcut(shortcut);
    }
    return action;
}

WdAction *WdMenu::addWdAction(QString const &keyname,
        QIcon const &icon,
        QString const &text, QObject const *receiver,
        const char *slotName, const QKeySequence &shortcut) {
    auto action = addWdAction(keyname, text, receiver, slotName, shortcut);
    if (action) {
        action->setIcon(icon);
    }
    return action;
}

void WdMenu::addWdMenu(WdMenu *menu) {
    if (keyExists(menu->getKey())) {
        // already exists, warning
        qWarning() << "The key " << menu->getKey() << "already exists";
    }
    else {
        this->addMenu(menu);
        _menus[menu->getKey()] = menu;
    }
}

WdMenu *WdMenu::addWdMenu(QString const &keyname,
        QString const &title) {
    if (keyExists(keyname)) {
        // already exists, warning
        qWarning() << "The key " << keyname << "already exists";
    }
    else {
        WdMenu *menu = new WdMenu(keyname, title);
        this->addWdMenu(menu);
        return menu;
    }
    return nullptr;
}

WdMenu *WdMenu::addWdMenu(QString const &keyname,
        QIcon const &icon,
        QString const &title) {
    auto menu = addWdMenu(keyname, title);
    if (menu) {
        menu->setIcon(icon);
    }
    return menu;
}

WdAction *WdMenu::addWdSection(QString const &keyname,
        QString const &text) {
    auto action = addWdAction(keyname, text);
    if (action) {
        action->setSeparator(true);
    }
    return action;
}

WdAction *WdMenu::addWdSection(QString const &keyname,
        QIcon const &icon,
        QString const &text) {
    auto action = addWdSection(keyname, text);
    if (action) {
        action->setIcon(icon);
    }
    return action;
}

WdAction *WdMenu::insertWdSection(QAction *before,
        QString const &keyname,
        QString const &text) {
    if (keyExists(keyname)) {
        // already exists, warning
        qWarning() << "The key " << keyname << "already exists";
    }
    else {
        WdAction *action = new WdAction(keyname, text, this);
        action->setSeparator(true);
        this->insertAction(before, action);
        _actions[keyname] = action;
        return action;
    }
    return nullptr;
}

WdAction *WdMenu::insertWdSection(QAction *before,
        QString const &keyname,
        QIcon const &icon,
        QString const &text) {
    auto action = insertWdSection(before, keyname, text);
    if (action) {
        action->setIcon(icon);
    }
    return action;
}

WdMenu *WdMenu::insertWdMenu(QAction *before,
        QString const &keyname,
        QString const &text) {
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    }
    else {
        WdMenu *menu = new WdMenu(keyname, text);
        insertWdMenu(before, menu);
        return menu;
    }
    return nullptr;
}

WdMenu *WdMenu::insertWdMenu(QAction *before,
        QString const &keyname,
        QIcon const &icon,
        QString const &text) {
    WdMenu *menu = insertWdMenu(before, keyname, text);
    if (menu) {
        menu->setIcon(icon);
    }
    return nullptr;
}

bool WdMenu::insertWdMenu(QAction *before,
                                     WdMenu *menu) {
    if (keyExists(menu->getKey())) {
        qWarning() << "The key" << menu->getKey() << "already exists";
        return false;
    }
    else {
        _menus[menu->getKey()] = menu;
        insertMenu(before, menu);
        return true;
    }
    return false;
}

WdAction *WdMenu::insertWdAction(QAction *before,
        QString const &keyname,
        QString const &text) {
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    }
    else {
        WdAction *action = new WdAction(keyname, text, this);
        insertWdAction(before, action);
        return action;
    }
    return nullptr;
}

WdAction *WdMenu::insertWdAction(QAction *before,
        QString const &keyname,
        QIcon const &icon,
        QString const &text) {
    WdAction *action = insertWdAction(before, keyname, text);
    if (action) {
        action->setIcon(icon);
    }
    return nullptr;
}

bool WdMenu::insertWdAction(QAction *before,
                                       WdAction *action) {
    if (keyExists(action->getKey())) {
        qWarning() << "The key" << action->getKey() << "already exists";
        return false;
    }
    else {
        _actions[action->getKey()] = action;
        insertAction(before, action);
        return true;
    }
    return false;
}

void WdMenu::removeWdAction(QString const &keyname) {
    WdAction *action = getWdAction(keyname);
    if (!action) {
        // does not exist
        qWarning() << "The key " << keyname << "does not exist";
    }
    else {
        _actions.remove(keyname);
        this->removeAction(action);
    }
}

bool WdMenu::removeRootWdMenu(QString const &keyname) {
    WdMenu *menu = getRootWdMenu(keyname);
    if (!menu) {
        // does not exist
        qWarning() << "The key " << keyname << "does not exist";
    }
    else {
        _menus.remove(keyname);
        this->removeAction(menu->menuAction());
        delete menu;
        return true;
    }
    return false;
}

bool WdMenu::removeWdMenu(QString const &keyname) {
    if (removeRootWdMenu(keyname)) {
        return true;
    }
    else {
        // we search deeper
        for (auto it = std::begin(_menus);
             it != std::end(_menus); ++it) {
            if (it.value()->removeRootWdMenu(keyname)) {
                return true;
            }
        }
    }
    return false;
}

WdAction *WdMenu::getRootWdAction(QString const &keyname) const {
    auto it = _actions.find(keyname);
    if (it != std::end(_actions)) {
        return it.value();
    }
    return nullptr;
}

WdMenu *WdMenu::getWdActionContainer(QString const &keyname) {
    if (getRootWdAction(keyname)) {
        return this;
    }
    else {
        // we search deeper
        for (auto it = std::begin(_menus);
             it != std::end(_menus); ++it) {
            WdMenu *menu = it.value()->getWdActionContainer(keyname);
            if (menu) {
                return menu;
            }
        }
    }
    return nullptr;
}

WdAction *WdMenu::getWdAction(QString const &keyname) const {
    WdAction *action = getRootWdAction(keyname);
    if (action) {
        return action;
    }
    else {
        // we search deeper
        for (auto it = std::begin(_menus);
             it != std::end(_menus); ++it) {
            action = it.value()->getWdAction(keyname);
            if (action) {
                return action;
            }
        }
    }
    return nullptr;
}

WdMenu *WdMenu::getRootWdMenu(QString const &keyname) const {
    auto it = _menus.find(keyname);
    if (it != std::end(_menus)) {
        return it.value();
    }
    return nullptr;
}

WdMenu *WdMenu::getWdMenuContainer(QString const &keyname) {
    if (getRootWdMenu(keyname)) {
        return this;
    }
    else {
        // we search deeper
        for (auto it = std::begin(_menus);
             it != std::end(_menus); ++it) {
            WdMenu *menu = it.value()->getRootWdMenu(keyname);
            if (menu) {
                return it.value();
            }
        }
    }
    return nullptr;
}

WdMenu *WdMenu::getWdMenu(QString const &keyname) const {
    WdMenu *menu = getRootWdMenu(keyname);
    if (menu) {
        return menu;
    }
    else {
        // we search deeper
        for (auto it = std::begin(_menus);
             it != std::end(_menus); ++it) {
            menu = it.value()->getWdMenu(keyname);
            if (menu) {
                return menu;
            }
        }
    }
    return nullptr;
}

bool WdMenu::keyExists(QString const &keyname) const {
    return getWdAction(keyname) || getWdMenu(keyname) || _key == keyname;
}

QString const &WdMenu::getKey() const {
    return _key;
}
