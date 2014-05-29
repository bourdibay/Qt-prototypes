//
//   TestScintilla.cpp
//   Widev
//
//   Created by bourdi_b on 13 nov. 2013 14:50:26
//
//
//

////////////////////////
// This class is only for debug purpose.
/////////////////////////

#include <iostream>

// qt classes
#include <QStringList>
#include <QDebug>
#include <QLabel>
#include <QString>
#include <QFileDialog>
#include <QInputDialog>
#include <QGridLayout>

// test dev widev
#include "TestScintilla.h"

// widevGraphic
//   menu
#include "WdMenuBar.h"
#include "WdMenu.h"
#include "WdAction.h"

static int NB_COMPONENT_NAME = 2;

TestScintilla::TestScintilla(QMainWindow *window, WdScintilla *scintilla)
    : _window(window), _scintilla(scintilla)
{
    WdMenuBar *menuBar = new WdMenuBar(_window);

    menuBar->addWdMenu("DEV_TEST", tr("Tests of scintilla"));

    ///////// TEST SCINTILLA /////////
    menuBar->addWdMenu("TEST_SCINTILLA",
        QStringList() << "DEV_TEST",
        tr("scintilla"));
    menuBar->addWdMenu("APPEND_SCINTILLA",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Append text"));
    menuBar->addWdMenu("ADD_SCINTILLA",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Add text"));
    menuBar->addWdMenu("DELETE_SCINTILLA",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Delete text"));
    menuBar->addWdMenu("COPY_SCINTILLA",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Copy text"));
    menuBar->addWdMenu("MOVE_CARET_SCINTILLA",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Move caret"));
    menuBar->addWdMenu("MARKERS_MARGINS_SCINTILLA",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Margins/markers"));
    menuBar->addWdMenu("STYLE_SCINTILLA",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Style"));
    menuBar->addWdMenu("FOLDING_SCINTILLA",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Folding"));

    // Move caret
    menuBar->addWdAction("TEST_SCINTILLA_MOVE_CARET_LEFT",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "MOVE_CARET_SCINTILLA",
        tr("Move the caret to one char on the left."),
        this, SLOT(moveCaretLeftFirstSplit()));
    menuBar->addWdAction("TEST_SCINTILLA_MOVE_CARET_LEFT_EXTEND",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "MOVE_CARET_SCINTILLA",
        tr("Move the caret to one char on the left extend."),
        this, SLOT(moveCaretLeftExtendFirstSplit()));
    menuBar->addWdAction("TEST_SCINTILLA_MOVE_CARET_RIGHT",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "MOVE_CARET_SCINTILLA",
        tr("Move the caret to one char on the right."),
        this, SLOT(moveCaretRightFirstSplit()));
    menuBar->addWdAction("TEST_SCINTILLA_MOVE_CARET_RIGHT_EXTEND_FIRST_SPLIT",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "MOVE_CARET_SCINTILLA",
        tr("Move the caret to one char on the right extend."),
        this, SLOT(moveCaretRightExtendFirstSplit()));
    menuBar->addWdAction("TEST_SCINTILLA_GOTO_LINE",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "MOVE_CARET_SCINTILLA",
        tr("Goto line in the current document."),
        this, SLOT(gotoLine()));
    menuBar->addWdAction("TEST_SCINTILLA_GOTO_POS",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "MOVE_CARET_SCINTILLA",
        tr("Goto position in the current document."),
        this, SLOT(gotoPos()));

    // Delete text
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_SELECTION",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete the text selected."),
        this, SLOT(deleteSelection()));
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_ALL",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete all the text."),
        this, SLOT(deleteAll()));
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_BACK",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete back."),
        this, SLOT(deleteBack()));
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_BACK_NOLINE",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete back on the line."),
        this, SLOT(deleteBackNoLine()));
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_LINE_LEFT",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete all the left of the line."),
        this, SLOT(deleteLineLeft()));
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_LINE_RIGHT",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete all the right of the line."),
        this, SLOT(deleteLineRight()));
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_WORD_LEFT",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete the left word."),
        this, SLOT(deleteLeftWord()));
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_WORD_RIGHT",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete the right word."),
        this, SLOT(deleteRightWord()));
    menuBar->addWdAction("TEST_SCINTILLA_DELETE_CURR_LINE",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "DELETE_SCINTILLA",
        tr("Delete the current line."),
        this, SLOT(deleteCurrentLine()));

    // Append text
    menuBar->addWdAction("TEST_SCINTILLA_APPEND_TEXT_TEST",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "APPEND_SCINTILLA",
        tr("Launch the test that append text in the current document."),
        this, SLOT(appendTextTest()));

    // Add text
    menuBar->addWdAction("TEST_SCINTILLA_ADD_TEXT_TEST",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "ADD_SCINTILLA",
        tr("Launch the test that add text after the cursor in the current document."),
        this, SLOT(addTextTest()));

    // Copy text
    menuBar->addWdAction("TEST_SCINTILLA_COPY_SELECTION",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "COPY_SCINTILLA",
        tr("Copy the selection in the current document."),
        this, SLOT(copySelection()));
    menuBar->addWdAction("TEST_SCINTILLA_COPY_RANGE",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "COPY_SCINTILLA",
        tr("Copy a range in the current document."),
        this, SLOT(copyRange()));
    menuBar->addWdAction("TEST_SCINTILLA_COPY_CLIPBOARD",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "COPY_SCINTILLA",
        tr("Copy a text to the clipboard in the current document."),
        this, SLOT(copyToClipboard()));
    menuBar->addWdAction("TEST_SCINTILLA_PASTE",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "COPY_SCINTILLA",
        tr("Paste in the current document."),
        this, SLOT(paste()));
    menuBar->addWdAction("TEST_SCINTILLA_CUT_SELECTION",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "COPY_SCINTILLA",
        tr("Cut the selection in the current document."),
        this, SLOT(cutSelection()));

    // Margins/markers
    menuBar->addWdAction("TEST_SCINTILLA_TEST_MARGINS_MARKERS",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "MARKERS_MARGINS_SCINTILLA",
        tr("Launch tests of margins and markers in the current document."),
        this, SLOT(markersTest()));

    // Style
    menuBar->addWdAction("TEST_SCINTILLA_TEST_STYLE_INDIC",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "STYLE_SCINTILLA",
        tr("Launch test of indicators in the current document."),
        this, SLOT(indicatorTest()));
    menuBar->addWdAction("TEST_SCINTILLA_TEST_COLOR",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "STYLE_SCINTILLA",
        tr("Launch test of colors in the current document."),
        this, SLOT(colorTest()));

    // Folding
    menuBar->addWdAction("TEST_SCINTILLA_TEST_FOLDING",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA" << "FOLDING_SCINTILLA",
        tr("Launch test of folding in the current document."),
        this, SLOT(foldTest()));

    // OTHER
    menuBar->addWdAction("TEST_SCINTILLA_GET_POSITION_INFO",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Get the positon info in the current document."),
        this, SLOT(getPositonInfo()));
    menuBar->addWdAction("TEST_SCINTILLA_GET_SELECTION_INFO",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Get the selection info in the current document."),
        this, SLOT(getSelectionInfo()));
    menuBar->addWdAction("TEST_SCINTILLA_ZOOM",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Zoom the current document."),
        this, SLOT(zoom()));
    menuBar->addWdAction("TEST_SCINTILLA_DISPLAY_EOL",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Display end-of-line in the current document."),
        this, SLOT(displayEOL()));
    menuBar->addWdAction("TEST_SCINTILLA_CONVERT_EOL",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Convert end-of-line in the current document."),
        this, SLOT(convertEOL()));
    menuBar->addWdAction("TEST_SCINTILLA_OVERWRITE_INSERT_MODE",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Switch between insert and overwrite mode in the current document."),
        this, SLOT(switchOverwriteMode()));
    menuBar->addWdAction("TEST_SCINTILLA_GET_LINE_INFO",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Get a line information from the document opened"),
        this, SLOT(getLineInfo()));
    menuBar->addWdAction("TEST_SCINTILLA_LOWER_CASE_SELECTION",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Lower the selection from the document opened"),
        this, SLOT(lowerCaseSelection()));
    menuBar->addWdAction("TEST_SCINTILLA_UPPER_CASE_SELECTION",
        QStringList() << "DEV_TEST" << "TEST_SCINTILLA",
        tr("Upper the selection from the document opened"),
        this, SLOT(upperCaseSelection()));

    _window->setMenuBar(menuBar);
}

