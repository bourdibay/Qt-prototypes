//
//   WdScintilla.cpp
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:46:55
//
//
//

// std includes
#include <cwchar>
#include <string>

// Qt includes
#include <QDebug>
#include <QString>
#include <QFile>
#include <QPixmap>
#include <QColor>
#include <QMenu>
#include <QShortcut>
#include <QScrollBar>
#include <QGridLayout>
#include <QFontDatabase>
#include <QContextMenuEvent>

#include "TextEditor/WdScintilla.h"
#include "TextEditor/CallTip.h"
#include "TextEditor/ScintillaPrivate.h"

const int WdScintilla::CALLTIP_HEIGHT_SHIFT = 15;

WdScintilla::WdScintilla(QWidget *parent)
    : _scintilla(new ScintillaPrivate(this)),
      _filename(),
      _layout(new QGridLayout()),
      _textCopied(),
      _textGoingToBeDeleted(),
      _lengthTextDeleted(0) {
    setParent(parent);
    // _scintilla->setUtf8(true);

    _layout->addWidget(_scintilla, 0, 0, 0, 0);
    _layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_layout);

    setMarginLineNumber(0);
    displayLineNumbers(0, true);
    setMarginWidth(0, "00000");

    QFontDatabase::addApplicationFont(":/font/Inconsolata-Regular.ttf");
    QFontDatabase::addApplicationFont(":/font/Inconsolata-Bold.ttf");
#ifdef Q_OS_WIN
    QFont font("Inconsolata [Regular]", 8);
#elif defined Q_OS_MAC
    QFont font("Inconsolata [Regular]", 14);
#else // TODO: linux, to test
    QFont font("Inconsolata [Regular]", 9);
#endif
    _scintilla->setFont(font);

    QObject::connect(_scintilla, SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)),
                     this, SLOT(slot_marginClicked(int, int, Qt::KeyboardModifiers)));
    QObject::connect(_scintilla, SIGNAL(cursorPositionChanged(int, int)),
                     this, SLOT(slot_cursorPositionChanged(int, int)));
    QObject::connect(_scintilla, SIGNAL(SCN_MODIFIED(int, int, const char *, int, int, int, int, int,
                                        int, int)),
                     this, SLOT(slot_textModified(int, int, const char *, int, int, int, int, int, int, int)));
    QObject::connect(_scintilla, SIGNAL(SCN_ZOOM()),
                     this, SLOT(slot_zoom()));

    QObject::connect(_scintilla->getCallTip(),
                     SIGNAL(entryChosen(QString const &)),
                     this, SLOT(slot_callTip_entryChosen(QString const &)));
    QObject::connect(_scintilla, SIGNAL(SCN_FOCUSOUT()), this,
                     SLOT(slot_lostFocus()));
    QObject::connect(_scintilla, SIGNAL(SCN_FOCUSIN()), this,
                     SLOT(slot_getFocus()));
    QObject::connect(_scintilla->horizontalScrollBar(),
                     SIGNAL(actionTriggered(int)), this,
                     SLOT(slot_scrollbarMoved(int)));
    QObject::connect(_scintilla->verticalScrollBar(),
                     SIGNAL(actionTriggered(int)), this,
                     SLOT(slot_scrollbarMoved(int)));
}

WdScintilla::~WdScintilla() {
    qDebug() << "Destrucotr of Scintilla";
    // TODO: ask to save here ? I don't think so
    // It should be up to the splitEditor to detect that I think.
    // Thus we need a hasChanged() method.
}

//TODO: take a QFile (because of WFileSystemWatcher already has a QFile)
void WdScintilla::openFile(QString const &filename) {
    _filename = filename;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly) == false) {
        qDebug() << "Cannot open file " << filename;
        //TODO: throw
    }
    QByteArray content = file.readAll();

    file.close();

    setReadOnly(false);
    clearText();
    addText(content);
}

void WdScintilla::save(QString const &name) {
    const QString nameToUse = name.isEmpty() ? _filename : name;
    qDebug() << "Saving file as:" << nameToUse;
    QFile file(nameToUse);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << tr("Cannot open the file %1 in write mode").arg(nameToUse);
        return;
    }
    QTextStream out(&file);
    out << getText(0, getLengthDocument());
    file.close();
}

bool WdScintilla::eventFilter(QObject *o, QEvent *ev) {
    if (ev->type() == QEvent::ShortcutOverride && o == this) {
        ev->accept();
        return true;
    }
    return QWidget::eventFilter(o, ev);
}


bool WdScintilla::isReadOnly() const {
    return _scintilla->isReadOnly();
}

void WdScintilla::setReadOnly(const bool s) const {
    _scintilla->setReadOnly(s);
}

// Set text
void WdScintilla::setText(QString const &text) const {
    _scintilla->setText(text);
}

void WdScintilla::setText(QByteArray const &text) const {
    _scintilla->setText(text);
}

void WdScintilla::setText(char const *str, const long length) const {
    const QString qstr = QString::fromUtf8(str, length);
    setText(qstr);
}

void WdScintilla::setText(char const *str) const {
    setText(str, static_cast<long>(strlen(str)));
}

void WdScintilla::setText(std::string const &str) const {
    setText(str.c_str(), static_cast<long>(str.length()));
}

// add text
void WdScintilla::addText(QString const &str) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_ADDTEXT,
                              static_cast<unsigned long>(str.toUtf8().length()), str.toUtf8().constData());
}

void WdScintilla::addText(QByteArray const &text) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_ADDTEXT, text.length(), text.constData());
}

void WdScintilla::addText(char const *str) const {
    addText(str, static_cast<int>(strlen(str)));
}

