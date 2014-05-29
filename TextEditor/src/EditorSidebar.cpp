
#include <QDebug>

#include <QSize>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QTextBlock>
#include <QScrollBar>
#include <QTextCursor>
#include <QColor>

#include "TextEditor/EditorSidebar.h"
#include "TextEditor/TextEditor.h"

EditorSidebar::EditorSidebar(TextEditor *editor, QWidget *parent)
    : QWidget(parent),
    _editor(editor),
    _currentSizeHint(0, 0),
    _verticalScroll(0),
    _marginColor(QColor(210, 210, 210))
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    QObject::connect(_editor->verticalScrollBar(), SIGNAL(valueChanged(int)), 
        this, SLOT(scroll(int)));
    QObject::connect(_editor, SIGNAL(cursorPositionChanged()),
        this, SLOT(update()));
    QObject::connect(_editor, SIGNAL(textChanged()),
        this, SLOT(update()));
}

void EditorSidebar::scroll(const int dy)
{
    _verticalScroll = dy;
    update();
}

QSize EditorSidebar::sizeHint() const
{
    return QSize(idealWidth(), 0);
}

void EditorSidebar::paintEvent(QPaintEvent *ev)
{
    const QSize newSizeHint = sizeHint();
    if (_currentSizeHint != newSizeHint) {
        updateGeometry();
        _currentSizeHint = newSizeHint;
    }
}

int EditorSidebar::posToLineNumber(const int y)
{
    QTextBlock block = _editor->firstVisibleBlock();
    int top = static_cast<int>(_editor->blockBoundingGeometry(block).translated(_editor->contentOffset()).top());
    int bottom = top + static_cast<int>(_editor->blockBoundingRect(block).height());
    while (block.isValid() && block.isVisible()) {
        if (y >= top && y <= bottom) {
            return block.blockNumber();
        }
        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(_editor->blockBoundingRect(block).height());
    }
    // not found
    return -1;
}

void EditorSidebar::setMarginColor(QColor const &color)
{
    _marginColor = color;
}