//////////////////////////////////////////////////
/////// SCINTILLA
//////////////////////////////////////////////////

void TestScintilla::moveCaretLeftFirstSplit() {
    _scintilla->moveCaretCharLeft();
}

void TestScintilla::moveCaretLeftExtendFirstSplit() {
    _scintilla->moveCaretCharLeftExtend();
}

void TestScintilla::moveCaretRightFirstSplit() {
    _scintilla->moveCaretCharRight();
}

void TestScintilla::moveCaretRightExtendFirstSplit() {
    _scintilla->moveCaretCharRightExtend();
}

void TestScintilla::deleteSelection() {
    _scintilla->deleteSelection();
}

void TestScintilla::deleteAll() {
    _scintilla->deleteAll();
}

void TestScintilla::deleteBack() {
    _scintilla->deleteBack();
}

void TestScintilla::deleteBackNoLine() {
    _scintilla->deleteBackNoLine();
}

void TestScintilla::deleteLineLeft() {
    _scintilla->deleteLineLeft();
}

void TestScintilla::deleteLineRight() {
    _scintilla->deleteLineRight();
}

void TestScintilla::deleteLeftWord() {
    _scintilla->deleteLeftWord();
}

void TestScintilla::deleteRightWord() {
    _scintilla->deleteRightWord();
}