void WdScintilla::addText(char const *str, const long length) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_ADDTEXT, length, str);
}

void WdScintilla::addText(std::string const &str) const {
    addText(str.c_str());
}

//////// APPEND TEXT //////////
void WdScintilla::appendText(QString const &str) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_APPENDTEXT, str.toUtf8().length(),
                              str.toUtf8().constData());
}

void WdScintilla::appendText(QByteArray const &text) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_APPENDTEXT, text.length(), text.constData());
}

void WdScintilla::appendText(char const *str) const {
    appendText(str, static_cast<int>(strlen(str)));
}

void WdScintilla::appendText(char const *str, const long length) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_APPENDTEXT, length, str);
}

void WdScintilla::appendText(std::string const &str) const {
    appendText(str.c_str());
}

///////// INSERT TEXT ///////////////
void WdScintilla::insertText(const int pos, char const *text) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_INSERTTEXT, pos, text);
}

void WdScintilla::insertText(const int line, const int index, char const *str) const {
    const int pos = _scintilla->positionFromLineIndex(line, index);
    insertText(pos, str);
}

void WdScintilla::insertText(const int pos, QByteArray const &text) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_INSERTTEXT, pos, text.constData());
}

void WdScintilla::insertText(const int pos, QString const &text) const {
    insertText(pos, text.toUtf8());
}

void WdScintilla::insertText(const int line, const int index,
                                        QString const &text) const {
    _scintilla->insertAt(text, line, index);
}

///////// COPY / CUT / PASTE //////////////////

void WdScintilla::copySelection() {
    const QString text = getSelectedText();
    if (text.isEmpty() == false) {
        _textCopied = text;
        _scintilla->copy();
    }
}

void WdScintilla::copy(const int start, const int end) {
    const QString text = getText(start, end);
    if (text.isEmpty() == false) {
        _textCopied = text;
        _scintilla->SendScintilla(QsciScintilla::SCI_COPYRANGE, start, end);
    }
}

void WdScintilla::copy(const int lineStart, const int indexStart,
                                  const int lineEnd, const int indexEnd) {
    const int posBegin = _scintilla->positionFromLineIndex(lineStart, indexStart);
    const int posEnd = _scintilla->positionFromLineIndex(lineEnd, indexEnd);
    copy(posBegin, posEnd);
}

void WdScintilla::copyToClipboard(const long length, char const *text) {
    _textCopied = QString::fromUtf8(text, length);
    _scintilla->SendScintilla(QsciScintilla::SCI_COPYTEXT, length, text);
}

void WdScintilla::copyToClipboard(const long length, std::string const &text) {
    copyToClipboard(length, text.c_str());
}

void WdScintilla::copyToClipboard(QString const &text) {
    _textCopied = text;
    _scintilla->SendScintilla(QsciScintilla::SCI_COPYTEXT, text.toUtf8().length(),
                              text.toUtf8().constData());
}

void WdScintilla::paste() const {
    _scintilla->paste();
}

void WdScintilla::cutSelection() {
    const QString text = getSelectedText();
    if (text.isEmpty() == false) {
        _textCopied = text;
        _scintilla->cut();
    }
}

QString WdScintilla::getTextCopied() const {
    return _textCopied;
}

void WdScintilla::duplicateLine() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LINEDUPLICATE);
}

void WdScintilla::copyCurrentLine() {
    const QString str = getCurrentLine(-1);
    _scintilla->SendScintilla(QsciScintilla::SCI_LINECOPY);
    _textCopied = str;
}

void WdScintilla::cutCurrentLine() {
    const QString str = getCurrentLine(-1);
    _scintilla->SendScintilla(QsciScintilla::SCI_LINECUT);
    _textCopied = str;
}

////////////////////

bool WdScintilla::canPaste() const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_CANPASTE);
}

bool WdScintilla::canCopy() const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETSELECTIONEMPTY);
}

bool WdScintilla::canCut() const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETSELECTIONEMPTY);
}

bool WdScintilla::canRedo() const {
    return _scintilla->isRedoAvailable();
}

bool WdScintilla::canUndo() const {
    return _scintilla->isUndoAvailable();
}

// Move caret left one character.
void WdScintilla::moveCaretCharLeft() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_WORDLEFT);
}

// Move caret left one character extending selection to new caret position.
void WdScintilla::moveCaretCharLeftExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_WORDLEFTEXTEND);
}

// Move caret right one character.
void WdScintilla::moveCaretCharRight() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_WORDRIGHT);
}

// Move caret right one character extending selection to new caret position.
void WdScintilla::moveCaretCharRightExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_WORDRIGHTEXTEND);
}

void WdScintilla::moveCaretToEndDocument() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DOCUMENTEND);
}

void WdScintilla::moveCaretToEndDocumentExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DOCUMENTENDEXTEND);
}

void WdScintilla::moveCaretToStartDocument() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DOCUMENTSTART);
}

void WdScintilla::moveCaretToStartDocumentExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DOCUMENTSTARTEXTEND);
}

void WdScintilla::moveLineDown() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LINEDOWN);
}

void WdScintilla::moveLineDownExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LINEDOWNEXTEND);
}

void WdScintilla::moveLineUp() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LINEUP);
}

void WdScintilla::moveLineUpExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LINEUPEXTEND);
}

void WdScintilla::moveCaretStartLine() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_HOME);
}

void WdScintilla::moveCaretStartLineExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_HOMEEXTEND);
}

void WdScintilla::moveCaretEndLine() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LINEEND);
}

void WdScintilla::moveCaretEndLineExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LINEENDEXTEND);
}

