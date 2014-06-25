//
//   WdScintilla.h
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:43:29
//
//
//

#ifndef __WD_SCINTILLA_H__
#define __WD_SCINTILLA_H__

// stl
#include <vector>

// Qsci
#include <Qsci/qsciscintilla.h>

// qt classes
#include <QWidget>
#include <QPointer>
#include <QList>

class QString;
class QGridLayout;
class QFile;
class QByteArray;
class QPixmap;
class QImage;
class QColor;
class QMenu;
class QContextMenuEvent;

// FIXME: colors of the text put the background in white. Update the background
// too.

// The tab/space replacement does not work as expected.
// To replace a tab by a fixed number of spaces,
// we have to catch the textmodified and replace the tab by x spaces
// if the indentation mode is disabled....

// TODO:
// scroll
// highlight
// style
// undo/redo control (how to fusion with my undo/redo management ?)

// maybe annotation and calltip

// widev graphical classes
class CallTip;
struct CompletionData;
class ScintillaPrivate;

/**
* \class WdScintilla
* \author Benjamin Bourdin
* \brief Renderer engine that handle a file and provides
* a lot of functionnalities.
* There is no multiple selection.
*/
class WdScintilla : public QWidget {
    Q_OBJECT

public:
    /**
    * \brief Constructor.
    * \param parent The parent of scintilla.
    */
    WdScintilla(QWidget *parent);
    virtual ~WdScintilla();

    virtual void openFile(QString const &filename);
    virtual void save(QString const &name = QString());

    /**
    * \brief Set the text in the document. Clear all existing text before.
    * \param text The text to set.
    */
    void setText(QString const &text) const;
    /**
    * \brief Set the text in the document. Clear all existing text before.
    * \param str The text to set.
    */
    void setText(char const *str) const;
    /**
    * \brief Set the text in the document. Clear all existing text before.
    * \param str The text to set.
    * \param length The length of the text to write.
    */
    void setText(char const *str, const long length) const;
    /**
    * \brief Set the text in the document. Clear all existing text before.
    * \param str The text to set.
    */
    void setText(std::string const &str) const;
    /**
    * \brief Set the text in the document. Clear all existing text before.
    * \param text The text to set.
    */
    void setText(QByteArray const &text) const;

    /**
    * \brief Add the text after the cursor.
    * \param text The text to set.
    */
    void addText(QString const &text) const;
    /**
    * \brief Add the text after the cursor.
    * \param text The text to set.
    */
    void addText(QByteArray const &text) const;
    /**
    * \brief Add the text after the cursor.
    * \param str The text to set.
    */
    void addText(char const *str) const;
    /**
    * \brief Add the text after the cursor.
    * \param str The text to set.
    * \param length The length of the text to write.
    */
    void addText(char const *str, const long length) const;
    /**
    * \brief Add the text after the cursor.
    * \param str The text to set.
    */
    void addText(std::string const &str) const;

    /**
    * \brief Append a text to the end of the document without changing the
    * selection.
    * \param text The text to append.
    */
    void appendText(QString const &text) const;
    /**
    * \brief Append a text to the end of the document without changing the
    * selection.
    * \param text The text to append.
    */
    void appendText(QByteArray const &text) const;
    /**
    * \brief Append a text to the end of the document without changing the
    * selection.
    * \param str The text to append.
    */
    void appendText(char const *str) const;
    /**
    * \brief Append a text to the end of the document without changing the
    * selection.
    * \param str The text to append.
    * \param length The length of the text to append.
    */
    void appendText(char const *str, const long length) const;
    /**
    * \brief Append a text to the end of the document without changing the
    * selection.
    * \param str The text to append.
    */
    void appendText(std::string const &str) const;

    /**
    * \brief Insert a text in the document.
    * \param pos The position from where to insert.
    * \param str The text to insert.
    */
    void insertText(const int pos, char const *str) const;
    /**
    * \brief Insert a text in the document.
    * \param line The line where to insert.
    * \param index The index on the line where to insert.
    * \param str The text to insert.
    */
    void insertText(const int line, const int index, char const *str) const;
    /**
    * \brief Insert a text in the document.
    * \param pos The position from where to insert.
    * \param text The text to insert.
    */
    void insertText(const int pos, QString const &text) const;
    /**
    * \brief Insert a text in the document.
    * \param line The line where to insert.
    * \param index The index on the line where to insert.
    * \param text The text to omsert.
    */
    void insertText(const int line, const int index, QString const &text) const;
    void insertText(const int pos, QByteArray const &text) const;

    virtual void copySelection();
    /**
    * \brief Copy the text between two positions.
    * \param start The first position.
    * \param end The last position.
    */
    void copy(const int start, const int end);
    /**
    * \brief Copy the text between two positions.
    * \param lineStart The line where starting to copy.
    * \param indexStart The index on the line where starting to copy.
    * \param lineEnd The line where stopping to copy.
    * \param indexEnd The index on the line where stopping to copy.
    */
    void copy(const int lineStart, const int indexStart, const int lineEnd,
              const int indexEnd);
    /**
    * \brief Copy the text to the clipboard.
    * \param length The length of the text.
    * \param text The text to copy.
    */
    void copyToClipboard(const long length, char const *text);
    /**
    * \brief Copy the text to the clipboard.
    * \param length The length of the text.
    * \param text The text to copy.
    */
    void copyToClipboard(const long length, std::string const &text);
    /**
    * \brief Copy the text to the clipboard.
    * \param text The text to copy.
    */
    void copyToClipboard(QString const &text);
    virtual void paste() const;
    // TODO: paste at a pos
    virtual void cutSelection();
    /**
    * \brief Get the text in the clipboard.
    * \return The text copied in the clipboard.
    */
    QString getTextCopied() const;
    /**
    * \brief Duplicate the current line.
    */
    void duplicateLine() const;
    /**
    * \brief Copy The current line.
    */
    void copyCurrentLine();
    /**
    * \brief Cut the current line.
    */
    void cutCurrentLine();

