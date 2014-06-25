
#include <QDebug>

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QTextBlock>

#include "TextEditor/TextEditor.h"
#include "TextEditor/LineNumberSidebar.h"

LineNumberSidebar::LineNumberSidebar(TextEditor *editor, QWidget *parent)
: EditorSidebar(editor, parent)
, _textColor(Qt::black)
, _fontMetricsWidth(fontMetrics().width(QLatin1Char('9'))) {
    showLineNumbers(true);
}

void LineNumberSidebar::showLineNumbers(const bool state) {
    setVisible(state);
}

void LineNumberSidebar::mousePressEvent(QMouseEvent *ev) {
    const int line = posToLineNumber(ev->pos().y());
    if (_editor->hasBreakpoint(line)) {
        _editor->clearBreakpoint(line);
    } else {
        _editor->putBreakpoint(line);
    }
}

int LineNumberSidebar::idealWidth() const {
    int lineNumber = _editor->document()->blockCount();
    // 4 space minimum required.
    if (lineNumber < 10000)
        return 3 + _fontMetricsWidth * 4;
    int result = 1;
    // from basic facebook algorithm
    // https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920
    for (;;) {
        // Skip ahead by 4 orders of magnitude
        lineNumber /= 10000;
        result += 4;
        if (lineNumber < 10)
            return 3 + _fontMetricsWidth * result;
        if (lineNumber < 100)
            return 3 + _fontMetricsWidth * (result + 1);
        if (lineNumber < 1000)
            return 3 + _fontMetricsWidth * (result + 2);
        if (lineNumber < 10000)
            return 3 + _fontMetricsWidth * (result + 3);
    }
    return result;
}

void LineNumberSidebar::paintEvent(QPaintEvent *ev) {
    EditorSidebar::paintEvent(ev);
    // begin painting
    QPainter painter(this);

    // fill the background
    painter.fillRect(ev->rect(), _marginColor);

    // get the editor's painting area
    const QRect editorRect = _editor->contentsRect();

    // enable clipping to match the vertical painting area of the editor
    painter.setClipRect(
        QRect(0, editorRect.top(), width(), editorRect.bottom()),
        Qt::ReplaceClip);
    painter.setClipping(true);

    QTextBlock block = _editor->firstVisibleBlock();
    int top = static_cast<int>(_editor->blockBoundingGeometry(block)
                                   .translated(_editor->contentOffset())
                                   .top());
    int bottom =
        top + static_cast<int>(_editor->blockBoundingRect(block).height());
    painter.setPen(_textColor);
    const int fontMetricsHeight = fontMetrics().height();
    const int width = this->width();
    while (block.isValid() && top <= ev->rect().bottom()) {
        if (block.isVisible() && bottom >= ev->rect().top()) {
            const QString number = QString::number(block.blockNumber() + 1);
            painter.drawText(0, top, width, fontMetricsHeight, Qt::AlignRight,
                             number);
        }
        block = block.next();
        top = bottom;
        bottom =
            top + static_cast<int>(_editor->blockBoundingRect(block).height());
    }
}

void LineNumberSidebar::setTextColor(QColor const &color) {
    _textColor = color;
}
