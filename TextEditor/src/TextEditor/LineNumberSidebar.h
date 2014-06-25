
#ifndef __LINE_NUMBER_SIDEBAR_H__
#define __LINE_NUMBER_SIDEBAR_H__

#include "TextEditor/EditorSidebar.h"

class QPaintEvent;
class QWidget;
class TextEditor;
class QColor;

class LineNumberSidebar : public EditorSidebar {
public:
    LineNumberSidebar(TextEditor *editor, QWidget *parent = nullptr);
    virtual ~LineNumberSidebar() {}

    void setTextColor(QColor const &color);

public slots:
    void showLineNumbers(const bool state);

protected:
    virtual void paintEvent(QPaintEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual int idealWidth() const;

private:
    QColor _textColor;
    const int _fontMetricsWidth; // font metric width cache
};

#endif