    /**
    * \brief Specify if we can paste something.
    * \return True if we can.
    */
    virtual bool canPaste() const;
    // TODO: see undo/redo.
    virtual bool canRedo() const;
    virtual bool canUndo() const;
    /**
    * \brief Specify if we can copy something.
    * \return True if we can.
    */
    virtual bool canCopy() const;
    /**
    * \brief Specify if we can cut something.
    * \return True if we can.
    */
    virtual bool canCut() const;

    /**
    * \brief Move the caret one character to the left.
    */
    void moveCaretCharLeft() const;
    /**
    * \brief Move the caret one character to the left by extending the
    * selection.
    */
    void moveCaretCharLeftExtend() const;
    /**
    * \brief Move the caret one character to the right.
    */
    void moveCaretCharRight() const;
    /**
    * \brief Move the caret one character to the right by extending the
    * selection.
    */
    void moveCaretCharRightExtend() const;
    /**
    * \brief Move the caret to the end of the document.
    */
    void moveCaretToEndDocument() const;
    /**
    * \brief Move the caret to the end of the document by extending the
    * selection.
    */
    void moveCaretToEndDocumentExtend() const;
    /**
    * \brief Move the caret to the beginning of the document.
    */
    void moveCaretToStartDocument() const;
    /**
    * \brief Move the caret to the beginning of the document by extending the
    * selection.
    */
    void moveCaretToStartDocumentExtend() const;
    /**
    * \brief Move the caret one line down.
    */
    void moveLineDown() const;
    /**
    * \brief Move the caret one line down by extending the selection.
    */
    void moveLineDownExtend() const;
    /**
    * \brief Move the caret one line up.
    */
    void moveLineUp() const;
    /**
    * \brief Move the caret one line up by extending the selection.
    */
    void moveLineUpExtend() const;
    /**
    * \brief Move the caret to the beginning of the line.
    */
    void moveCaretStartLine() const;
    /**
    * \brief Move the caret to the beginning of the line by extending the
    * selection.
    */
    void moveCaretStartLineExtend() const;
    /**
    * \brief Move the caret to the end of the line.
    */
    void moveCaretEndLine() const;
    /**
    * \brief Move the caret to the end of the line by extending the selection.
    */
    void moveCaretEndLineExtend() const;
    /**
    * \brief Move the caret one page down.
    */
    void moveCaretPageDown() const;
    /**
    * \brief Move the caret one page down by extending the selection.
    */
    void moveCaretPageDownExtend() const;
    /**
    * \brief Move the caret one page down up.
    */
    void moveCaretPageUp() const;
    /**
    * \brief Move the caret one page up by extending the selection.
    */
    void moveCaretPageUpExtend() const;

    /**
    * \brief Set the current position by extending the selection.
    * \param pos The position.
    */
    void setCurrentPosition(const int pos) const;
    /**
    * \brief Set the current position by extending the selection.
    * \param line The line of the cursor.
    * \param index The index of the line of the cursor.
    */
    void setCurrentPosition(const int line, const int index) const;
    /**
    * \brief Set the current position without the selection.
    * \param pos The position.
    */
    void setCurrentPositionNoSelection(const int pos) const;
    /**
    * \brief Set the current position without the selection.
    * \param line The line of the cursor.
    * \param index The index of the line of the cursor.
    */
    void setCurrentPositionNoSelection(const int line, const int index) const;
    /**
    * \brief Set the foreground color of the caret.
    * \param col The color.
    */
    void setCaretForegroundColor(QColor const &col) const;
    /**
    * \brief Set the background color of the caret.
    * \param col The color.
    */
    void setCaretLineBackgroundColor(QColor const &col) const;

    /**
    * \brief End of Line management
    */
    enum class eEOLType {
        EOL_WINDOWS = QsciScintilla::EolMode::EolWindows,
        EOL_LINUX = QsciScintilla::EolMode::EolUnix,
        EOL_MAC = QsciScintilla::EolMode::EolMac
    };
    /**
    * \brief Set the End of Line mode.
    * \param mode The mode to set.
    */
    void setEOL(const eEOLType mode);
    /**
    * \brief Get the End of Line mode.
    * \return The End of Line mode.
    */
    eEOLType getEOL() const;
    /**
    * \brief Convert the End of Line mode.
    * \param mode The mode to set.
    */
    void convertEOL(const eEOLType mode);
    /**
    * \brief Display the end of line character.
    * \param display True to display, False to hide.
    */
    void displayEOL(const bool display);
    /**
    * \brief Tell if we are dsplaying the End of Line character.
    * \return True if we display the End of Line character.
    */
    bool isEOLDisplayed() const;