void WdScintilla::moveCaretPageDown() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_STUTTEREDPAGEDOWN);
}

void WdScintilla::moveCaretPageDownExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_STUTTEREDPAGEDOWNEXTEND);
}

void WdScintilla::moveCaretPageUp() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_STUTTEREDPAGEUP);
}

void WdScintilla::moveCaretPageUpExtend() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_STUTTEREDPAGEUPEXTEND);
}

void WdScintilla::setCurrentPosition(const int pos) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETCURRENTPOS, pos);
}

void WdScintilla::setCurrentPosition(const int line, const int index) const {
    setCurrentPosition(_scintilla->positionFromLineIndex(line, index));
}

void WdScintilla::setCurrentPositionNoSelection(const int pos) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETEMPTYSELECTION, pos);
}

void WdScintilla::setCurrentPositionNoSelection(const int line, const int index) const {
    setCurrentPositionNoSelection(_scintilla->positionFromLineIndex(line, index));
}

void WdScintilla::setCaretForegroundColor(QColor const &color) const {
    _scintilla->setCaretForegroundColor(color);
}

void WdScintilla::setCaretLineBackgroundColor(QColor const &color) const {
    _scintilla->setCaretLineBackgroundColor(color);
}

// delete the text selected.
void WdScintilla::deleteSelection() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_CLEAR);
}

// Delete all text in the document.
void WdScintilla::deleteAll() const {
    _scintilla->clear();
}

void WdScintilla::setEOL(const WdScintilla::eEOLType mode) {
    _scintilla->setEolMode(static_cast<QsciScintilla::EolMode>(mode));
}

WdScintilla::eEOLType WdScintilla::getEOL() const {
    return static_cast<WdScintilla::eEOLType>(_scintilla->eolMode());
}

void WdScintilla::convertEOL(const WdScintilla::eEOLType mode) {
    _scintilla->convertEols(static_cast<QsciScintilla::EolMode>(mode));
}

// display the end of line caractere.
void WdScintilla::displayEOL(const bool display) {
    _scintilla->setEolVisibility(display);
}

bool WdScintilla::isEOLDisplayed() const {
    return _scintilla->eolVisibility();
}

// Fold management

int WdScintilla::getLineLength(const int line) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_LINELENGTH, line);
}

QString WdScintilla::getLine(const int line) const {
    const int lengthLine = getLineLength(line);
    char *strLine = new char[lengthLine + 1];
    strLine[lengthLine] = '\0';
    _scintilla->SendScintilla(QsciScintilla::SCI_GETLINE, line, strLine);
    QString qstrLine = QString::fromUtf8(strLine, lengthLine);
    return qstrLine;
}

int WdScintilla::getLengthRange(const int lineStart, const int indexStart,
        const int lineEnd, const int indexEnd) const {
    const long posBegin = _scintilla->positionFromLineIndex(lineStart, indexStart);
    const long posEnd = _scintilla->positionFromLineIndex(lineEnd, indexEnd);
    return posEnd - posBegin;
}


QString WdScintilla::getSelectedText() const {
    return _scintilla->selectedText();
}

QString WdScintilla::getText(const long begin, const long end) const {
    const int length = end - begin;
    char *buffer = new char[length + 1];
    _scintilla->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE,
                              begin, end, buffer);
    QString ret = QString::fromUtf8(buffer, length);
    return ret;
}

QString WdScintilla::getText(const int lineStart, const int indexStart,
                                        const int lineEnd, const int indexEnd) const {
    const long posBegin = _scintilla->positionFromLineIndex(lineStart, indexStart);
    const long posEnd = _scintilla->positionFromLineIndex(lineEnd, indexEnd);
    return getText(posBegin, posEnd);
}

int WdScintilla::getBeginPositionLine(const int line) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_POSITIONFROMLINE, line);
}

int WdScintilla::getEndPositionLine(const int line) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETLINEENDPOSITION, line);
}

int WdScintilla::getLineNumber(const int pos) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_LINEFROMPOSITION, pos);
}

int WdScintilla::getSelectionBegin() const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETSELECTIONNSTART);
}

int WdScintilla::getSelectionEnd() const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETSELECTIONEND);
}

int WdScintilla::getCurrentPosition() const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETCURRENTPOS);
}

char WdScintilla::getChar(const int pos) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETCHARAT, pos);
}

char WdScintilla::getChar(const int line, const int index) const {
    return getChar(_scintilla->positionFromLineIndex(line, index));
}

QString WdScintilla::getCurrentLine(int length) const {
    if (length < 0) {
        const int currPos = getCurrentPosition();
        const int currLine = getLineNumber(currPos);
        return getLine(currLine);
    }
    char *buffer = new char[length + 1];
    _scintilla->SendScintilla(QsciScintilla::SCI_GETCURLINE, length, buffer);
    QString str = QString::fromUtf8(buffer, length);
    return str;
}

int WdScintilla::getCurrentLineNumber() const {
    const int pos = getCurrentPosition();
    return getLineNumber(pos);
}

int WdScintilla::getCurrentIndexNumber() const {
    const int pos = getCurrentPosition();
    return getIndexNumber(pos);
}

int WdScintilla::getLengthDocument() const {
    return _scintilla->text().length();
}

int WdScintilla::getNumberLines() const {
    return _scintilla->lines();
}

void WdScintilla::gotoLine(const int line) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_GOTOLINE, line);
}

void WdScintilla::gotoPos(const int pos) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_GOTOPOS, pos);
}

int WdScintilla::getWordStartPosition(const int pos) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_WORDSTARTPOSITION, pos, true);
}

