//
//   WdMenu.h
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:43:16
//
//
//

#ifndef __WD_MENU_H__
#define __WD_MENU_H__

// Qt classes
#include <QMenu>

// Qt classes
class QString;
class QWidget;
class QIcon;
class QAction;

// public graphical classes
class WdAction;

/**
* \class WdMenu
* \brief Improvement of QMenu for conveniance.
* Used by WdMenuBar.
*/
class WdMenu : public QMenu {
public:
    /**
    * \brief Constructor.
    * \param keyname The keyname of the menu.
    * \param parent THe parent.
    */
    WdMenu(QString const &keyname, QWidget *parent = 0);
    /**
    * \brief Constructor.
    * \param keyname The keyname of the menu.
    * \param title The text displayed.
    * \param parent The parent.
    */
    WdMenu(QString const &keyname, QString const &title, QWidget *parent = 0);

    virtual ~WdMenu();

    /**
    * \brief Add a wdAction to the menu.
    * \param action The action to add.
    */
    void addWdAction(WdAction *action);
    /**
    * \brief Add a wdAction tp the menu.
    * \param keyname The keyname.
    * \param text The text displayed.
    * \return The wdAction created and added.
    */
    WdAction *addWdAction(const QString &keyname, QString const &text);
    /**
    * \brief Add a wdAction tp the menu.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    * \return The wdAction created and added.
    */
    WdAction *addWdAction(QString const &keyname, QIcon const &icon,
                          QString const &text);
    /**
    * \brief Add a wdAction tp the menu.
    * \param keyname The keyname.
    * \param text The text displayed.
    * \param receiver The receiver of the signal clicked().
    * \param slotName The slot name called.
    * \param shortcut The shortcut that can trigger the signal.
    * \return The wdAction created and added.
    */
    WdAction *addWdAction(QString const &keyname, QString const &text,
                          QObject const *receiver, const char *slotName,
                          const QKeySequence &shortcut = 0);
    /**
    * \brief Add a wdAction tp the menu.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    * \param receiver The receiver of the signal clicked().
    * \param slotName The slot name called.
    * \param shortcut The shortcut that can trigger the signal.
    * \return The wdAction created and added.
    */
    WdAction *addWdAction(QString const &keyname, QIcon const &icon,
                          QString const &text, QObject const *receiver,
                          const char *slotName,
                          const QKeySequence &shortcut = 0);
    /**
    * \brief Add a wdMenu to the menu.
    * \param menu The menu to add.
    */
    void addWdMenu(WdMenu *menu);
    /**
    * \brief Add a wdMenu to the menu.
    * \param keyname The keyname.
    * \param title The text displayed.
    * \return The menu created and added.
    */
    WdMenu *addWdMenu(QString const &keyname, QString const &title);
    /**
    * \brief Add a wdMenu to the menu.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param title The text displayed.
    * \return The menu created and added.
    */
    WdMenu *addWdMenu(QString const &keyname, QIcon const &icon,
                      QString const &title);
    /**
    * \brief Add a wdAction to the menu followed by a separator.
    * \param keyname The keyname.
    * \param text The text displayed.
    * \return The action created and added.
    */
    WdAction *addWdSection(QString const &keyname, QString const &text);
    /**
    * \brief Add a wdAction to the menu followed by a separator.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    * \return The action created and added.
    */
    WdAction *addWdSection(QString const &keyname, QIcon const &icon,
                           QString const &text);
    /**
    * \brief Insert a wdAction to the menu followed by a separator.
    * \param before The previous action where to insert.
    * \param keyname The keyname.
    * \param text The text displayed.
    * \return The action created and added.
    */
    WdAction *insertWdSection(QAction *before, QString const &keyname,
                              QString const &text);
    /**
    * \brief Insert a wdAction to the menu followed by a separator.
    * \param before The previous action where to insert.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    * \return The action created and added.
    */
    WdAction *insertWdSection(QAction *before, QString const &keyname,
                              QIcon const &icon, QString const &text);
    /**
    * \brief Insert a wdMenu to the menu.
    * \param before The previous action in the menu where to insert.
    * \param keyname The keyname.
    * \param text The text displayed.
    * \return The menu created and added.
    */
    WdMenu *insertWdMenu(QAction *before, QString const &keyname,
                         QString const &text);
    /**
    * \brief Insert a wdMenu to the menu.
    * \param before The previous action in the menu where to insert.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    * \return The menu created and added.
    */
    WdMenu *insertWdMenu(QAction *before, QString const &keyname,
                         QIcon const &icon, QString const &text);
    /**
    * \brief Insert a wdMenu to the menu.
    * \param before The previous action in the menu where to insert.
    * \param menu The menu to insert.
    * \return True if we inserted it.
    */
    bool insertWdMenu(QAction *before, WdMenu *menu);
    /**
    * \brief Insert a wdAction to the menu.
    * \param before The previous action in the menu where to insert.
    * \param keyname The keyname.
    * \param text The text displayed.
    * \return The action created and added.
    */
    WdAction *insertWdAction(QAction *before, QString const &keyname,
                             QString const &text);
    /**
    * \brief Insert a wdAction to the menu.
    * \param before The previous action in the menu where to insert.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    * \return The action created and added.
    */
    WdAction *insertWdAction(QAction *before, QString const &keyname,
                             QIcon const &icon, QString const &text);
    /**
    * \brief Insert a wdAction to the menu.
    * \param before The previous action in the menu where to insert.
    * \param action The action to insert.
    * \return True if the action is inserted.
    */
    bool insertWdAction(QAction *before, WdAction *action);