    /**
    * \brief Get the line text.
    * \param line The line number.
    * \return The text at the line.
    */
    QString getLine(const int line) const;
    /**
    * \brief Get the length of the line.
    * \param line The line number.
    * \return The length of the line.
    */
    int getLineLength(const int line) const;
    /**
    * \brief Get the selected text.
    * \return The selected text.
    */
    QString getSelectedText() const;
    /**
    * \brief Get the text between two positions.
    * \param begin The first position.
    * \param end The last position.
    * \return The text between both positions.
    */
    QString getText(const long begin, const long end) const;
    /**
    * \brief Get the text between two positions.
    * \param lineStart The first line number.
    * \param indexStart The index of the first line number.
    * \param lineEnd The last line number.
    * \param indexEnd The index of the last line number.
    * \return The text between both positions.
    */
    QString getText(const int lineStart, const int indexStart,
                    const int lineEnd, const int indexEnd) const;
    /**
    * \brief Get the position of the beginning of the line.
    * \param line The line number.
    * \return The position of the beginning of the line.
    */
    int getBeginPositionLine(const int line) const;
    /**
    * \brief Get the position of the end of the line.
    * \param line The line number.
    * \return The position of the end of the line.
    */
    int getEndPositionLine(const int line) const;
    /**
    * \brief Get the line number where the position is included.
    * \param pos The position.
    * \return The line number that includes the position.
    */
    int getLineNumber(const int pos) const;
    /**
    * \brief Get the index on the line number where the position is included.
    * \param pos The position.
    * \return The index on the line number that includes the position.
    */
    int getIndexNumber(const int pos) const;

    int getCurrentLineNumber() const;
    int getCurrentIndexNumber() const;
    int getLengthRange(const int lineStart, const int indexStart,
                       const int lineEnd, const int indexEnd) const;

    /**
    * \brief Get the position of the beginning of the selected text.
    * \return The position of the beginning of the selected text.
    */
    int getSelectionBegin() const;
    /**
    * \brief Get the position of the end of the selected text.
    * \return The position of the end of the selected text.
    */
    int getSelectionEnd() const;
    /**
    * \brief Get the current position.
    * \return The current position.
    */
    int getCurrentPosition() const;
    /**
    * \brief Get the character at a position.
    * \param pos The position.
    * \return The character at the position.
    */
    char getChar(const int pos) const;
    /**
    * \brief Get the character at a position.
    * \param line The line number.
    * \param index The index on the line number.
    * \return The character at the position.
    */
    char getChar(const int line, const int index) const;
    /**
    * \brief Get the current line text.
    * \param length The length of the text we want.
    * \return The current line text.
    */
    QString getCurrentLine(int length) const;
    /**
    * \brief Get the number of characters in the document.
    * \return The number of characters in the document.
    */
    int getLengthDocument() const;
    /**
    * \brief Get the number of lines in the document.
    * \return The number of lines in the document.
    */
    int getNumberLines() const;

    /**
    * \brief Get the position of the beginning of the word
    * at the position pos.
    * \param pos The position.
    * \return The position of the beginning of the word.
    */
    int getWordStartPosition(const int pos) const;
    /**
    * \brief Get the position of the beginning of the word
    * at the position.
    * \param line The line number.
    * \param index The index on the line number.
    * \return The position of the beginning of the word.
    */
    int getWordStartPosition(const int line, const int index) const;
    /**
    * \brief Get the position of the end of the word
    * at the position pos.
    * \param pos The position.
    * \return The position of the end of the word.
    */
    int getWordEndPosition(const int pos) const;
    /**
    * \brief Get the position of the end of the word
    * at the position.
    * \param line The line number.
    * \param index The index on the line number.
    * \return The position of the end of the word.
    */
    int getWordEndPosition(const int line, const int index) const;

    /**
    * \brief Delete the selection or if no selection,
    * the character before the caret.
    */
    void deleteBack() const;
    /**
    * \brief Delete the selection or if no selection,
    * the character before the caret.
    * Will not delete the character before at the start of a line.
    */
    void deleteBackNoLine() const;
    /**
    * \brief Delete back from the current position to the start of the line.
    */
    void deleteLineLeft() const;
    /**
    * \brief Delete forwards from the current position to the end of the line.
    */
    void deleteLineRight() const;
    /**
    * \brief Delete the word to the left of the caret.
    */
    void deleteLeftWord() const;
    /**
    * \brief Delete the word to the right of the caret.
    */
    void deleteRightWord() const;
    /**
    * \brief Delete the current line.
    */
    void deleteCurrentLine() const;
    /**
    * \brief Delete the selection.
    */
    void deleteSelection() const;
    /**
    * \brief Delete all the text in the document.
    */
    void deleteAll() const;
    /**
    * \brief Delete a text at the position pos.
    * \param pos The position.
    * \param length The number of characters to delete.
    */
    void deleteRange(const int pos, const long length) const;
    /**
    * \brief Delete a text at a position.
    * \param line The line number.
    * \param index The index on the line number.
    * \param length The number of characters to delete.
    */
    void deleteRange(const int line, const int index, const long length) const;

    virtual void selectAll() const;
    /**
    * \brief Select the text between two positions.
    * \param begin The first position.
    * \param end The last position.
    */
    void selectRange(const int begin, const int end) const;
    /**
    * \brief Select a text between two positions.
    * \param beginLine The first line.
    * \param beginIndex The index on the first line.
    * \param endLine The last line.
    * \param endIndex The index on the last line.
    */
    void selectRange(const int beginLine, const int beginIndex,
                     const int endLine, const int endIndex) const;
    virtual void unselect() const;
    /**
    * \brief Set the background color of the selected text.
    * \param color The color.
    */
    void setSelectionBackgroundColor(QColor const &color) const;
    /**
    * \brief Set the foreground color of the selected text.
    * \param color The color.
    */
    void setSelectionForegroundColor(QColor const &color) const;