int WdScintilla::getWordStartPosition(const int line, const int index) const {
    return getWordStartPosition(_scintilla->positionFromLineIndex(line, index));
}

int WdScintilla::getWordEndPosition(const int pos) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_WORDENDPOSITION, pos, true);
}

int WdScintilla::getWordEndPosition(const int line, const int index) const {
    return getWordEndPosition(_scintilla->positionFromLineIndex(line, index));
}

void WdScintilla::deleteBack() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DELETEBACK);
}

void WdScintilla::deleteBackNoLine() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DELETEBACKNOTLINE);
}

void WdScintilla::deleteLineLeft() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DELLINELEFT);
}

void WdScintilla::deleteLineRight() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DELLINERIGHT);
}

void WdScintilla::deleteLeftWord() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DELWORDLEFT);
}

void WdScintilla::deleteRightWord() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DELWORDRIGHT);
}

void WdScintilla::deleteCurrentLine() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LINEDELETE);
}

void WdScintilla::deleteRange(const int pos, const long length) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_DELETERANGE, pos, length);
}

void WdScintilla::deleteRange(const int line,
        const int index,
        const long length) const {
    deleteRange(_scintilla->positionFromLineIndex(line, index), length);
}

void WdScintilla::selectAll() const {
    _scintilla->selectAll(true);
}

void WdScintilla::unselect() const {
    _scintilla->selectAll(false);
}

void WdScintilla::setSelectionBackgroundColor(QColor const &color) const {
    _scintilla->setSelectionBackgroundColor(color);
}

void WdScintilla::setSelectionForegroundColor(QColor const &color) const {
    _scintilla->setSelectionForegroundColor(color);
}


void WdScintilla::setZoom(const int zoom) const {
    const int currentZoom = getZoom();
    if (currentZoom > zoom) {
        setZoomRange(-1 * (currentZoom - zoom));
    }
    else if (currentZoom < zoom) {
        setZoomRange(zoom - currentZoom);
    }
    else {   // = 0
        setZoomRange(-currentZoom);
    }
}

void WdScintilla::setZoomRange(const int range) const {
    if (range > 0) {
        _scintilla->zoomIn(range);
    }
    else {
        _scintilla->zoomOut(-range);
    }
}

int WdScintilla::getZoom() const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETZOOM);
}

void WdScintilla::setOverwriteMode(const bool state) const {
    _scintilla->setOverwriteMode(state);
}

bool WdScintilla::isOverwriteMode() const {
    return _scintilla->overwriteMode();
}

int WdScintilla::getIndexNumber(const int pos) const {
    int line = 0;
    int index = 0;
    _scintilla->lineIndexFromPosition(pos, &line, &index);
    return index;
}

void WdScintilla::lowerCaseSelection() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_LOWERCASE);
}

void WdScintilla::upperCaseSelection() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_UPPERCASE);
}

void WdScintilla::selectRange(const int begin, const int end) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETSEL, begin, end);
}

void WdScintilla::selectRange(const int beginLine, const int beginIndex,
        const int endLine, const int endIndex) const {
    const int posBegin = _scintilla->positionFromLineIndex(beginLine, beginIndex);
    const int posEnd = _scintilla->positionFromLineIndex(endLine, endIndex);
    selectRange(posBegin, posEnd);
}


void WdScintilla::setMarginSymbol(const int margin) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETMARGINTYPEN,
                              margin, QsciScintilla::SC_MARGIN_SYMBOL);
}

void WdScintilla::setMarginText(const int margin) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETMARGINTYPEN,
                              margin, QsciScintilla::SC_MARGIN_TEXT);
}

void WdScintilla::setMarginLineNumber(const int margin) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETMARGINTYPEN,
                              margin, QsciScintilla::SC_MARGIN_NUMBER);
}

void WdScintilla::setMarginTextJustified(const int margin) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETMARGINTYPEN,
                              margin, QsciScintilla::SC_MARGIN_RTEXT);
}

void WdScintilla::setMarginWidth(const int margin, QString const &s) const {
    _scintilla->setMarginWidth(margin, s);
}

void WdScintilla::displayLineNumbers(const int margin, const bool display) const {
    _scintilla->setMarginLineNumbers(margin, display);
}

void WdScintilla::setMarginSensitiveToClick(const int margin, const bool sens) const {
    _scintilla->setMarginSensitivity(margin, sens);
}

int WdScintilla::defineMarker(const WdMarkerSymbol symbol, const int nbMarker) const {
    return _scintilla->markerDefine(symbol, nbMarker);
}

int WdScintilla::defineMarker(const char ch, const int nbMarker) const {
    return _scintilla->markerDefine(ch, nbMarker);
}

int WdScintilla::defineMarker(const QPixmap &pm, const int nbMarker) const {
    return _scintilla->markerDefine(pm, nbMarker);
}

int WdScintilla::defineMarker(const QImage &im, const int nbMarker) const {
    return _scintilla->markerDefine(im, nbMarker);
}

void WdScintilla::setMarkerBackgroundColor(QColor const &color,
        const int nbMarker) const {
    _scintilla->setMarkerBackgroundColor(color, nbMarker);
}

void WdScintilla::setMarkerForegroundColor(QColor const &color,
        const int nbMarker) const {
    _scintilla->setMarkerForegroundColor(color, nbMarker);
}

void WdScintilla::setMarginMarkerMask(const int margin, const int mask) const {
    _scintilla->setMarginMarkerMask(margin, mask);
}

void WdScintilla::setMarginMarkerMask(const int margin,
        QList<int> const &markers) const {
    setMarginMarkerMask(margin, getMaskFromMarkers(markers));
}

