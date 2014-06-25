//
//   WdMenuBar.h
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:43:22
//
//
//

#ifndef __WD_MENU_BAR_H__
#define __WD_MENU_BAR_H__

// Qt classes
#include <QMenuBar>
#include <QMap>

// Qt classes
class QString;
class QStringList;
class QIcon;
class QAction;
class QEvent;

// public graphical classes
class WdMenu;
class WdAction;

/**
* \class WdMenuBar
* \brief The Widev version of the MenuBar, with more convenient
* functionnalities.
*/
class WdMenuBar : public QMenuBar {
public:
    virtual ~WdMenuBar();
    /**
    * \brief Constructor.
    * \param parent The parent.
    */
    WdMenuBar(QWidget *parent);

    ///////////// MENU MANAGEMENT ////////////

    // todo: get with stringlist
    /**
    * Get the wdMenu with the keyname. Search in all menus and submenus.
    * \param keyname The keyname of the menu.
    * \return The wdMenu.
    */
    WdMenu *getWdMenu(QString const &keyname) const;
    /**
    * Get the wdMenu that contains the menu with the keyname.
    * \param keyname The keyname of the menu.
    * \return The wdMenu that contains the menu with the keyname.
    */
    WdMenu *getWdMenuContainer(QString const &keyname) const;
    /**
    * Get the wdMenu with the keyname from the root menu.
    * \param keyname The keyname of the menu.
    * \param rootMenu The root menu from where to search.
    * \return The wdMenu.
    */
    WdMenu *getWdMenu(QString const &keyname, WdMenu *rootMenu) const;
    /**
    * Get the wdMenu with the keyname only in the rootmenu level.
    * \param keyname The keyname of the menu.
    * \return The wdMenu.
    */
    WdMenu *getRootWdMenu(QString const &keyname) const;
    /**
    * Get the wdMenu with the keyname only in the rootmenu level.
    * \param keyname The keyname of the menu.
    * \param rootMenu The root menu level.
    * \return The wdMenu.
    */
    WdMenu *getRootWdMenu(QString const &keyname, WdMenu *rootMenu) const;
    /**
    * Get the wdMenu with the keyname that satisfy the hierarchie menu.
    * \param menuPath The menu hierarchie.
    * \return The wdMenu.
    */
    WdMenu *getWdMenu(QStringList const &menuPath) const;

    // search in all actions in all menus
    /**
    * \brief Add a wdMenu to the root.
    * \param menu The menu to add.
    */
    void addWdMenu(WdMenu *menu);
    /**
    * \brief Add a wdMenu to the root.
    * \param keyname The keyname.
    * \param text The text displayed.
    * \return The menu created and added.
    */
    WdMenu *addWdMenu(QString const &keyname, QString const &text);
    /**
    * \brief Add a wdMenu to the root.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    * \return The menu created and added.
    */
    WdMenu *addWdMenu(QString const &keyname, QIcon const &icon,
                      QString const &text);
    /**
    * \brief Add a wdMenu to the root.
    * \param keyname The keyname.
    * \param menuPath The menu hierarchie where to add the menu.
    * \param text The text displayed.
    * \return The menu created and added.
    */
    WdMenu *addWdMenu(QString const &keyname, QStringList const &menuPath,
                      QString const &text);
    /**
    * \brief Add a wdMenu to the root.
    * \param keyname The keyname.
    * \param menuPath The menu hierarchie where to add the menu.
    * \param icon The icon.
    * \param text The text displayed.
    * \return The menu created and added.
    */
    WdMenu *addWdMenu(QString const &keyname, QStringList const &menuPath,
                      QIcon const &icon, QString const &text);

    /**
    * \brief Remove the menu with the keyname.
    * \param keyname The keyname.
    * \return True if we removed it.
    */
    bool removeWdMenu(QString const &keyname);

    /**
    * \brief Insert a wdMenu.
    * \param before The menu entry that will be before the new menu.
    * \param menu The menu to insert.
    * \return True if we inserted the menu.
    */
    bool insertWdMenu(QAction *before, WdMenu *menu);
    /**
    * \brief Insert a wdMenu.
    * \param before The menu entry that will be before the new menu.
    * \param keyname The keyname.
    * \param text The text of the menu.
    * \return The menu inserted.
    */
    WdMenu *insertWdMenu(QAction *before, QString const &keyname,
                         QString const &text);
    /**
    * \brief Insert a wdMenu.
    * \param before The menu entry that will be before the new menu.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text of the menu.
    * \return The menu inserted.
    */
    WdMenu *insertWdMenu(QAction *before, QString const &keyname,
                         QIcon const &icon, QString const &text);
    /**
    * \brief Insert a wdMenu.
    * \param menu The root menu where to insert the new menu.
    * \param before The menu entry that will be before the new menu.
    * \param keyname The keyname.
    * \param text The text of the menu.
    * \return The menu inserted.
    */
    WdMenu *insertWdMenu(WdMenu *menu, QAction *before, QString const &keyname,
                         QString const &text);
    /**
    * \brief Insert a wdMenu.
    * \param menu The root menu where to insert the new menu.
    * \param before The menu entry that will be before the new menu.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text of the menu.
    * \return The menu inserted.
    */
    WdMenu *insertWdMenu(WdMenu *menu, QAction *before, QString const &keyname,
                         QIcon const &icon, QString const &text);