    virtual void setZoom(const int zoom) const;
    virtual void setZoomRange(const int range) const;
    virtual int getZoom() const;

    /**
    * \brief Switch between the insert and overwrite mode.
    * \param state True = overwrite mode, False = insert mode.
    */
    void setOverwriteMode(const bool state) const;
    /**
    * \brief Tell if we are in the overwrite mode.
    * \return True = overwrite mode, False = insert mode.
    */
    bool isOverwriteMode() const;

    /**
    * \brief Go to a line.
    * \param line The line number to go.
    */
    void gotoLine(const int line) const;
    /**
    * \brief Go to a position.
    * \param pos The position to go.
    */
    void gotoPos(const int pos) const;

    /**
    * \brief Transform the selection to lower case.
    */
    void lowerCaseSelection() const;
    /**
    * \brief Transform the selection to upper case.
    */
    void upperCaseSelection() const;

    /**
    * \brief Reserve the margin for symbols.
    * \param margin The margin to reserve.
    */
    void setMarginSymbol(const int margin) const;
    /**
    * \brief Reserve the margin for text.
    * \param margin The margin to reserve.
    */
    void setMarginText(const int margin) const;
    /**
    * \brief Reserve the margin for line numbers.
    * \param margin The margin to reserve.
    */
    void setMarginLineNumber(const int margin) const;
    /**
    * \brief Reserve the margin for text justified.
    * \param margin The margin to reserve.
    */
    void setMarginTextJustified(const int margin) const;
    /**
    * \brief Set the width for the margin.
    * \param margin The margin.
    * \param s The text that could be displayed in the margin.
    */
    void setMarginWidth(const int margin, QString const &s) const;
    /**
    * \brief Display the line numbers (if the margin line numbers is reserved).
    * \param margin The line number margin.
    * \param display True if we display, False to hide.
    */
    void displayLineNumbers(const int margin, const bool display) const;
    /**
    * \brief Set the margin sensitive to click.
    * \param margin The margin.
    * \param sens True if it is sensitive, False otherwise.
    */
    void setMarginSensitiveToClick(const int margin, const bool sens) const;