void WdScintilla::setMarginsBackgroundColor(QColor const &color) const {
    _scintilla->setMarginsBackgroundColor(color);
}

void WdScintilla::setMarginsForegroundColor(QColor const &color) const {
    _scintilla->setMarginsForegroundColor(color);
}

void WdScintilla::addMarker(const int line, const int marker) const {
    _scintilla->markerAdd(line, marker);
}

void WdScintilla::addSetMarker(const int line, const int mask) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_MARKERADDSET,
                              line, mask);
}

void WdScintilla::addSetMarker(const int line, QList<int> const &mask) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_MARKERADDSET,
                              line, getMaskFromMarkers(mask));
}

unsigned int WdScintilla::getMarkersMask(const int line) const {
    return _scintilla->markersAtLine(line);
}

QList<int> WdScintilla::getMarkers(const int line) const {
    return getMarkersFromMask(getMarkersMask(line));
}

void WdScintilla::deleteMarker(const int line, const int marker) const {
    _scintilla->markerDelete(line, marker);
}

void WdScintilla::deleteAllMarkers() const {
    deleteAllMarkers(-1);
}

void WdScintilla::deleteAllMarkers(const int marker) const {
    _scintilla->markerDeleteAll(marker);
}

void WdScintilla::deleteAllMarkersOnLine(const int line) const {
    QList<int> markers = getMarkers(line);
    for (int i : markers) {
        deleteMarker(line, i);
    }
}

int WdScintilla::findNextMarkers(const int line, const unsigned int mask) const {
    return _scintilla->markerFindNext(line, mask);
}

int WdScintilla::findNextMarkers(const int line, QList<int> const &markers) const {
    return findNextMarkers(line, getMaskFromMarkers(markers));
}

int WdScintilla::findNextMarker(const int line, const int marker) const {
    return findNextMarkers(line, QList<int>() << marker);
}

int WdScintilla::findPreviousMarkers(const int line, const unsigned int mask) const {
    return _scintilla->markerFindPrevious(line, mask);
}

int WdScintilla::findPreviousMarkers(const int line, QList<int> const &markers) const {
    return findPreviousMarkers(line, getMaskFromMarkers(markers));
}

int WdScintilla::findPreviousMarker(const int line, const int marker) const {
    return findPreviousMarkers(line, QList<int>() << marker);
}

QList<int> WdScintilla::getLinesWithMarkers(const int fromLine,
        const int toLine,
        const unsigned int mask) const {
    QList<int> ret;
    int currentLine = fromLine;
    while (1) {
        const int lineMatched = findNextMarkers(currentLine, mask);
        if (lineMatched > toLine || lineMatched < 0) {
            return ret;
        }
        else {
            ret << lineMatched;
            currentLine = lineMatched + 1;
        }
    }
    return ret;
}

QList<int> WdScintilla::getLinesWithMarkers(const int fromLine,
        const int toLine,
        QList<int> const &markers) const

{
    return getLinesWithMarkers(fromLine, toLine, getMaskFromMarkers(markers));
}

QList<int> WdScintilla::getLinesWithMarker(const int fromLine,
        const int toLine,
        const int marker) const

{
    return getLinesWithMarkers(fromLine, toLine, QList<int>() << marker);
}

QList<int> WdScintilla::getLinesWithMarkers(const int fromLine,
        const unsigned int mask) const

{
    return getLinesWithMarkers(fromLine, getNumberLines(), mask);
}

QList<int> WdScintilla::getLinesWithMarkers(const int fromLine,
        QList<int> const &markers) const

{
    return getLinesWithMarkers(fromLine, getMaskFromMarkers(markers));
}

QList<int> WdScintilla::getLinesWithMarker(const int fromLine,
        const int marker) const

{
    return getLinesWithMarkers(fromLine, QList<int>() << marker);
}

QList<int> WdScintilla::getLinesWithMarkers(const unsigned int mask) const

{
    return getLinesWithMarkers(0, getNumberLines(), mask);
}

QList<int> WdScintilla::getLinesWithMarkers(QList<int> const &markers) const {
    return getLinesWithMarkers(getMaskFromMarkers(markers));
}

QList<int> WdScintilla::getLinesWithMarker(const int marker) const {
    return getLinesWithMarkers(QList<int>() << marker);
}

unsigned int WdScintilla::getMaskFromMarkers(QList<int> const &markers) const {
    unsigned int ret = 0;
    for (int i : markers) {
        ret = ret | (1 << i);
    }
    return ret;
}

QList<int> WdScintilla::getMarkersFromMask(const unsigned int mask) const {
    int max = 32;
    QList<int> ret;
    for (int i = 0; i < max; ++i) {
        if (((mask >> i) & 0x01) == 1) {
            ret << i;
        }
    }
    return ret;
}

/////////////// STYLING ///////////////////////
void WdScintilla::startStyling(const int position, const int mask) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_STARTSTYLING, position, mask);
}

void WdScintilla::setStyling(const int length, const int style) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETSTYLING, length, style);
}

void WdScintilla::setForegroundColor(const int numberStyle,
        QColor const &color) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, numberStyle, color);
}

void WdScintilla::setBackgroundColor(const int numberStyle,
        QColor const &color) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_STYLESETBACK, numberStyle, color);
}

void WdScintilla::applyStyle(const int pos, const int length,
                                        const int numberStyle) const {
    startStyling(pos, 31);
    setStyling(length, numberStyle);
}

