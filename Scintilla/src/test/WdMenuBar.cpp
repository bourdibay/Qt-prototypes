//
//   WdMenuBar.cpp
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:46:44
//
//
//

// for qwarning
#include <QDebug>

// Qt classes
#include <QString>
#include <QIcon>
#include <QStringList>
#include <QAction>
#include <QEvent>

// public graphical classes
#include "WdMenuBar.h"
#include "WdMenu.h"
#include "WdAction.h"

WdMenuBar::WdMenuBar(QWidget *parent) : QMenuBar(parent), _menus() {
}

WdMenuBar::~WdMenuBar() {
    QList<QString> keys = _menus.keys();
    for (auto k : keys) {
        removeWdMenu(k);
    }
    _menus.clear();
}

///////////////////////////////////////////////////////////////////////////
///////////// MENU MANAGEMENT /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/////////////////////// BEGIN GETTERS MENU //////////////////////////
WdMenu *WdMenuBar::getWdMenuContainer(QString const &keyname) const {
    if (getRootWdMenu(keyname)) {
        return nullptr;
    } else {
        // we search deeper
        for (auto it = std::begin(_menus); it != std::end(_menus); ++it) {
            WdMenu *menu = it.value()->getWdMenuContainer(keyname);
            if (menu) {
                return menu;
            }
        }
    }
    return nullptr;
}

WdMenu *WdMenuBar::getWdMenu(QString const &keyname, WdMenu *rootMenu) const {
    if (!rootMenu) {
        qWarning() << "The menu rootMenu is null";
        return nullptr;
    }
    return rootMenu->getWdMenu(keyname);
}

WdMenu *WdMenuBar::getWdMenu(QString const &keyname) const {
    WdMenu *menu = getRootWdMenu(keyname);
    if (menu) {
        return menu;
    } else {
        // we search deeper
        for (auto it = std::begin(_menus); it != std::end(_menus); ++it) {
            menu = getWdMenu(keyname, it.value());
            if (menu) {
                return menu;
            }
        }
    }
    return nullptr;
}

WdMenu *WdMenuBar::getRootWdMenu(QString const &keyname) const {
    auto it = _menus.find(keyname);
    if (it != std::end(_menus)) {
        return it.value();
    }
    return nullptr;
}

WdMenu *WdMenuBar::getRootWdMenu(QString const &keyname,
                                 WdMenu *rootMenu) const {
    if (!rootMenu) {
        qWarning() << "The menu rootMenu is null";
        return nullptr;
    }
    return rootMenu->getRootWdMenu(keyname);
}

WdMenu *WdMenuBar::getWdMenu(QStringList const &menuPath) const {
    WdMenu *currentMenu = nullptr;
    for (auto keyPath : menuPath) {
        // the first time, we fetch in the root.
        WdMenu *menu = nullptr;
        if (!currentMenu) {
            menu = getRootWdMenu(keyPath);
        } else {
            menu = currentMenu->getRootWdMenu(keyPath);
        }
        if (menu) {
            currentMenu = menu;
        } else {
            if (keyExists(keyPath)) {
                qWarning() << "The key" << keyPath << "already exists in "
                                                      "another level or is "
                                                      "attributed to an action";
                return nullptr;
            } else {
                qWarning() << "The key" << keyPath << "does not exist";
                return nullptr;
            }
        }
    }
    return currentMenu;
}
/////////////////////// END GETTERS MENU //////////////////////////

/////////////////////// BEGIN ADD MENU ////////////////////////////
void WdMenuBar::addWdMenu(WdMenu *menu) {
    const QString keyname = menu->getKey();
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
    } else {
        _menus[keyname] = menu;
        this->addMenu(menu);
    }
}

WdMenu *WdMenuBar::addWdMenu(QString const &keyname, QString const &text) {
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
    } else {
        WdMenu *menu = new WdMenu(keyname, text);
        _menus[keyname] = menu;
        this->addMenu(menu);
        return menu;
    }
    return nullptr;
}