    // markers
    typedef QsciScintilla::MarkerSymbol WdMarkerSymbol;
    /**
    * \brief Define a marker number.
    * \param symbol The symbol to define.
    * \param nbMarker The marker number of the symbol.
    * \return The number of the marker defined.
    */
    int defineMarker(const WdMarkerSymbol symbol, const int nbMarker) const;
    /**
    * \brief Define a marker number.
    * \param ch The symbol to define.
    * \param nbMarker The marker number of the symbol.
    * \return The number of the marker defined.
    */
    int defineMarker(const char ch, const int nbMarker) const;
    /**
    * \brief Define a marker number.
    * \param pm The symbol to define.
    * \param nbMarker The marker number of the symbol.
    * \return The number of the marker defined.
    */
    int defineMarker(const QPixmap &pm, const int nbMarker) const;
    /**
    * \brief Define a marker number.
    * \param im The symbol to define.
    * \param nbMarker The marker number of the symbol.
    * \return The number of the marker defined.
    */
    int defineMarker(const QImage &im, const int nbMarker) const;
    /**
    * \brief Set the background color of the margin.
    * \param col The color.
    * \param nbMarker The number of the marker.
    */
    void setMarkerBackgroundColor(const QColor &col,
                                  const int nbMarker = -1) const;
    /**
    * \brief Set the foreground color of the margin.
    * \param col The color.
    * \param nbMarker The number of the marker.
    */
    void setMarkerForegroundColor(const QColor &col,
                                  const int nbMarker = -1) const;
    /**
    * \brief Set the margin mask of markers accepted on the margin.
    * \param margin The margin.
    * \param mask The mask.
    */
    void setMarginMarkerMask(const int margin, const int mask) const;
    /**
    * \brief Set the margin mask of markers accepted on the margin.
    * \param margin The margin.
    * \param markers The list of markers accepted.
    */
    void setMarginMarkerMask(const int margin, QList<int> const &markers) const;
    /**
    * \brief Set the margins background color.
    * \param color The color.
    */
    void setMarginsBackgroundColor(QColor const &color) const;
    /**
    * \brief Set the margins foreground color.
    * \param color The color.
    */
    void setMarginsForegroundColor(QColor const &color) const;
    /**
    * \brief Add a marker at a line.
    * \param line The line number.
    * \param marker The marker to add.
    */
    void addMarker(const int line, const int marker) const;
    /**
    * \brief Add several markers at a line.
    * \param line The line nnumber.
    * \param maskMarker The mask marker to set.
    */
    void addSetMarker(const int line, const int maskMarker) const;
    /**
    * \brief Add several markers at a line.
    * \param line The line number.
    * \param markers The list of markers to set.
    */
    void addSetMarker(const int line, QList<int> const &markers) const;
    /**
    * \brief Get the marker mask of the line.
    * \param line The line number.
    * \return The marker mask of the line.
    */
    unsigned int getMarkersMask(const int line) const;
    /**
    * \brief Get all the markers on the line.
    * \param line The line number.
    * \return The list of markers on the line.
    */
    QList<int> getMarkers(const int line) const;
    /**
    * \brief Delete the marker on the line.
    * \param line The line number.
    * \param marker The marker to delete.
    */
    void deleteMarker(const int line, const int marker) const;
    /**
    * \brief Delete all the markers in the document.
    * \param marker The marker to delete.
    */
    void deleteAllMarkers(const int marker) const;
    /**
    * \brief Delete all the markers on the line.
    * \param line The line number.
    */
    void deleteAllMarkersOnLine(const int line) const;
    /**
    * \brief Delete all the markers in the document.
    */
    void deleteAllMarkers() const;
    /**
    * \brief Get the marker mask for all the markers given.
    * \param markers The markers.
    * \return The mask that contains all the markers.
    */
    unsigned int getMaskFromMarkers(QList<int> const &markers) const;
    /**
    * \brief Get the markers from a mask.
    * \param mask The mask.
    * \return The list of markers contained in the mask.
    */
    QList<int> getMarkersFromMask(const unsigned int mask) const;
    /**
    * \brief Find the next marker from the line.
    * \param line The line number from where starting.
    * \param marker The marker to find.
    * \return The line number where we found the marker, or -1.
    */
    int findNextMarker(const int line, const int marker) const;
    /**
    * \brief Find the next marker from the line.
    * \param line The line number from where starting.
    * \param mask The mask with all the markers to find.
    * \return The line number where we found the marker, or -1.
    */
    int findNextMarkers(const int line, const unsigned int mask) const;
    /**
    * \brief Find the next marker from the line.
    * \param line The line number from where starting.
    * \param markers The markers to find.
    * \return The line number where we found the marker, or -1.
    */
    int findNextMarkers(const int line, QList<int> const &markers) const;
    /**
    * \brief Find the previous marker from the line.
    * \param line The line number from where starting.
    * \param mask The mask with all markers to find.
    * \return The line number where we found the marker, or -1.
    */
    int findPreviousMarkers(const int line, const unsigned int mask) const;
    /**
    * \brief Find the previous marker from the line.
    * \param line The line number from where starting.
    * \param markers The markers to find.
    * \return The line number where we found the marker, or -1.
    */
    int findPreviousMarkers(const int line, QList<int> const &markers) const;
    /**
    * \brief Find the previous marker from the line.
    * \param line The line number from where starting.
    * \param marker The marker to find.
    * \return The line number where we found the marker, or -1.
    */
    int findPreviousMarker(const int line, const int marker) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param fromLine The line where starting.
    * \param mask The mask of the markers we want.
    * \return The list of lines where the markers are.
    */
    QList<int> getLinesWithMarkers(const int fromLine,
                                   const unsigned int mask) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param fromLine The line where starting.
    * \param markers The markers we want.
    * \return The list of lines where the markers are.
    */
    QList<int> getLinesWithMarkers(const int fromLine,
                                   QList<int> const &markers) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param fromLine The line where starting.
    * \param marker The marker we want.
    * \return The list of lines where the marker is.
    */
    QList<int> getLinesWithMarker(const int fromLine, const int marker) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param fromLine The line where starting.
    * \param toLine The line where we stop to search.
    * \param mask The mask of the markers we want.
    * \return The list of lines where the markers are.
    */
    QList<int> getLinesWithMarkers(const int fromLine, const int toLine,
                                   const unsigned int mask) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param fromLine The line where starting.
    * \param toLine The line where we stop to search.
    * \param markers The markers we want.
    * \return The list of lines where the markers are.
    */
    QList<int> getLinesWithMarkers(const int fromLine, const int toLine,
                                   QList<int> const &markers) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param fromLine The line where starting.
    * \param toLine The line where we stop to search.
    * \param marker The marker we want.
    * \return The list of lines where the marker is.
    */
    QList<int> getLinesWithMarker(const int fromLine, const int toLine,
                                  const int marker) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param mask The mask of the markers we want.
    * \return The list of lines where the markers are.
    */
    QList<int> getLinesWithMarkers(const unsigned int mask) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param markers The markers we want.
    * \return The list of lines where the markers are.
    */
    QList<int> getLinesWithMarkers(QList<int> const &markers) const;
    /**
    * \brief Get the lines that contains the marker.
    * \param marker The marker we want.
    * \return The list of lines where the marker is.
    */
    QList<int> getLinesWithMarker(const int marker) const;

    // styles: colors, font, ...
    /**
    * \typedef An indicator allows to underline lines for example, for warnings
    * display, etc...
    * See http://www.scintilla.org/ScintillaDoc.html#Indicators
    */
    typedef QsciScintilla::IndicatorStyle WdIndicator;
    /**
    * \brief Start to stylise characters from the position.
    * \param position The position to start.
    * \param mask The mask of styles accepted.
    */
    void startStyling(const int position, const int mask = 31) const;
    /**
    * \brief Set the style from the startStyling() to length.
    * \param length The number of characters to stylize.
    * \param style The style to apply.
    */
    void setStyling(const int length, const int style) const;
    // TODO: range text style (bold, italic, ...)
    /**
    * \brief Set the foreground color for a style.
    * \param numberStyle The number of the style.
    * \param color The color.
    */
    void setForegroundColor(const int numberStyle, QColor const &color) const;
    /**
    * \brief Set the background color for a style.
    * \param numberStyle The number of the style.
    * \param color The color.
    */
    void setBackgroundColor(const int numberStyle, QColor const &color) const;
    /**
    * \brief Apply a style from a position.
    * \param pos The position to start.
    * \param length The number of characters to stylize.
    * \param numberStyle The style to apply.
    */
    void applyStyle(const int pos, const int length,
                    const int numberStyle) const;
    /**
    * \brief Apply a style from a position.
    * \param lineStart The first line to start.
    * \param indexStart The index on the first line.
    * \param lineEnd The last line to apply.
    * \param indexEnd The index on the last line.
    * \param numberStyle The style to apply.
    */
    void applyStyle(const int lineStart, const int indexStart,
                    const int lineEnd, const int indexEnd,
                    const int numberStyle) const;
    /**
    * \brief Apply a style from a position.
    * \param lineStart The first line to start.
    * \param indexStart The index on the first line.
    * \param length The number of characters to stylize.
    * \param numberStyle The style to apply.
    */
    void applyStyle(const int lineStart, const int indexStart, const int length,
                    const int numberStyle) const;

