//
//   ScintillaPrivate.cpp
//   Widev
//
//   Created by bourdi_b on 08 févr. 2014 19:58:49
//
//
//

#include <QMenu>
#include <QShortcut>
#include <QDebug>
#include <QKeyEvent>
#include <QListView>

#include <Qsci/qscicommand.h>

#include "TextEditor/ScintillaPrivate.h"
#include "TextEditor/CallTip.h"
#include "TextEditor/ScintillaCommandPrivate.h"

ScintillaPrivate::ScintillaPrivate(QWidget *parent)
    : QsciScintilla(parent),
      _contextualMenu(new QMenu(this)),
      _callTip(new CallTip(this)) {
    installEventFilter(this);
    ScintillaCommandPrivate(this);

}

bool ScintillaPrivate::event(QEvent *ev) {
    // To worksaround scintilla binding and override
    if (ev->type() == QEvent::ShortcutOverride) {
        return false;
    }
    return QsciScintilla::event(ev);
}

void ScintillaPrivate::contextMenuEvent(QContextMenuEvent *e) {
    if (_contextualMenu->actions().length() > 0) {
        _contextualMenu->popup(e->globalPos());
    }
}

QMenu *ScintillaPrivate::getContextualMenu() const {
    return _contextualMenu;
}

CallTip *ScintillaPrivate::getCallTip() const {
    return _callTip;
}

void ScintillaPrivate::hideCallTip() {
    if (isCallTipDisplayed()) {
        _callTip->hideComplete();
    }
}

void ScintillaPrivate::showCallTip(const int x, const int y) {
    _callTip->move(x, y);
    if (isCallTipDisplayed() == false) {
        _callTip->show();
    }
}

bool ScintillaPrivate::hasCallTipFocus() const {
    return isCallTipDisplayed() && _callTip->getCurrentIndex().isValid();
}

bool ScintillaPrivate::isCallTipDisplayed() const {
    return !_callTip->isHidden();
}

bool ScintillaPrivate::eventFilter(QObject *obj, QEvent *ev) {
    if (obj == this && isCallTipDisplayed()) {
        if (ev->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
            const int key = keyEvent->key();

            switch (key) {
            case Qt::Key_Down:
            case Qt::Key_Up:
                if (keyEvent->modifiers() != Qt::NoModifier) {
                    hideCallTip();
                }
                else {
                    _callTip->sendEvents(keyEvent);
                    if (keyEvent->isAccepted()) {
                        qDebug() << "return true";
                        return true;
                    }
                }
                break;
            case Qt::Key_Right:
            case Qt::Key_Left:
                if (hasCallTipFocus()) {
                    qDebug() << ";eft right ok";
                    hideCallTip();
                }
                break;
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if (keyEvent->modifiers() != Qt::NoModifier) {
                    hideCallTip();
                }
                else {
                    _callTip->sendEvents(keyEvent);
                    if (keyEvent->isAccepted()) {
                        qDebug() << "accepted";
                        return true;
                    }
                    qDebug() << "not accepted";
                }
                break;
            case Qt::Key_Escape:
                hideCallTip();
                break;
            }
        }
    }
    return QsciScintilla::eventFilter(obj, ev);
}

void ScintillaPrivate::mousePressEvent(QMouseEvent *e) {
    hideCallTip();
    QsciScintilla::mousePressEvent(e);
}
