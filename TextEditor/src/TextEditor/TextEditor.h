
#ifndef __TEXT_EDITOR_H__
#define __TEXT_EDITOR_H__

#include <QPlainTextEdit>
#include <QList>
#include <QTextLayout>
#include <QTextBlock>

#include "CallTip.h"

class QWidget;
class QKeyEvent;

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEditor(QWidget *parent = nullptr);
    virtual ~TextEditor() {}

    bool hasBreakpoint(const QTextBlock block) const;
    bool hasBreakpoint(const int line) const;
    void putBreakpoint();
    void putBreakpoint(const int line);
    void putBreakpoint(QTextBlock block);
    void clearBreakpoint();
    void clearBreakpoint(const int line);
    void clearBreakpoint(QTextBlock block);

    void setCurrentDocument(QTextDocument *document);

    QString getText(const int pos, const int length) const;
    // maybe a better implementation ? with move cursor ?
    int getLengthDocument() const { return toPlainText().length(); }
    QString getSelectedText() const { 
        QString text = textCursor().selectedText();
        return textBlockToText(text);
    }

    void selectText(const int position, const int length) {
        QTextCursor cursor = selectTextCursor(position, length);
        setTextCursor(cursor);
    }

    int getCurrentPosition() const { return textCursor().position(); }

    void deleteText(const int position, const int length) {
        QTextCursor cursor = selectTextCursor(position, length);
        cursor.removeSelectedText();
    }

    void insertText(const int position, QString const &text) {
        replaceText(position, 0, text);
    }

    void replaceText(const int position, const int length, QString const &text) {
        QTextCursor prevCursor = textCursor();
        QTextCursor cursor = selectTextCursor(position, length);
        cursor.insertText(text);
        setTextCursor(prevCursor);
    }

    int positionFromLine(const int line, const int index) const;
    void positionToLine(const int position, int *line, int *index);
    void setFormat(const int position, const int length, QTextCharFormat const &format);
    QTextCharFormat getFormat(const int position, const int length) const;

    int lineHeight(const int line) const {
        QTextBlock block = document()->findBlockByNumber(line);
        return static_cast<int>(blockBoundingRect(block).height());
    }


    void showCallTipAutoCompletion();
    void hideCallTipAutoCompletion() { _callTipAutoCompletion->hide(); }
    void setAutoCompletionData(std::vector<CompletionData> const &content) {
        _callTipAutoCompletion->setContent(content);
    }

    bool eventFilter(QObject *obj, QEvent *ev);


    private slots:
        void highlightCurrentLine();
        void slot_textChanged(const int position, const int charsRemoved, const int charsAdded);

signals:
        void textAdded(const int position, const QString &textAdded);
        void textRemoved(const int position, const int nbCharsRemoved);

        void breakpointPut(const int line);
        void breakpointCleared(const int line);

        void hasFocus();
        void losesFocus();

protected:
    virtual void focusInEvent(QFocusEvent *ev);
    virtual void focusOutEvent(QFocusEvent *ev);

private:
    QTextCursor selectTextCursor(const int position, const int length) const
    {
        QTextCursor cursor = QTextCursor(document());
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, position);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
        return cursor;
    }
    inline QString textBlockToText(QString &text) const { 
        // replace the unicode U+2029 by a return line.
        // This is useful when we have several blocks concatenated within the text.
        return text.replace(QString::fromWCharArray(L"\u2029"), "\n");
        //TODO: manage different end-of-line char according to the platform (unix, osx, window)
    }

    CallTip *_callTipAutoCompletion;

    bool _formatting; // used to ignore changes made during the formatting.

    friend class EditorSidebar;
    friend class LineNumberSidebar;
    friend class BreakpointSidebar;
};

#endif
