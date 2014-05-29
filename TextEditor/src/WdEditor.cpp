#include <QDebug>
#include <QHBoxLayout>
#include <QTextBlock>

#include "TextEditor/WdEditor.h"

WdEditor::WdEditor(QWidget *parent)
    : QWidget(parent),
    _layout(new QHBoxLayout()),
    _textEditor(new TextEditor(this)),
    _lineNumberSidebar(new LineNumberSidebar(_textEditor, this)),
    _breakpointSidebar(new BreakpointSidebar(_textEditor, this))
{
    setLayout(_layout);
    _layout->setSpacing(0);
    _layout->setContentsMargins(0, 0, 0, 0);

    _layout->addWidget(_breakpointSidebar);
    _layout->addWidget(_lineNumberSidebar);
    _layout->addWidget(_textEditor);

    connect(_textEditor, &QPlainTextEdit::cursorPositionChanged, this,
        [this] () {
            emit cursorPositionChanged();
    });
    connect(_textEditor, &QPlainTextEdit::selectionChanged, this,
        [this] () {
            emit selectionChanged();
    });
    connect(_textEditor, &TextEditor::textRemoved, this,
        [this] (const int position, const int nbCharsRemoved) {
            emit textRemoved(position, nbCharsRemoved);
    });
    connect(_textEditor, &TextEditor::textAdded, this,
        [this] (const int position, const QString &text) {
            emit textAdded(position, text);
    });
    connect(_textEditor, &TextEditor::hasFocus, this,
        [this] () {
            emit documentHasFocus();
    });
    connect(_textEditor, &TextEditor::losesFocus, this,
        [this] () {
            emit documentLosesFocus();
    });

    QFontDatabase::addApplicationFont(":/font/Inconsolata-Regular.ttf");
#ifdef Q_OS_WIN
    QFont font("Inconsolata [Regular]", 8);
#elif defined Q_OS_MAC
    QFont font("Inconsolata [Regular]", 14);
#else // TODO: linux, to test
    QFont font("Inconsolata [Regular]", 9);
#endif

    _textEditor->setFont(font);
}

QTextDocument *WdEditor::openFile(QString const &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        const QString text = file.readAll();
        QTextDocument *document = new QTextDocument(_textEditor);
        document->setPlainText(text);
        QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(document);
        document->setDocumentLayout(layout);
        setDocument(document);
        return document;
    }
    file.close();
    return nullptr;
}