void WdScintilla::applyStyle(const int lineStart, const int indexStart,
                                        const int lineEnd, const int indexEnd,
                                        const int numberStyle) const {
    const int posStart = _scintilla->positionFromLineIndex(lineStart, indexStart);
    const int posEnd = _scintilla->positionFromLineIndex(lineEnd, indexEnd);
    applyStyle(posStart, posEnd - posStart, numberStyle);
}

void WdScintilla::applyStyle(const int lineStart, const int indexStart,
                                        const int length,
                                        const int numberStyle) const {
    const int posStart = _scintilla->positionFromLineIndex(lineStart, indexStart);
    applyStyle(posStart, length, numberStyle);
}


int WdScintilla::defineIndicator(const WdIndicator style, const int number) const {
    return _scintilla->indicatorDefine(style, number);
}

void WdScintilla::setIndicator(const int lineStart, const int indexStart,
        const int lineEnd, const int indexEnd,
        const int numberIndicator) const {
    _scintilla->fillIndicatorRange(lineStart, indexStart,
                                   lineEnd, indexEnd, numberIndicator);
}

void WdScintilla::clearIndicator(const int lineStart, const int indexStart,
        const int lineEnd, const int indexEnd,
        const int number) const {
    _scintilla->clearIndicatorRange(lineStart, indexStart,
                                    lineEnd, indexEnd, number);
}

void WdScintilla::setIndicatorColor(QColor const &color, const int number) const {
    _scintilla->setIndicatorForegroundColor(color, number);
}


void WdScintilla::setDocumentBackground(QColor const &color) const {
    _scintilla->setPaper(color);
}

QColor WdScintilla::getDocumentBackground() const {
    return _scintilla->paper();
}

void WdScintilla::setDocumentForeground(QColor const &color) const {
    _scintilla->setColor(color);
}

QColor WdScintilla::getDocumentForeground() const {
    return _scintilla->color();
}

/////////// FOLDING //////////////

void WdScintilla::setFolding(const WdScintilla::WdFoldingStyle style,
                                        const int margin) const {
    _scintilla->setFolding(style, margin);
}

int WdScintilla::getFoldLevel(const int line) const {
    int level = getFoldLevelWithMask(line);
    level &= QsciScintilla::SC_FOLDLEVELNUMBERMASK;
    return level;
}

int WdScintilla::getFoldLevelWithMask(const int line) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETFOLDLEVEL, line);
}

bool WdScintilla::isFoldHeader(const int line) const {
    int level = _scintilla->SendScintilla(QsciScintilla::SCI_GETFOLDLEVEL, line);
    level &= QsciScintilla::SC_FOLDLEVELHEADERFLAG;
    return level != 0;
}

bool WdScintilla::isFoldWhiteLine(const int line) const {
    int level = _scintilla->SendScintilla(QsciScintilla::SCI_GETFOLDLEVEL, line);
    level &= QsciScintilla::SC_FOLDLEVELWHITEFLAG;
    return level != 0;
}

void WdScintilla::setFoldHeader(const int line, const bool st) const {
    int level = getFoldLevel(line);
    if (st) {
        level |= QsciScintilla::SC_FOLDLEVELHEADERFLAG;
    }
    else {
        level = level & ~QsciScintilla::SC_FOLDLEVELHEADERFLAG;
    }
    setFoldLevelWithMask(line, level);
}

void WdScintilla::setFoldWhiteLine(const int line, const bool st) const {
    int level = getFoldLevel(line);
    if (st) {
        level |= QsciScintilla::SC_FOLDLEVELWHITEFLAG;
    }
    else {
        level = level & ~QsciScintilla::SC_FOLDLEVELWHITEFLAG;
    }
    setFoldLevelWithMask(line, level);
}

void WdScintilla::setFoldLevelWithMask(const int line, const int level) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETFOLDLEVEL, line, level);
}

void WdScintilla::setFoldLevel(const int line, const int level) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETFOLDLEVEL, line,
                              (level & QsciScintilla::SC_FOLDLEVELNUMBERMASK));
}

int WdScintilla::getFoldBaseLevel() const {
    return QsciScintilla::SC_FOLDLEVELBASE;
}

int WdScintilla::getFoldMaxLevel() const {
    return QsciScintilla::SC_FOLDLEVELNUMBERMASK;
}

void WdScintilla::increaseFoldLevel(const int line, const int inc) const {
    int level = getFoldLevel(line);
    level += inc;
    if (level > getFoldMaxLevel()) {
        level = getFoldMaxLevel();
    }
    if (isFoldHeader(line)) {
        level |= QsciScintilla::SC_FOLDLEVELHEADERFLAG;
    }
    if (isFoldWhiteLine(line)) {
        level |= QsciScintilla::SC_FOLDLEVELWHITEFLAG;
    }
    setFoldLevelWithMask(line, level);
}

void WdScintilla::decreaseFoldLevel(const int line, const int inc) const {
    int level = getFoldLevel(line);
    level -= inc;
    if (level < getFoldBaseLevel()) {
        level = getFoldBaseLevel();
    }
    if (isFoldHeader(line)) {
        level |= QsciScintilla::SC_FOLDLEVELHEADERFLAG;
    }
    if (isFoldWhiteLine(line)) {
        level |= QsciScintilla::SC_FOLDLEVELWHITEFLAG;
    }
    setFoldLevelWithMask(line, level);
}

void WdScintilla::setFoldPart(const int lineStart, const int lineEnd) const {
    setFoldHeader(lineStart, true);
    for (int i = lineStart + 1; i <= lineEnd; ++i) {
        increaseFoldLevel(i, 1);
    }
}

void WdScintilla::setFoldPart(const int lineStart, const int lineEnd,
        const int level) const {
    for (int i = lineStart; i <= lineEnd; ++i) {
        setFoldLevel(i, level);
    }
}

