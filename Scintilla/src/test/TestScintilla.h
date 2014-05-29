
#ifndef __TEST_SCINTILLA_H__
#define __TEST_SCINTILLA_H__

// allows to perform a lot of unit tests about widev functionnalities.

// qt classes
#include <QObject>
#include <QMainWindow>

#include "TextEditor/WdScintilla.h"

class QString;

class TestScintilla : QObject {
    Q_OBJECT
public:
    TestScintilla(QMainWindow *window, WdScintilla *scintilla);

public slots:
    // scintilla
    void moveCaretLeftFirstSplit();
    void moveCaretRightFirstSplit();
    void moveCaretRightExtendFirstSplit();
    void moveCaretLeftExtendFirstSplit();

    void deleteSelection();
    void deleteAll();
    void deleteBack();
    void deleteBackNoLine();
    void deleteLineLeft();
    void deleteLineRight();
    void deleteLeftWord();
    void deleteRightWord();
    void deleteCurrentLine();

    void appendTextTest();
    void addTextTest();

    void copySelection();
    void copyRange();
    void copyToClipboard();
    void paste();
    void cutSelection();

    void getPositonInfo();
    void getLineInfo();
    void getSelectionInfo();

    void zoom();

    void displayEOL();
    void convertEOL();

    void switchOverwriteMode();
    void gotoLine();
    void gotoPos();

    void lowerCaseSelection();
    void upperCaseSelection();

    void markersTest();

    void indicatorTest();
    void colorTest();

    void foldTest();

private:
    Q_DISABLE_COPY(TestScintilla);

    QMainWindow *_window;
    WdScintilla *_scintilla;
};

#endif