void TestScintilla::deleteCurrentLine() {
    _scintilla->deleteCurrentLine();
}


void TestScintilla::appendTextTest() {
    _scintilla->appendText("\n-----------TEST APPEND TEXT------------\n");
    _scintilla->appendText("Test qstring:");
    _scintilla->appendText(QString("This is a test from a QString and utf8 characters"));
    _scintilla->appendText("\n");
    _scintilla->appendText("Test qbytearray:");
    _scintilla->appendText(QByteArray("This is a qbytearray"));
    _scintilla->appendText("\n");
    _scintilla->appendText("Test char*:");
    _scintilla->appendText(reinterpret_cast<const char *>("I'm there"));
    _scintilla->appendText("\n");
    _scintilla->appendText("Test char* + length 5:");
    _scintilla->appendText(reinterpret_cast<const char *>("I'm there"), 5);
    _scintilla->appendText("\n");
    _scintilla->appendText("Test char * with special char:");
    _scintilla->appendText("¡ Puedo escribir español aquí ! ¿ Estoy aquí ? J'écris français.");
    _scintilla->appendText("\n");
    _scintilla->appendText("Test char * with special char + length 13:");
    _scintilla->appendText("¡ Puedo escribir español aquí ! ¿ Estoy aquí ?", 13);
    _scintilla->appendText("\n");
    _scintilla->appendText("Test std string:");
    _scintilla->appendText(std::string("Jag talar lite svenska nu !"));
    _scintilla->appendText("\n");
    _scintilla->appendText("Test std string with special char:");
    _scintilla->appendText(
        std::string("Jag bor i Linköping, och på lunchen brukar jag äta köttbullar :)!"));
    _scintilla->appendText("\n");
}

