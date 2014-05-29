
#include <QDebug>

#include <QWidget>
#include <QKeyEvent>

#include "TextEditor/TextEditor.h"
#include "TextEditor/WdUserData.h"

TextEditor::TextEditor(QWidget *parent)
    : QPlainTextEdit(parent),
    _callTipAutoCompletion(new CallTip(this))
{
    _callTipAutoCompletion->hide();
    installEventFilter(this);

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    highlightCurrentLine();
}

void TextEditor::putBreakpoint()
{
    putBreakpoint(textCursor().block());
}

void TextEditor::putBreakpoint(const int line)
{
    putBreakpoint(document()->findBlockByNumber(line));
}

void TextEditor::putBreakpoint(QTextBlock block)
{
    WdUserData *data = static_cast<WdUserData *>(block.userData());
    if (!data) {
        data = new WdUserData("breakpoint");
        block.setUserData(data);
    } else {
        data->properties.insert("breakpoint", QVariant());
    }
    emit breakpointPut(block.blockNumber());
}

bool TextEditor::hasBreakpoint(const int line) const
{
    return hasBreakpoint(document()->findBlockByNumber(line));
}

bool TextEditor::hasBreakpoint(const QTextBlock block) const
{
    const WdUserData *data = static_cast<WdUserData *>(block.userData());
    return data && data->properties.contains("breakpoint");
}

void TextEditor::clearBreakpoint()
{
    clearBreakpoint(textCursor().block());
}

void TextEditor::clearBreakpoint(const int line)
{
    clearBreakpoint(document()->findBlockByNumber(line));
}

void TextEditor::clearBreakpoint(QTextBlock block)
{
    WdUserData *data = static_cast<WdUserData *>(block.userData());
    if (data) {
        data->properties.remove("breakpoint");
        emit breakpointCleared(block.blockNumber());
    }
}

void TextEditor::setCurrentDocument(QTextDocument *doc)
{
    Q_ASSERT(doc != nullptr);
    QTextDocument *currentDocument = document();
    if (currentDocument) {
        QObject::disconnect(currentDocument);
    }
    QObject::connect(doc, SIGNAL(contentsChange(int, int, int)),
        this, SLOT(slot_textChanged(int, int, int)));
    setDocument(doc);
}

void TextEditor::slot_textChanged(const int position, const int charsRemoved, const int charsAdded)
{
    // If the changes are not made by the formatting process.
    // Indeed the setFormat() performs changes in the document,
    // but they are 'hidden', cosmetic only.
    if (_formatting == false) {
        if (charsRemoved > 0) {
            emit textRemoved(position, charsRemoved);
        } 
        if (charsAdded > 0) {
            const QString text = getText(position, charsAdded);
            emit textAdded(position, text);
        }
    }
}

void TextEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        const QColor lineColor = QColor(Qt::yellow).lighter(160);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

QString TextEditor::getText(const int position, const int length) const
{
    QTextCursor cursor = QTextCursor(document());
    cursor.setPosition(position);
    cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor, length);
    QString text = cursor.selectedText();
    return textBlockToText(text);
}

int TextEditor::positionFromLine(const int line, const int index) const
{
    Q_ASSERT(line >= 0);
    QTextBlock block = document()->findBlockByNumber(line);
    QTextCursor cursor(block);
    cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::MoveAnchor, index);
    return cursor.position();
}

void TextEditor::positionToLine(const int position, int *line, int *index)
{
    const QTextBlock block = document()->findBlock(position);
    *line = block.blockNumber();
    *index = position - block.position();
}

void TextEditor::setFormat(const int position, const int length, QTextCharFormat const &format)
{
    _formatting = true;
    QTextCursor prevCursor = textCursor();
    QTextCursor cursor = selectTextCursor(position, length);
    setTextCursor(cursor);
    setCurrentCharFormat(format);
    setTextCursor(prevCursor);
    _formatting = false;
}

QTextCharFormat TextEditor::getFormat(const int position, const int length) const
{
    QTextCursor cursor = selectTextCursor(position, length);
    return cursor.charFormat();
}

void TextEditor::showCallTipAutoCompletion()
{
    const QRect rect = cursorRect();
    qDebug() << "X=" << rect.x() << " y=" << rect.y();
    QPoint p(rect.x(), rect.y());

    const int pos = getCurrentPosition();
    int line, index;
    positionToLine(pos, &line, &index);

    const int lineHeight = this->lineHeight(line);
    const int callTipHeight = _callTipAutoCompletion->height();

    // if the calltip cannot be displayed above the line,
    // we display it under, no matter if it fits well or not.
    if (p.y() - callTipHeight < 0) {
        p.setY(p.y() + lineHeight); // we display it under the line
    }
    else {
        // we have room to display above the line if we cannot under the line
        if (p.y() + lineHeight + callTipHeight > this->height()) {
            // if it is too down, we display it above the line
            p.setY(p.y() - callTipHeight);
        } else {
            // we display under the line
            p.setY(p.y() + lineHeight);
        }
    }
    qDebug() << "line h=" << lineHeight;
    _callTipAutoCompletion->move(p.x(), p.y());
    _callTipAutoCompletion->show();
}



bool TextEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == this && !_callTipAutoCompletion->isHidden()) {
        if (ev->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
            const int key = keyEvent->key();
            switch (key) {
            case Qt::Key_Down:
            case Qt::Key_Up:
                if (keyEvent->modifiers() != Qt::NoModifier) {
                    _callTipAutoCompletion->hide();
                }
                else {
                    _callTipAutoCompletion->sendEvents(keyEvent);
                    if (keyEvent->isAccepted()) {
                        return true;
                    }
                }
                break;
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if (keyEvent->modifiers() != Qt::NoModifier) {
                    _callTipAutoCompletion->hide();
                }
                else {
                    _callTipAutoCompletion->sendEvents(keyEvent);
                    if (keyEvent->isAccepted()) {
                        qDebug() << "accepted";
                        return true;
                    }
                    qDebug() << "not accepted";
                }
                break;
            case Qt::Key_Escape:
                _callTipAutoCompletion->hide();
                break;
            }
        }
    }
    return QWidget::eventFilter(obj, ev);
}


void TextEditor::focusInEvent(QFocusEvent *ev)
{
    QPlainTextEdit::focusInEvent(ev);
    emit hasFocus();
}

void TextEditor::focusOutEvent(QFocusEvent *ev)
{
    _callTipAutoCompletion->hide();
    QPlainTextEdit::focusOutEvent(ev);
    emit losesFocus();
}