WdMenu *WdMenuBar::addWdMenu(QString const &keyname, QIcon const &icon,
                             QString const &text) {
    auto menu = this->addWdMenu(keyname, text);
    if (menu) {
        menu->setIcon(icon);
    }
    return menu;
}

WdMenu *WdMenuBar::addWdMenu(QString const &keyname,
                             QStringList const &menuPath, QString const &text) {
    // first check the keyname everywhere
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    }
    WdMenu *currentMenu = getWdMenu(menuPath);
    if (currentMenu) {
        return currentMenu->addWdMenu(keyname, text);
    }
    return nullptr;
}

WdMenu *WdMenuBar::addWdMenu(QString const &keyname,
                             QStringList const &menuPath, QIcon const &icon,
                             QString const &text) {
    WdMenu *menu = addWdMenu(keyname, menuPath, text);
    if (menu) {
        menu->setIcon(icon);
    }
    return menu;
}
/////////////////////// END ADD MENU ////////////////////////////////

/////////////////////// BEGIN REMOVE MENU //////////////////////////
bool WdMenuBar::removeWdMenu(QString const &keyname) {
    WdMenu *menu = getWdMenuContainer(keyname);
    if (!menu) {
        menu = getRootWdMenu(keyname);
        if (!menu) {
            qWarning() << "The key" << keyname << "does not exist for a menu";
            return false;
        } else {
            _menus.remove(keyname);
            this->removeAction(menu->menuAction());
            delete menu;
            return true;
        }
    } else {
        menu->removeWdMenu(keyname);
        return true;
    }
    return false;
}
/////////////////////// END REMOVE MENU //////////////////////////

/////////////////////// BEGIN INSERT MENU //////////////////////////
bool WdMenuBar::insertWdMenu(QAction *before, WdMenu *menu) {
    if (keyExists(menu->getKey())) {
        qWarning() << "The key" << menu->getKey() << "already exists";
        return false;
    } else {
        _menus[menu->getKey()] = menu;
        insertMenu(before, menu);
        return true;
    }
    return false;
}

WdMenu *WdMenuBar::insertWdMenu(QAction *before, QString const &keyname,
                                QString const &text) {
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    } else {
        WdMenu *menu = new WdMenu(keyname, text);
        _menus[keyname] = menu;
        insertWdMenu(before, menu);
    }
    return nullptr;
}

WdMenu *WdMenuBar::insertWdMenu(QAction *before, QString const &keyname,
                                QIcon const &icon, QString const &text) {
    WdMenu *menu = insertWdMenu(before, keyname, text);
    if (menu) {
        menu->setIcon(icon);
    }
    return menu;
}

WdMenu *WdMenuBar::insertWdMenu(WdMenu *menu, QAction *before,
                                QString const &keyname, QString const &text) {
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    } else {
        WdMenu *newMenu = new WdMenu(keyname, text);
        menu->insertWdMenu(before, newMenu);
        return newMenu;
    }
    return nullptr;
}

WdMenu *WdMenuBar::insertWdMenu(WdMenu *menu, QAction *before,
                                QString const &keyname, QIcon const &icon,
                                QString const &text) {
    WdMenu *newMenu = insertWdMenu(menu, before, keyname, text);
    if (newMenu) {
        newMenu->setIcon(icon);
    }
    return newMenu;
}

///////////////////////////////////////////////////////////////////////////
///////////// ACTION MANAGEMENT ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/////////////////////// BEGIN GETTERS ACTION /////////////////////////////
WdMenu *WdMenuBar::getWdActionContainer(QString const &keyname) const {
    for (auto it = std::begin(_menus); it != std::end(_menus); ++it) {
        WdMenu *menu = it.value()->getWdActionContainer(keyname);
        if (menu) {
            return menu;
        }
    }
    return nullptr;
}

WdAction *WdMenuBar::getWdAction(QString const &keyname,
                                 WdMenu *rootMenu) const {
    if (!rootMenu) {
        qWarning() << "The menu rootMenu is null";
        return nullptr;
    }
    return rootMenu->getWdAction(keyname);
}

