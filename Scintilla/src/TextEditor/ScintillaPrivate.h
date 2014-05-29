//
//   ScintillaPrivate.h
//   Widev
//
//   Created by bourdi_b on 08 févr. 2014 19:57:12
//
//
//

#ifndef __SCINTILLA_PRIVATE_H__
#define __SCINTILLA_PRIVATE_H__

// basic text editor
#include <Qsci/qsciscintilla.h>

// Qt classes
class QMenu;
class QKeyEvent;

// widev graphic classes
class CallTip;

/**
* \class ScintillaPrivate
* \brief Used for the reimplementation of the menu context.
*/
class ScintillaPrivate : public QsciScintilla {
public:
    /**
    * \brief Constructor.
    * \param parent The parent.
    */
    ScintillaPrivate(QWidget *parent);
    virtual ~ScintillaPrivate() {}
    /**
    * \brief Get the contextual menu.
    * \return The contextual menu.
    */
    QMenu *getContextualMenu() const;

    bool eventFilter(QObject *obj, QEvent *ev);

    CallTip *getCallTip() const;

    void hideCallTip();
    void showCallTip(const int x, const int y);
    bool hasCallTipFocus() const;
    bool isCallTipDisplayed() const;

    bool event(QEvent *ev) override;

protected:
    void mousePressEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e) override;

private:
    Q_DISABLE_COPY(ScintillaPrivate);

    QMenu *_contextualMenu; /**< The contextual menu. */
    CallTip *_callTip;
};

#endif
