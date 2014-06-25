//
//   WdAction.h
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:43:09
//
//
//

#ifndef __WD_ACTION_H__
#define __WD_ACTION_H__

// Qt classes
#include <QAction>

// Qt classes
class QString;
class QObject;
class QIcon;

/**
* \class WdAction
* \brief Improvement of QAction for conveniance.
*/
class WdAction : public QAction {
public:
    /**
    * \brief Constructor.
    * \param action The action that will be copied.
    * \param keyname The keyname of the action.
    */
    explicit WdAction(QAction *action, QString const &keyname);
    /**
    * \brief Constructor.
    * \param keyname The keyname.
    * \param parent The parent.
    */
    explicit WdAction(QString const &keyname, QObject *parent);
    /**
    * \brief Constructor.
    * \param keyname The keyname.
    * \param text The text displayed.
    * \param parent The parent.
    */
    explicit WdAction(QString const &keyname, QString const &text,
                      QObject *parent);
    /**
    * \brief Constructor.
    * \param keyname The keyname.
    * \param icon The icon.
    * \param text The text displayed.
    * \param parent The parent.
    */
    explicit WdAction(QString const &keyname, QIcon const &icon,
                      QString const &text, QObject *parent);

    /**
    * \brief Get the keyname.
    * \return Get the keyname of the wdAction.
    */
    QString const &getKey() const;

private:
    Q_DISABLE_COPY(WdAction);

    const QString _key; /**< The keyname. */
};

#endif