WdAction *WdMenuBar::getWdAction(QString const &keyname) const {
    for (auto it = std::begin(_menus); it != std::end(_menus); ++it) {
        WdAction *action = getWdAction(keyname, it.value());
        if (action) {
            return action;
        }
    }
    return nullptr;
}

WdAction *WdMenuBar::getRootWdAction(QString const &keyname,
                                     WdMenu *rootMenu) const {
    if (!rootMenu) {
        qWarning() << "The menu rootMenu is null";
        return nullptr;
    }
    return rootMenu->getRootWdAction(keyname);
}
/////////////////////// END GETTERS //////////////////////////

////////////////////// BEGIN ADD /////////////////////////////
WdAction *WdMenuBar::addWdAction(QString const &keyname,
                                 QStringList const &menuPath,
                                 QString const &text) {
    // first check the keyname everywhere
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    }
    WdMenu *currentMenu = getWdMenu(menuPath);
    if (currentMenu) {
        return currentMenu->addWdAction(keyname, text);
    }
    return nullptr;
}

WdAction *WdMenuBar::addWdAction(QString const &keyname,
                                 QStringList const &menuPath, QIcon const &icon,
                                 QString const &text) {
    WdAction *action = addWdAction(keyname, menuPath, text);
    if (action) {
        action->setIcon(icon);
    }
    return action;
}

WdAction *WdMenuBar::addWdAction(QString const &keyname,
                                 QStringList const &menuPath,
                                 QString const &text, QObject const *receiver,
                                 const char *slotName,
                                 const QKeySequence &shortcut) {
    // first check the keyname everywhere
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    }
    WdMenu *currentMenu = getWdMenu(menuPath);
    if (currentMenu) {
        return currentMenu->addWdAction(keyname, text, receiver, slotName,
                                        shortcut);
    }
    return nullptr;
}

WdAction *WdMenuBar::addWdAction(QString const &keyname,
                                 QStringList const &menuPath, QIcon const &icon,
                                 QString const &text, QObject const *receiver,
                                 const char *slotName,
                                 const QKeySequence &shortcut) {
    WdAction *action =
        addWdAction(keyname, menuPath, text, receiver, slotName, shortcut);
    if (action) {
        action->setIcon(icon);
    }
    return action;
}
////////////////////// END ADD //////////////////////////////////

////////////////////// BEGIN REMOVE /////////////////////////////
bool WdMenuBar::removeWdAction(QString const &keyname) {
    WdMenu *menu = getWdActionContainer(keyname);
    if (!menu) {
        return nullptr;
    } else {
        menu->removeWdAction(keyname);
        return true;
    }
    return false;
}
////////////////////// END REMOVE ///////////////////////////////

////////////////////// BEGIN INSERT /////////////////////////////
bool WdMenuBar::insertWdAction(WdMenu *menu, QAction *before,
                               WdAction *action) {
    if (keyExists(action->getKey())) {
        qWarning() << "The key" << action->getKey() << "already exists";
        return false;
    } else {
        menu->insertWdAction(before, action);
        return true;
    }
    return false;
}

WdAction *WdMenuBar::insertWdAction(WdMenu *menu, QAction *before,
                                    QString const &keyname,
                                    QString const &text) {
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    } else {
        return menu->insertWdAction(before, keyname, text);
    }
    return nullptr;
}

WdAction *WdMenuBar::insertWdAction(WdMenu *menu, QAction *before,
                                    QString const &keyname, QIcon const &icon,
                                    QString const &text) {
    if (keyExists(keyname)) {
        qWarning() << "The key" << keyname << "already exists";
        return nullptr;
    } else {
        return menu->insertWdAction(before, keyname, icon, text);
    }
    return nullptr;
}
////////////////////// END INSERT /////////////////////////////

bool WdMenuBar::keyExists(QString const &keyname) const {
    for (auto it = std::begin(_menus); it != std::end(_menus); ++it) {
        if (it.value()->keyExists(keyname)) {
            return true;
        }
    }
    return false;
}