void TestScintilla::addTextTest() {
    _scintilla->addText("\n-----------TEST ADD TEXT------------\n");
    _scintilla->addText("\n");
    _scintilla->addText("Test qstring:");
    _scintilla->addText(QString("This is a test from a QString and utf8 characters"));
    _scintilla->addText("\n");
    _scintilla->addText("Test qbytearray:");
    _scintilla->addText(QByteArray("This is a qbytearray"));
    _scintilla->addText("\n");
    _scintilla->addText("Test char*:");
    _scintilla->addText(reinterpret_cast<const char *>("I'm there"));
    _scintilla->addText("\n");
    _scintilla->addText("Test char* + length 5:");
    _scintilla->addText(reinterpret_cast<const char *>("I'm there"), 5);
    _scintilla->addText("\n");
    _scintilla->addText("Test char* with special char:");
    _scintilla->addText("¡ Puedo escribir español aquí ! ¿ Estoy aquí ? J'écris français.");
    _scintilla->addText("\n");
    _scintilla->addText("Test char* with special char + length 13:");
    _scintilla->addText("¡ Puedo escribir español aquí ! ¿ Estoy aquí ?", 13);
    _scintilla->addText("\n");
    _scintilla->addText("Test std string:");
    _scintilla->addText(std::string("Jag talar lite svenska nu !"));
    _scintilla->addText("\n");
    _scintilla->addText("Test std string with special char:");
    _scintilla->addText(
        std::string("Jag bor i Linköping, och på lunchen brukar jag äta köttbullar :)!"));
    _scintilla->addText("\n");
    _scintilla->addText("Bonus test:");
    _scintilla->addText("hého à tous, é(accent aiguë)à(a accent grace)ë(e tréma)è(accent grave) je suis ici");
    _scintilla->addText("\n");
}

void TestScintilla::getLineInfo() {
    const int line = QInputDialog::getInt(_window, "Get line number",
        "Get the line number:");
    const int length = _scintilla->getLineLength(line);
    const QString str = _scintilla->getLine(line);
    qDebug() << "GetLine: length=" << length << " Content=" << str;
}

void TestScintilla::getSelectionInfo() {
    const int begin = _scintilla->getSelectionBegin();
    const int end = _scintilla->getSelectionEnd();
    const QString text = _scintilla->getSelectedText();
    qDebug() << "Text selected=" << text << " begin=" << begin << " end=" << end;
}

void TestScintilla::getPositonInfo() {
    const int currPos = _scintilla->getCurrentPosition();
    const int line = _scintilla->getLineNumber(currPos);
    const int begin = _scintilla->getBeginPositionLine(line);
    const int end = _scintilla->getEndPositionLine(line);
    const QString text = _scintilla->getText(begin, end);
    const QString currText = _scintilla->getCurrentLine(10);
    const QString currFullText = _scintilla->getCurrentLine(-1);
    qDebug() << "currpos=" << currPos
        << "line=" << line << "beginLine=" << begin
        << "endLine=" << end << "getText between=" << text
        << "10 first char=" << currText
        << "full text=" << currFullText;
}


void TestScintilla::copySelection() {
    _scintilla->copySelection();
    const QString text = _scintilla->getTextCopied();
    qDebug() << "Text copied=" << text;
}

void TestScintilla::copyRange() {
    const int begin = QInputDialog::getInt(_window, "Get begin", "Get begin range:");
    const int end = QInputDialog::getInt(_window, "Get end", "Get end range:");
    _scintilla->copy(begin, end);
    QString text = _scintilla->getTextCopied();
    qDebug() << "Text copied=" << text;
    _scintilla->copy(0, 2, 2, 2);
    text = _scintilla->getTextCopied();
    qDebug() << "Text copied line0 idx2 to line2 idx2=" << text;
}

