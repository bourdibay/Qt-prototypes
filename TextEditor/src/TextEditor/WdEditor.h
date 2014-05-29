
#ifndef __WD_EDITOR_H__
#define __WD_EDITOR_H__

#include <QWidget>
#include <QList>
#include <QString>
#include <QTextDocument>
#include <QHBoxLayout>

#include "TextEditor/TextEditor.h"
#include "TextEditor/LineNumberSidebar.h"
#include "TextEditor/BreakpointSidebar.h"

class WdEditor : public QWidget
{
    Q_OBJECT
public:
    WdEditor(QWidget *parent = nullptr);
    virtual ~WdEditor() {}

    /*
    virtual void save(QString const &name = QString()) {}
    virtual void copySelection() {}
    virtual void paste() const {}
    virtual void cutSelection() {}
    virtual bool canPaste() const {return true;}
    virtual bool canRedo() const {return true;}
    virtual bool canUndo() const {return true;}
    virtual bool canCopy() const {return true;}
    virtual bool canCut() const {return true;}
    virtual void selectAll() const {}
    virtual void unselect() const {}
    virtual void setZoom(const int zoom) const {}
    virtual void setZoomRange(const int range) const {}
    virtual int getZoom() const {return 0;}
    virtual bool hasDocumentFocus() const {return true;}        
    virtual bool isReadOnly() const {return false;}
    virtual void setReadOnly(const bool s) const {}
    */

    void setDocument(QTextDocument *document) { _textEditor->setCurrentDocument(document); }
    QTextDocument *getDocument() const { return _textEditor->document(); }
    QTextDocument *openFile(QString const &filename);

    int getLengthDocument() const  { return _textEditor->getLengthDocument(); }
    int getCurrentPosition() const { return _textEditor->getCurrentPosition(); }
    QString getText(const int position, const int length) const { return _textEditor->getText(position, length); }
    QString getSelectedText() const { return _textEditor->getSelectedText(); }
    int positionFromLine(const int line, const int index) const { return _textEditor->positionFromLine(line, index); }
    void positionToLine(const int pos, int *line, int *index) { _textEditor->positionToLine(pos, line, index); }

    void setColor(const int position, const int length, QColor const &color) {
        QTextCharFormat format = getFormat(position, length);
        format.setForeground(color);
        setFormat(position, length, format);
    }
    void setFormat(const int position, const int length, QTextCharFormat const &format) {
        _textEditor->setFormat(position, length, format);
    }

    QTextCharFormat getFormat(const int position, const int length) {
        return _textEditor->getFormat(position, length);
    }


    void selectText(const int position, const int length) { _textEditor->selectText(position, length); }
    void deleteText(const int position, const int length) { _textEditor->deleteText(position, length); }
    void insertText(const int position, QString const &text) { _textEditor->insertText(position, text); }
    void replaceText(const int position, const int length, QString const &text) { _textEditor->replaceText(position, length, text); }

    void showCallTipAutoCompletion() { _textEditor->showCallTipAutoCompletion(); }
    void hideCallTipAutoCompletion() { _textEditor->hideCallTipAutoCompletion(); }
    void setAutoCompletionData(std::vector<CompletionData> const &content) {
        _textEditor->setAutoCompletionData(content);
    }

signals:
    void cursorPositionChanged();
    void selectionChanged();
    void textAdded(const int position, const QString &textAdded);
    void textRemoved(const int position, const int nbCharsRemoved);
    void documentHasFocus();
    void documentLosesFocus();

private:
    QHBoxLayout *_layout;
    TextEditor *_textEditor;
    LineNumberSidebar *_lineNumberSidebar;
    BreakpointSidebar *_breakpointSidebar;
};

#endif
