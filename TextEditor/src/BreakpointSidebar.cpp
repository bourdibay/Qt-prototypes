
#include <QDebug>

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTextBlock>

#include "TextEditor/BreakpointSidebar.h"
#include "TextEditor/TextEditor.h"

BreakpointSidebar::BreakpointSidebar(TextEditor *editor, QWidget *parent)
    : EditorSidebar(editor, parent),
    _borderSize(1),
    _breakpoint()
{
    const QFontMetrics metrics = fontMetrics();
    const int lineSpacing = metrics.lineSpacing();
    _breakpoint = QRect(_borderSize, _borderSize, 
        lineSpacing - 2 * _borderSize, 
        lineSpacing - 2 * _borderSize);

    connect(_editor, SIGNAL(breakpointPut(const int)), this,
        SLOT(breakpointChanged(const int)));
    connect(_editor, SIGNAL(breakpointCleared(const int)), this,
        SLOT(breakpointChanged(const int)));
}

void BreakpointSidebar::breakpointChanged(const int line)
{
    update();
}

void BreakpointSidebar::mousePressEvent(QMouseEvent *ev)
{
    const int line = posToLineNumber(ev->pos().y());
    if (_editor->hasBreakpoint(line)) {
        _editor->clearBreakpoint(line);
    } else {
        _editor->putBreakpoint(line);
    }
}

int BreakpointSidebar::idealWidth() const
{
    return _breakpoint.width() + 2 * _borderSize;
}

void BreakpointSidebar::paintEvent(QPaintEvent *ev)
{
    EditorSidebar::paintEvent(ev);

    // begin painting
    QPainter painter(this);

    // fill the background
    painter.fillRect(ev->rect(), _marginColor);

    // get the editor's painting area
    const QRect editorRect = _editor->contentsRect();

    // enable clipping to match the vertical painting area of the editor
    painter.setClipRect(QRect(0, editorRect.top(), width(), editorRect.bottom()), Qt::ReplaceClip);
    painter.setClipping(true);

    painter.setBrush(Qt::red);
    QTextBlock block = _editor->firstVisibleBlock();
    int top = static_cast<int>(_editor->blockBoundingGeometry(block).translated(_editor->contentOffset()).top());
    int bottom = top + static_cast<int>(_editor->blockBoundingRect(block).height());
    while (block.isValid() && top <= ev->rect().bottom()) {
        if (block.isVisible() && bottom >= ev->rect().top()) {
            // get the block data and check for a breakpoint
            if (_editor->hasBreakpoint(block))
            {
                // FIXME: Hughly breakpoing design...
                painter.drawRect(_breakpoint.translated(0, top));
            }
        }
        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(_editor->blockBoundingRect(block).height());
    }
}