void TestScintilla::copyToClipboard() {
    _scintilla->copyToClipboard(5, "coucou");
    QString text = _scintilla->getTextCopied();
    qDebug() << "Text copied=" << text;
    _scintilla->copyToClipboard(11, "hého à tous");
    text = _scintilla->getTextCopied();
    qDebug() << "Text copied=" << text;
    _scintilla->copyToClipboard(QString("je suis ici"));
    text = _scintilla->getTextCopied();
    qDebug() << "Text copied=" << text;
    const char *str =
        "hého à tous, é(accent aiguë)à(a accent grace)ë(e tréma)è(accent grave) je suis ici";
    const size_t len = strlen(str);
    _scintilla->copyToClipboard(len, str);
    text = _scintilla->getTextCopied();
    qDebug() << "Text copied=" << text;
}

void TestScintilla::paste() {
    _scintilla->paste();
}

void TestScintilla::cutSelection() {
    _scintilla->cutSelection();
    const QString text = _scintilla->getTextCopied();
    qDebug() << "Text copied=" << text;
}

void TestScintilla::zoom() {
    const int range = QInputDialog::getInt(_window, "Zoom", "zoom [-20, 20]:");
    _scintilla->setZoom(range);
}

void TestScintilla::displayEOL() {
    _scintilla->displayEOL(!_scintilla->isEOLDisplayed());
}

void TestScintilla::convertEOL() {
    const int n = QInputDialog::getInt(_window, "convert eol",
        "1=windows, 2=mac, 3=linux");
    switch (n) {
    case 1:
        _scintilla->convertEOL(WdScintilla::eEOLType::EOL_WINDOWS);
        break;
    case 2:
        _scintilla->convertEOL(WdScintilla::eEOLType::EOL_MAC);
        break;
    case 3:
        _scintilla->convertEOL(WdScintilla::eEOLType::EOL_LINUX);
        break;
    default:
        qWarning() << "No valid number for EOL conversion...";
    }
}

void TestScintilla::switchOverwriteMode() {
    bool st = _scintilla->isOverwriteMode();
    _scintilla->setOverwriteMode(!st);
    st = _scintilla->isOverwriteMode();
    if (st) {
        qDebug() << "Overwrite mode enabled";
    }
    else {
        qDebug() << "Insert mode enabled";
    }
}

void TestScintilla::gotoLine() {
    const int line = QInputDialog::getInt(_window, "Goto line", "go to the line:");
    _scintilla->gotoLine(line);
}

void TestScintilla::gotoPos() {
    const int pos = QInputDialog::getInt(_window, "Goto position", "go to the position:");
    _scintilla->gotoPos(pos);
}

void TestScintilla::lowerCaseSelection() {
    _scintilla->lowerCaseSelection();
}

void TestScintilla::upperCaseSelection() {
    _scintilla->upperCaseSelection();
}