void WdScintilla::setFoldPartWithMask(const int lineStart, const int lineEnd,
        const int level) const {
    for (int i = lineStart; i <= lineEnd; ++i) {
        setFoldLevelWithMask(i, level);
    }
}

void WdScintilla::removeFoldPart(const int lineStart, const int lineEnd) const {
    setFoldHeader(lineStart, false);
    for (int i = lineStart; i <= lineEnd; ++i) {
        decreaseFoldLevel(i, 1);
    }
}

void WdScintilla::removeFoldPart(const int line) const {
    const int lineStart = getFoldParent(line);
    const int lineEnd = getFoldLastChild(lineStart);
    removeFoldPart(lineStart, lineEnd);
}

int WdScintilla::getFoldParent(const int line) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETFOLDPARENT, line);
}

int WdScintilla::getFoldLastChild(const int line) const {
    return _scintilla->SendScintilla(QsciScintilla::SCI_GETLASTCHILD, line, -1);
}

/////////// BRACES /////////////
void WdScintilla::setMatchedBraceBackgroundColor(QColor const &color) const {
    _scintilla->setMatchedBraceBackgroundColor(color);
}

void WdScintilla::setMatchedBraceForegroundColor(QColor const &color) const {
    _scintilla->setMatchedBraceForegroundColor(color);
}

void WdScintilla::setUnmatchedBraceBackgroundColor(QColor const &color) const {
    _scintilla->setUnmatchedBraceBackgroundColor(color);
}

void WdScintilla::setUnmatchedBraceForegroundColor(QColor const &color) const {
    _scintilla->setUnmatchedBraceForegroundColor(color);
}

void WdScintilla::setBracingMatching(const WdScintilla::WdBraceMatch match) const {
    _scintilla->setBraceMatching(match);
}

void WdScintilla::selectToMatchingBrace() const {
    _scintilla->selectToMatchingBrace();
}

void WdScintilla::moveToMatchingBrace() const {
    _scintilla->moveToMatchingBrace();
}

////// SEARCH _ REPLACE ////////////////
bool WdScintilla::findFirst(QString const &expr,
                                       const bool isRegexp,
                                       const bool isCaseSensitive,
                                       const bool matchWordOnly,
                                       const bool searchWrapsEnd,
                                       const bool searchForward,
                                       const int fromLine,
                                       const int fromIndex,
                                       const bool unfoldTextFound,
                                       const bool posixRegexp) const {
    return _scintilla->findFirst(expr, isRegexp, isCaseSensitive,
                                 matchWordOnly, searchWrapsEnd, searchForward,
                                 fromLine, fromIndex, unfoldTextFound, posixRegexp);
}

bool WdScintilla::findFirstInSelection(QString const &expr,
        const bool isRegexp,
        const bool isCaseSensitive,
        const bool matchWordOnly,
        const bool searchForward,
        const bool unfoldTextFound,
        const bool posixRegexp) const {
    return _scintilla->findFirstInSelection(expr, isRegexp, isCaseSensitive,
                                            matchWordOnly, searchForward,
                                            unfoldTextFound, posixRegexp);
}

bool WdScintilla::findNext() const {
    return _scintilla->findNext();
}

void WdScintilla::replace(QString const &text) const {
    _scintilla->replace(text);
}

void WdScintilla::replaceSelectedText(QString const &text) const {
    _scintilla->replaceSelectedText(text);
}

///////// TABS / SPACES ////////////
void WdScintilla::setTabsIndentLine(const bool indent) const {
    _scintilla->setTabIndents(indent);
}

bool WdScintilla::getTabsIndentLine() const {
    return _scintilla->tabIndents();
}

void WdScintilla::setBackspaceUnindentLine(const bool indent) const {
    _scintilla->setBackspaceUnindents(indent);
}

bool WdScintilla::getBackspaceUnindentLine() const {
    return _scintilla->backspaceUnindents();
}

void WdScintilla::indentLine(const int line) const {
    _scintilla->indent(line);
}

void WdScintilla::unindentLine(const int line) const {
    _scintilla->unindent(line);
}

void WdScintilla::setAutoIndent(const bool indent) const {
    _scintilla->setAutoIndent(indent);
}

bool WdScintilla::getAutoIndent() const {
    return _scintilla->autoIndent();
}

void WdScintilla::setIndentationLine(const int line, const int width) const {
    _scintilla->setIndentation(line, width);
}

void WdScintilla::setIndentationUseTabs(const bool useTabs) const {
    _scintilla->setIndentationsUseTabs(useTabs);
}

void WdScintilla::setTabWidth(const int width) const {
    _scintilla->setTabWidth(width);
}

int WdScintilla::getTabWidth() const {
    return _scintilla->tabWidth();
}

void WdScintilla::setIndentationWidth(const int width) const {
    _scintilla->setIndentationWidth(width);
}

int WdScintilla::getIndentationWidth() const {
    return _scintilla->indentationWidth();
}

void WdScintilla::setIndentationGuide(const WdIndentGuide guide) const {
    _scintilla->SendScintilla(QsciScintilla::SCI_SETINDENTATIONGUIDES, guide);
}

QPoint WdScintilla::getCursorPixelCoord() const {
    const int pos = getCurrentPosition();
    const int x = _scintilla->SendScintilla(QsciScintilla::SCI_POINTXFROMPOSITION, 0, pos);
    const int y = _scintilla->SendScintilla(QsciScintilla::SCI_POINTYFROMPOSITION, 0, pos);
    return QPoint(x, y);
}