    /**
    * \brief Remove the wdAction.
    * \param keyname The keyname.
    */
    void removeWdAction(QString const &keyname);
    /**
    * \brief Remove the wdMenu.
    * \param keyname The keyname.
    * \return True if we removed it.
    */
    bool removeWdMenu(QString const &keyname);
    /**
    * \brief Remove the wdMenu only at this root.
    * \param keyname The keyname.
    * \return True if we removed it.
    */
    bool removeRootWdMenu(QString const &keyname);

    /**
    * \brief Get the wdAction.
    * \param keyname The keyname.
    * \return The action with the corresponding keyname.
    */
    WdAction *getWdAction(QString const &keyname) const;
    /**
    * \brief Get the wdAction only in the root menu.
    * \param keyname The keyname.
    * \return The action with the corresponding keyname.
    */
    WdAction *getRootWdAction(QString const &keyname) const;
    /**
    * \brief Get the wdMenu that contains the wdAction with the keyname.
    * \param keyname THe keyname.
    * \return The menu that contains the wdAction.
    */
    WdMenu *getWdActionContainer(QString const &keyname);

    /**
    * Get the wdMenu.
    * \param keyname The keyname.
    * \return The menu with the corresponding keyname.
    */
    WdMenu *getWdMenu(QString const &keyname) const;
    /**
    * Get the wdMenu that contains the menu with the keyname.
    * \param keyname The keyname.
    * \return The menu that contains the menu with the corresponding keyname.
    */
    WdMenu *getWdMenuContainer(QString const &keyname);
    /**
    * Get the wdMenu with the keyname only in the root menu.
    * \param keyname The keyname.
    * \return The menu with the corresponding keyname.
    */
    WdMenu *getRootWdMenu(QString const &keyname) const;

    /**
    * \brief Tell if the keyname exists.
    * \param keyname The keyname.
    * \return True if the kename exists.
    */
    bool keyExists(QString const &keyname) const;
    /**
    * \brief Get the keyname of the menu.
    * \return The keyname of the menu.
    */
    QString const &getKey() const;

private:
    Q_DISABLE_COPY(WdMenu);

    const QString _key;                 /**< The keyname of the menu. */
    QMap<QString, WdAction *> _actions; /**< The list of actions in the menu. */
    QMap<QString, WdMenu *> _menus;     /**< The tree of menus. */
};

#endif