void TestScintilla::markersTest() {
    _scintilla->defineMarker(WdScintilla::WdMarkerSymbol::Circle, 0);
    _scintilla->defineMarker(WdScintilla::WdMarkerSymbol::CircledMinus, 1);
    _scintilla->defineMarker(WdScintilla::WdMarkerSymbol::Rectangle, 2);
    _scintilla->defineMarker(WdScintilla::WdMarkerSymbol::BoxedPlus, 3);

    _scintilla->setMarginSymbol(2);
    _scintilla->setMarginSymbol(3);
    _scintilla->setMarginWidth(2, "000");
    _scintilla->setMarginWidth(3, "000");

    _scintilla->setMarginMarkerMask(1, QList<int>() << 0 << 1);
    _scintilla->setMarginMarkerMask(2, QList<int>() << 2);
    _scintilla->setMarginMarkerMask(3, QList<int>() << 3);

    // Here, only the circleminus should be displayed
    _scintilla->addMarker(1, 0);
    _scintilla->addMarker(1, 1);

    _scintilla->addMarker(2, 1);
    _scintilla->addMarker(8, 0);
    _scintilla->addSetMarker(3, QList<int>() << 0 << 1);
    _scintilla->addMarker(5, 3);
    _scintilla->addMarker(6, 3);
    _scintilla->addMarker(1, 3);
    _scintilla->addMarker(2, 2);
    _scintilla->addMarker(10, 2);

    qDebug() << "We're going to get markers";
    const QList<int> totalCircleAll = _scintilla->getLinesWithMarker(0);
    const QList<int> totalRectTill5 = _scintilla->getLinesWithMarker(0, 5, 2);
    const QList<int> totalBoxedFrom4 = _scintilla->getLinesWithMarker(4, 3);

    qDebug() << "I should have: 1, 3";
    qDebug() << totalCircleAll;
    qDebug() << "I should have: 2";
    qDebug() << totalRectTill5;
    qDebug() << "I should have: 5, 6";
    qDebug() << totalBoxedFrom4;

    _scintilla->addMarker(11, 0);
    _scintilla->addMarker(11, 2);
    _scintilla->addSetMarker(12, QList<int>() << 0 << 1 << 2 << 3);
    const QList<int> totalLine11 = _scintilla->getMarkers(11);
    const QList<int> totalLine12 = _scintilla->getMarkers(12);
    _scintilla->deleteMarker(11, 2);
    // line 12 because 11 + 1 displayed
    qDebug() << "Originally, line12=" << totalLine11;
    qDebug() << "Originally, line13=" << totalLine12;
    qDebug() << "We have deleted some marker, now line number 12 should have only a circle";
    _scintilla->deleteAllMarkersOnLine(12);
    qDebug() << "All markers should have been deleted on the line 13.";
}

void TestScintilla::indicatorTest() {
    _scintilla->defineIndicator(WdScintilla::WdIndicator::SquiggleIndicator, 0);
    _scintilla->defineIndicator(WdScintilla::WdIndicator::RoundBoxIndicator, 1);
    _scintilla->defineIndicator(WdScintilla::WdIndicator::BoxIndicator, 2);
    _scintilla->defineIndicator(WdScintilla::WdIndicator::ThickCompositionIndicator, 3);

    _scintilla->setIndicatorColor(QColor("red"), 0);
    if (_scintilla->getLineLength(1) > 11) {
        _scintilla->setIndicator(1, 5, 1, 10, 0);
    }
    if (_scintilla->getLineLength(4) > 20) {
        _scintilla->setIndicator(4, 0, 4, 20, 0);
    }

    _scintilla->setIndicatorColor(QColor("blue"), 1);
    if (_scintilla->getLineLength(0) > 15) {
        _scintilla->setIndicator(0, 3, 0, 15, 1);
    }
    if (_scintilla->getLineLength(0) > 15) {
        _scintilla->clearIndicator(0, 8, 0, 15);
    }

    _scintilla->setIndicatorColor(QColor("green"), 2);
    if (_scintilla->getLineLength(3) > 20) {
        _scintilla->setIndicator(3, 12, 3, 20, 2);
    }

    _scintilla->setIndicatorColor(QColor("pink"), 3);
    if (_scintilla->getLineLength(5) > 10) {
        _scintilla->setIndicator(5, 3, 5, 15, 3);
    }
}

void TestScintilla::colorTest() {
    _scintilla->setForegroundColor(1, QColor("purple"));
    _scintilla->setBackgroundColor(1, _scintilla->getDocumentBackground());
    _scintilla->setForegroundColor(2, QColor("green"));
    _scintilla->setBackgroundColor(2, _scintilla->getDocumentBackground());
    _scintilla->applyStyle(4, 45, 1);
    _scintilla->applyStyle(7, 9, 45, 1);
    _scintilla->applyStyle(6, 4, 10, 2);
}

void TestScintilla::foldTest() {
    _scintilla->setFoldPart(2, 10);
    _scintilla->setFoldPart(4, 8);
    _scintilla->setFoldPart(11, 15);
    _scintilla->setFoldPart(20, 45);
    _scintilla->setFoldPart(30, 35);
    _scintilla->setFoldPart(31, 32);
    //    removeFoldPart(20, 45);
    _scintilla->removeFoldPart(21);
}
