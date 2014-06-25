
#ifndef __BREAKPOINT_SIDEBAR_H__
#define __BREAKPOINT_SIDEBAR_H__

#include "TextEditor/EditorSidebar.h"

#include <QRect>

class TextEditor;

class QWidget;
class QPaintEvent;
class QMouseEvent;

class BreakpointSidebar : public EditorSidebar {
    Q_OBJECT

public:
    BreakpointSidebar(TextEditor *editor, QWidget *parent = nullptr);
    virtual ~BreakpointSidebar() {}

protected slots:
    void breakpointChanged(const int line);

protected:
    virtual void paintEvent(QPaintEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual int idealWidth() const;

protected:
    const int _borderSize;
    QRect _breakpoint;
};

#endif