    /**
    * \brief Define an indicator number.
    * \param style The indicator.
    * \param number The number we want.
    * \return The number of the indicator defined.
    */
    int defineIndicator(const WdIndicator style, const int number) const;
    /**
    * \brief Set an indicator on a line.
    * \param lineStart The first line to start.
    * \param indexStart The index on the first line.
    * \param lineEnd The last line.
    * \param indexEnd The index on the last line.
    * \param numberIndicator The indicator to set.
    */
    void setIndicator(const int lineStart, const int indexStart,
                      const int lineEnd, const int indexEnd,
                      const int numberIndicator) const;
    /**
    * \brief Clear one or several indicator(s) on a line.
    * \param lineStart The first line to start.
    * \param indexStart The index on the first line.
    * \param lineEnd The last line.
    * \param indexEnd The index on the last line.
    * \param number The indicator to set (-1 = all by default).
    */
    void clearIndicator(const int lineStart, const int indexStart,
                        const int lineEnd, const int indexEnd,
                        const int number = -1) const;
    /**
    * \brief Set the indicator color.
    * \param color The color.
    * \param number The indicator number.
    */
    void setIndicatorColor(QColor const &color, const int number) const;

    /**
    * \brief Set the document background color.
    * \param color The color.
    */
    void setDocumentBackground(QColor const &color) const;
    /**
    * \brief Get the document background color.
    * *return The background color of the document.
    */
    QColor getDocumentBackground() const;
    /**
    * \brief Set the document foreground color.
    * \param color The color.
    */
    void setDocumentForeground(QColor const &color) const;
    /**
    * \brief Get the document foreground color.
    * *return The foreground color of the document.
    */
    QColor getDocumentForeground() const;

    /**
    * \brief The folding style on the margin (cross, etc...).
    */
    typedef QsciScintilla::FoldStyle WdFoldingStyle;
    /**
    * \brief Set the folding style in the margin.
    * \param style The folding style.
    * \param margin The folding margin.
    */
    void setFolding(const WdFoldingStyle style, const int margin = 2) const;
    /**
    * \brief Get the folding level for the line.
    * \param line The line number.
    * \return The folding level.
    */
    int getFoldLevel(const int line) const;
    /**
    * \brief Get the folding level for the line
    * (with a mask for more information).
    * \param line The line number.
    * \return The folding level with the mask.
    */
    int getFoldLevelWithMask(const int line) const;
    /**
    * \brief Tell if the line contains the beginning of a folding bloc.
    * \param line The line number.
    * \return True if it is.
    */
    bool isFoldHeader(const int line) const;
    /**
    * \brief Tell if the line folded is a blank line.
    * \param line The line number.
    * \return True if it is.
    */
    bool isFoldWhiteLine(const int line) const;
    /**
    * \brief Set the beginning of a folding.
    * \param line The line number.
    * \param st True if we set, False if we unset.
    */
    void setFoldHeader(const int line, const bool st) const;
    /**
    * \brief Set the beginning of a white line in the folding.
    * \param line The line number.
    * \param st True if we set, False if we unset.
    */
    void setFoldWhiteLine(const int line, const bool st) const;
    /**
    * \brief Set the fold level of the line.
    * \param line The line number.
    * \param level The level to set.
    */
    void setFoldLevel(const int line, const int level) const;
    /**
    * \brief Set the fold level of the line with the mask (header and whiteline
    * mask).
    * \param line The line number.
    * \param level The level to set.
    */
    void setFoldLevelWithMask(const int line, const int level) const;
    // TODO: constexpr ?
    /**
    * \brief Get the fold base folding level.
    * \return The base folding level.
    */
    int getFoldBaseLevel() const;
    /**
    * \brief Get the maximum folding level.
    * \return The maximum folding level.
    */
    int getFoldMaxLevel() const;
    /**
    * \brief Increase the fold level of the line.
    * \param line The line number.
    * \param inc The number of level to increase.
    */
    void increaseFoldLevel(const int line, const int inc) const;
    /**
    * \brief Decrease the fold level of the line.
    * \param line The line number.
    * \param inc The number of level to decrease.
    */
    void decreaseFoldLevel(const int line, const int inc) const;
    /**
    * \brief Fold a block of line with an appropriate level.
    * \param lineStart The first line.
    * \param lineEnd The last line.
    */
    void setFoldPart(const int lineStart, const int lineEnd) const;
    /**
    * \brief Fold a block of line with a specific level.
    * \param lineStart The first line.
    * \param lineEnd The last line.
    * \param level The level to set.
    */
    void setFoldPart(const int lineStart, const int lineEnd,
                     const int level) const;
    /**
    * \brief Fold a block of line with a specific level (with the mask).
    * \param lineStart The first line.
    * \param lineEnd The last line.
    * \param level The level to set.
    */
    void setFoldPartWithMask(const int lineStart, const int lineEnd,
                             const int level) const;
    /**
    * \brief Remove the fold block.
    * \param lineStart The first line.
    * \param lineEnd The last line.
    */
    void removeFoldPart(const int lineStart, const int lineEnd) const;
    /**
    * \brief Remove the fold block.
    * \param line The line number concerned.
    */
    void removeFoldPart(const int line) const;
    /**
    * \brief Get the line number of the fold parent of the line.
    * \param line The line number.
    * \return The line number of the parent.
    */
    int getFoldParent(const int line) const;
    /**
    * \brief Get the line number of the fold last child of the line.
    * \param line The line number.
    * \return The line number of the last child.
    */
    int getFoldLastChild(const int line) const;

