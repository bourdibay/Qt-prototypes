
#ifndef __EDITOR_SIDEBAR_H__
#define __EDITOR_SIDEBAR_H__

#include <QWidget>
#include <QMouseEvent>

class TextEditor;
class QPaintEvent;
class QSize;
class QColor;
class QTextCursor;

class EditorSidebar : public QWidget {
    Q_OBJECT
public:
    EditorSidebar(TextEditor *editor, QWidget *parent = nullptr);
    virtual ~EditorSidebar() {}

    virtual QSize sizeHint() const;
    void setMarginColor(QColor const &color);

public slots:
    virtual void scroll(const int verticalScroll);

protected:
    virtual void paintEvent(QPaintEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev) {
        QWidget::mousePressEvent(ev);
    }

    virtual int idealWidth() const = 0;
    // convert a mouse position to a line number
    int posToLineNumber(const int y);

protected:
    TextEditor *_editor;
    QSize _currentSizeHint;
    int _verticalScroll;
    QColor _marginColor;
};

#endif