    ///////////// ACTION MANAGEMENT ///////////////
    /**
    * \brief Get a wdAction from the keyname.
    * \param keyname The keyname.
    * \return The wdAction found with the keyname.
    */
    WdAction *getWdAction(QString const &keyname) const;
    /**
    * \brief Get a wdMenu that contains the wdAction with the keyname.
    * \param keyname The keyname.
    * \return The menu that contains the wdAction found with the keyname.
    */
    WdMenu *getWdActionContainer(QString const &keyname) const;
    /**
    * \brief Get the wdAction with the keyname from the rootMenu.
    * \param keyname The keyname.
    * \param rootMenu The root menu from where to search.
    * \return The wdAction.
    */
    WdAction *getWdAction(QString const &keyname, WdMenu *rootMenu) const;
    /**
    * \brief Get the wdAction with the keyname in the rootMenu only.
    * \param keyname The keyname.
    * \param rootMenu The root menu from where to search.
    * \return The wdAction.
    */
    WdAction *getRootWdAction(QString const &keyname, WdMenu *rootMenu) const;

    /**
    * \brief Add a wdAction to the hierarchie.
    * \param keyname The keyname.
    * \param menuPath The path where adding the action.
    * \param text The text displayed.
    */
    WdAction *addWdAction(QString const &keyname, QStringList const &menuPath,
                          QString const &text);
    /**
    * \brief Add a wdAction to the hierarchie.
    * \param keyname The keyname.
    * \param menuPath The path where adding the action.
    * \param icon The icon.
    * \param text The text displayed.
    */
    WdAction *addWdAction(QString const &keyname, QStringList const &menuPath,
                          QIcon const &icon, QString const &text);
    /**
    * \brief Add a wdAction to the hierarchie.
    * \param keyname The keyname.
    * \param menuPath The path where adding the action.
    * \param text The text displayed.
    * \param receiver The receiver of the signal clicked().
    * \param slotName The name of the slot.
    * \param shortcut THe shortcut to trigger this signal.
    */
    WdAction *addWdAction(QString const &keyname, QStringList const &menuPath,
                          QString const &text, QObject const *receiver,
                          const char *slotName,
                          const QKeySequence &shortcut = 0);
    /**
    * \brief Add a wdAction to the hierarchie.
    * \param keyname The keyname.
    * \param menuPath The path where adding the action.
    * \param icon The icon.
    * \param text The text displayed.
    * \param receiver The receiver of the signal clicked().
    * \param slotName The name of the slot.
    * \param shortcut THe shortcut to trigger this signal.
    */
    WdAction *addWdAction(QString const &keyname, QStringList const &menuPath,
                          QIcon const &icon, QString const &text,
                          QObject const *receiver, const char *slotName,
                          const QKeySequence &shortcut = 0);

    /**
    * \brief Remove a wdAction.
    * \param keyname The keyname.
    * \return True if we removed it.
    */
    bool removeWdAction(QString const &keyname);

    /**
    * \brief Insert a wdAction.
    * \param menu The root menu.
    * \param before The action before the new one.
    * \param action The new action to insert.
    * \return True if we inserted it.
    */
    bool insertWdAction(WdMenu *menu, QAction *before, WdAction *action);
    /**
    * \brief Insert a wdAction.
    * \param menu The root menu.
    * \param before The action before the new one.
    * \param keyname The keyname.
    * \param text The text displayed.
    */
    WdAction *insertWdAction(WdMenu *menu, QAction *before,
                             QString const &keyname, QString const &text);
    /**
    * \brief Insert a wdAction.
    * \param menu The root menu.
    * \param before The action before the new one.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    */
    WdAction *insertWdAction(WdMenu *menu, QAction *before,
                             QString const &keyname, QIcon const &icon,
                             QString const &text);

    /**
    * \brief Tell if the keyname already exists somewhere in the hierarchie.
    * \param keyname The keyname.
    * \return True if it exists.
    */
    bool keyExists(QString const &keyname) const;

    // TODO: initializer list instead of qstringlist
private:
    Q_DISABLE_COPY(WdMenuBar);

    QMap<QString, WdMenu *> _menus; /**< The menus tree. */
};

#endif