    /**
    * \brief The braces matching options.
    */
    typedef QsciScintilla::BraceMatch WdBraceMatch;
    /**
    * \brief Set the background color of the matched braces.
    * \param color The color.
    */
    void setMatchedBraceBackgroundColor(QColor const &color) const;
    /**
    * \brief Set the foreground color of the matched braces.
    * \param color The color.
    */
    void setMatchedBraceForegroundColor(QColor const &color) const;
    /**
    * \brief Set the background color of the unmatched braces.
    * \param col The color.
    */
    void setUnmatchedBraceBackgroundColor(QColor const &col) const;
    /**
    * \brief Set the foreground color of the unmatched braces.
    * \param col The color.
    */
    void setUnmatchedBraceForegroundColor(QColor const &col) const;
    /**
    * \brief Set the bracing matching option.
    * \param match The matching option.
    */
    void setBracingMatching(const WdBraceMatch match) const;
    /**
    * \brief Select the matching brace.
    */
    void selectToMatchingBrace() const;
    /**
    * \brief Move to the matching brace.
    */
    void moveToMatchingBrace() const;

    /**
    * \brief Find The first element and move the cursor to the
    * beginning of the matched expression.
    * \param expr The string expression to find.
    * \param isRegexp True if the expression is a regexp.
    * \param isCaseSensitive True if the expression is case sensitive.
    * \param matchWordOnly True if we match only the word.
    * \param searchWrapsEnd True if we go back to the beginning
    * of the document after the end.
    * \param searchForward True if we search forward, otherwise search backward.
    * \param fromLine The start line.
    * \param fromIndex The index of the start line.
    * \param unfoldTextFound If the text found is folded, we unfold the block.
    * \param posixRegexp Is the regexp POSIX ?
    * \return True if we found, False otherwise.
    */
    bool findFirst(QString const &expr, const bool isRegexp,
                   const bool isCaseSensitive, const bool matchWordOnly,
                   const bool searchWrapsEnd, const bool searchForward = true,
                   const int fromLine = -1, const int fromIndex = -1,
                   const bool unfoldTextFound = true,
                   const bool posixRegexp = false) const;
    /**
    * \brief Find The first element in the selection and move the cursor to the
    * beginning of the matched expression.
    * \param expr The string expression to find.
    * \param isRegexp True if the expression is a regexp.
    * \param isCaseSensitive True if the expression is case sensitive.
    * \param matchWordOnly True if we match only the word.
    * of the document after the end.
    * \param searchForward True if we search forward, otherwise search backward.
    * \param unfoldTextFound If the text found is folded, we unfold the block.
    * \param posixRegexp Is the regexp POSIX ?
    * \return True if we found, False otherwise.
    */
    bool findFirstInSelection(QString const &expr, const bool isRegexp,
                              const bool isCaseSensitive,
                              const bool matchWordOnly,
                              const bool searchForward = true,
                              const bool unfoldTextFound = true,
                              const bool posixRegexp = false) const;
    /**
    * \brief Find the next expression after a call to findFirst()
    * or findFirstInSelection().
    * \return True if we found.
    */
    bool findNext() const;
    /**
    * \brief Replace the selected text after a call to findFirst(),
    findFirstInSelection() or findNext().
    * \param text The text to replace.
    */
    void replace(QString const &text) const;
    /**
    * \brief Replace the selected text.
    * \param text The text to replace.
    */
    void replaceSelectedText(QString const &text) const;

    /**
    * \brief Set if the tab indents a line.
    * \param indent True if a tabulation indents the line.
    */
    void setTabsIndentLine(const bool indent) const;
    /**
    * \brief Tell if the tab indents a line.
    * \return True if a tabulation indents the line.
    */
    bool getTabsIndentLine() const;
    /**
    * \brief Set if the backspace unindents a line.
    * \param indent True if a backspace unindents a line.
    */
    void setBackspaceUnindentLine(const bool indent) const;
    /**
    * \brief Tell if the backspace unindents a line.
    * \return True if a backspace unindents a line.
    */
    bool getBackspaceUnindentLine() const;
    /**
    * \brief Indent the line.
    * \param line The line number to indent.
    */
    void indentLine(const int line) const;
    /**
    * \brief Unindent the line.
    * \param line The line number to unindent.
    */
    void unindentLine(const int line) const;
    /**
    * \brief Set the auto indentation of lines.
    * \param indent True if we indent the lines automatically.
    */
    void setAutoIndent(const bool indent) const;
    /**
    * \brief Tell if the auto indentation mode is enabled.
    * \return True if the auto indentation mode is enabled.
    */
    bool getAutoIndent() const;
    /**
    * \brief Set the indentation line width.
    * \param line The line number.
    * \param width The width of the indentation.
    */
    void setIndentationLine(const int line, const int width) const;
    /**
    * \brief Set the indentation by using tabulations.
    * \param useTabs True if we use tabulations, otherwise we use spaces.
    */
    void setIndentationUseTabs(const bool useTabs) const;
    /**
    * \brief Set the indentation width.
    * \param width The indentation width.
    */
    void setIndentationWidth(const int width) const;
    /**
    * \brief Get the indentation width.
    * \return The indentation width.
    */
    int getIndentationWidth() const;
    /**
    * \brief Set the tabulation width.
    * \param width The tabulation width.
    */
    void setTabWidth(const int width) const;
    /**
    * \brief Get the width of a tabulation.
    * \return The width of a tabulation.
    */
    int getTabWidth() const;