void WdScintilla::clearText() const {
    _scintilla->SendScintilla(QsciScintilla::SCI_CLEARALL);
}

//////////// MENU //////////
QMenu *WdScintilla::getContextualMenu() const {
    return _scintilla->getContextualMenu();
}

////////////// PRIVATE ///////////////

int WdScintilla::getPixelVerticalCallTip(const int y) const {
    return y + CALLTIP_HEIGHT_SHIFT;
}

int WdScintilla::getPixelHorizontalCallTip(const int x) const {
    return x;
}

void WdScintilla::showCalltip(const int pixel_x, const int pixel_y) const {
    _scintilla->showCallTip(pixel_x, pixel_y);
}

void WdScintilla::showCalltipCurrentPosition() const {
    const QPoint p = getCursorPixelCoord();

    const int px = getPixelHorizontalCallTip(p.x());
    int py = getPixelVerticalCallTip(p.y());
    const int callTipHeight = _scintilla->getCallTip()->height();

    // TODO: the same with px
    if (py + callTipHeight > _scintilla->height()) {
        py -= callTipHeight;
        py -= CALLTIP_HEIGHT_SHIFT;
    }

    _scintilla->showCallTip(px, py);
}

void WdScintilla::setCallTipContent(std::vector<CompletionData> &content) {
    _scintilla->getCallTip()->setContent(content);
}

bool WdScintilla::hasDocumentFocus() const {
    return _scintilla->hasFocus();
}

void WdScintilla::setWrapMode(const WdScintilla::WdWrapMode mode) const {
    _scintilla->setWrapMode(mode);
}

void WdScintilla::setWrapMode(const WdScintilla::WdWrapMode mode,
        const WdScintilla::WdWrapVisualFlag start,
        const WdScintilla::WdWrapVisualFlag end) const {
    setWrapMode(mode);
    setWrapVisualFlag(start, end);
}

void WdScintilla::setWrapVisualFlag(const WdScintilla::WdWrapVisualFlag start,
        const WdScintilla::WdWrapVisualFlag end,
        const int indent) const {
    _scintilla->setWrapVisualFlags(start, end, indent);
}

WdScintilla::WdWrapMode WdScintilla::getWrapMode() const {
    return _scintilla->wrapMode();
}

WdScintilla::WdWrapIndentMode WdScintilla::getWrapIndentMode() const {
    return _scintilla->wrapIndentMode();
}

void WdScintilla::setWrapIndentMode(const WdScintilla::WdWrapIndentMode mode)
const {
    _scintilla->setWrapIndentMode(mode);
}

int WdScintilla::getPosition(const int line, const int index) const {
    return _scintilla->positionFromLineIndex(line, index);
}

///////// CONNECT ////////////////
void WdScintilla::slot_marginClicked(int margin, int line, Qt::KeyboardModifiers) {
    emit marginClicked(margin, line);
}

void WdScintilla::slot_cursorPositionChanged(int line, int index) {
    emit cursorPositionChanged(line, index);

    if (_scintilla->isCallTipDisplayed()) {
        showCalltipCurrentPosition();
    }
}

void WdScintilla::slot_textModified(int pos, int modificationType,
        const char *text,
        int length,
        int /*linesAdded*/,
        int /*line*/,
        int /*foldLeveNow*/,
        int /*foldLevelPrev*/,
        int /*token*/,
        int /*annotationLinesAdded*/) {
    /*
    SC_MOD_INSERTTEXT = 0x1, SC_MOD_DELETETEXT = 0x2, SC_MOD_CHANGESTYLE = 0x4,
    SC_MOD_CHANGEFOLD = 0x8, SC_PERFORMED_USER = 0x10, SC_PERFORMED_UNDO = 0x20,
    SC_PERFORMED_REDO = 0x40, SC_MULTISTEPUNDOREDO = 0x80, SC_LASTSTEPINUNDOREDO = 0x100,
    SC_MOD_CHANGEMARKER = 0x200, SC_MOD_BEFOREINSERT = 0x400, SC_MOD_BEFOREDELETE = 0x800,
    SC_MULTILINEUNDOREDO = 0x1000, SC_STARTACTION = 0x2000, SC_MOD_CHANGEINDICATOR = 0x4000,
    SC_MOD_CHANGELINESTATE = 0x8000, SC_MOD_CHANGEMARGIN = 0x10000, SC_MOD_CHANGEANNOTATION = 0x20000,
    SC_MOD_CONTAINER = 0x40000, SC_MOD_LEXERSTATE = 0x80000, SC_MODEVENTMASKALL = 0xfffff
    */
    if (modificationType & _scintilla->SC_MOD_BEFOREDELETE) {
        _textGoingToBeDeleted = getText(pos, pos + length);
        _lengthTextDeleted = length;
    }
    if (modificationType & _scintilla->SC_MOD_DELETETEXT) {
        emit textDeleted(pos, _textGoingToBeDeleted);
    }
    if (modificationType & _scintilla->SC_MOD_INSERTTEXT) {
        if (text) {
            QString str = QString::fromUtf8(text, length);
            emit textInserted(pos, str);
            setZoom(0);
        }
    }
}

void WdScintilla::slot_zoom() {
    emit zoomChanged(getZoom());
}

void WdScintilla::slot_callTip_entryChosen(QString const &text) {
    qDebug() << "slot calltip entry chosen:" << text;
    //TODO: replace text.
}

void WdScintilla::slot_lostFocus() {
    _scintilla->hideCallTip();
}

void WdScintilla::slot_scrollbarMoved(int) {
    _scintilla->hideCallTip();
}

void WdScintilla::slot_getFocus() {
    emit documentHasFocus();
}