    enum WdIndentGuide {
        NONE = QsciScintilla::SC_IV_NONE,
        REAL = QsciScintilla::SC_IV_REAL,
        LOOKFORWARD = QsciScintilla::SC_IV_LOOKFORWARD,
        LOOKBOTH = QsciScintilla::SC_IV_LOOKBOTH
    };
    void setIndentationGuide(const WdIndentGuide guide) const;

    /**
    * \brief Get the contextual menu of the document.
    * \return The contextual menu.
    */
    QMenu *getContextualMenu() const;
    /**
    * \brief Get the pixel coordinates at the cursor position.
    * \return THe pixel coordinates at the cursor position.
    */
    QPoint getCursorPixelCoord() const;

    void showCalltip(const int pixel_x, const int pixel_y) const;
    void showCalltipCurrentPosition() const;
    void setCallTipContent(std::vector<CompletionData> &content);

    virtual bool hasDocumentFocus() const;

    typedef QsciScintilla::WrapMode WdWrapMode;
    typedef QsciScintilla::WrapVisualFlag WdWrapVisualFlag;
    typedef QsciScintilla::WrapIndentMode WdWrapIndentMode;

    void setWrapMode(const WdWrapMode mode) const;
    void setWrapMode(const WdWrapMode mode, const WdWrapVisualFlag start,
                     const WdWrapVisualFlag end) const;
    WdWrapMode getWrapMode() const;
    int getPosition(const int line, const int index) const;
    void setWrapIndentMode(const WdWrapIndentMode mode) const;
    WdWrapIndentMode getWrapIndentMode() const;
    void setWrapVisualFlag(const WdWrapVisualFlag start,
                           const WdWrapVisualFlag end,
                           const int indent = 0) const;

    void clearText() const;
    virtual bool isReadOnly() const;
    virtual void setReadOnly(const bool s) const;
    // TODO: SCI_CLEARDOCUMENTSTYLE

private slots:
    /**
    * \brief Called when a margin is clicked.
    * \param margin The margin clicked.
    * \param line The line number.
    * \param state The state of the keyboard.
    */
    void slot_marginClicked(int margin, int line, Qt::KeyboardModifiers state);
    /**
    * \brief Called when the cursor position changes.
    * \param line The line number.
    * \param index The index of the line.
    */
    void slot_cursorPositionChanged(int line, int index);
    /**
    * \brief Called when the text is modified.
    * \param pos The position.
    * \param modificationType The type of modification (insertion, deletion...).
    * \param text The text modified.
    * \param length The length of the text.
    * \param linesAdded The number of lines added.
    * \param line The first line modified.
    * \param foldLevelNow Unused.
    * \param foldLevelPrev Unused
    * \param token Unused.
    * \param annotationLinesAdded The annotations added on the line.
    */
    void slot_textModified(int pos, int modificationType, const char *text,
                           int length, int linesAdded, int line,
                           int foldLeveNow, int foldLevelPrev, int token,
                           int annotationLinesAdded);
    /**
    * \brief Called when a zoom action is performed.
    */
    void slot_zoom();

    void slot_callTip_entryChosen(QString const &text);
    void slot_lostFocus();
    void slot_scrollbarMoved(int);
    void slot_getFocus();
signals:
    /**
    * \brief Thrown when the margin is clicked (must be click sensitive).
    * \param margin The margin clicked.
    * \param line The line number.
    */
    void marginClicked(const int margin, const int line);
    /**
    * \brief Thrown when the cursor position has changed.
    * \param line The line number.
    * \param index The index on the line.
    */
    void cursorPositionChanged(const int line, const int index);
    /**
    * \brief Thrown when a text is deleted.
    * \param offset Offset where the text is deleted.
    * \param text The text deleted.
    */
    void textDeleted(const int atOffset, QString const &text);
    /**
    * \brief Thrown when a text is inserted.
    * \param offset Offset where the text is inserted.
    * \param text The text inserted.
    */
    void textInserted(const int offset, QString const &text);
    /**
    * \brief Thrown when the zoom has changed.
    * \param currentZoom The current zooom level.
    */
    void zoomChanged(const int currentZoom);

    void documentHasFocus();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    /*Q_DECL_CONSTEXPR*/
    int getPixelVerticalCallTip(const int y) const;
    /*Q_DECL_CONSTEXPR*/ int getPixelHorizontalCallTip(const int y) const;

private:
    Q_DISABLE_COPY(WdScintilla);

    ScintillaPrivate *_scintilla;  /**< The engine renderer. */
    QString _filename;             /**< The filename. */
    QGridLayout *_layout;          /**< The layout. */
    QString _textCopied;           /**< The text copied in the clipboard. */
    QString _textGoingToBeDeleted; /**< The text that will be deleted.
                                   Used only for the signal textDeleted() */
    int _lengthTextDeleted; /**< The length of the text that will be deleted. */

    static const int CALLTIP_HEIGHT_SHIFT;
};

#endif
