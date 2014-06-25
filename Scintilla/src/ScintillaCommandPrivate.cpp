//
//   ScintillaCommandPrivate.cpp
//   Widev
//
//   Created by bourdi_b on 09 mars 2014 23:18:16
//
//
//

#include "TextEditor/ScintillaCommandPrivate.h"

// TODO: explanation of this class (override shortcuts)

// Starting with QScintilla v2.7 the standard OS/X keyboard shortcuts are used
// where possible.  In order to restore the behaviour of earlier versions then
// #define DONT_USE_OSX_KEYS here or add it to the qmake project (.pro) file.
#if defined(Q_OS_MAC) && !defined(DONT_USE_OSX_KEYS)
#define USING_OSX_KEYS
#else
#undef USING_OSX_KEYS
#endif

// Convert a Qt character to the Scintilla equivalent.  Return zero if it is
// invalid.
static int convert(int key) {
    // Convert the modifiers.
    int sci_mod = 0;

    if (key & Qt::SHIFT) {
        sci_mod |= QsciScintillaBase::SCMOD_SHIFT;
    }
    if (key & Qt::CTRL) {
        sci_mod |= QsciScintillaBase::SCMOD_CTRL;
    }
    if (key & Qt::ALT) {
        sci_mod |= QsciScintillaBase::SCMOD_ALT;
    }
    if (key & Qt::META) {
        sci_mod |= QsciScintillaBase::SCMOD_META;
    }

    key &= ~Qt::MODIFIER_MASK;

    // Convert the key.
    int sci_key;

    if (key > 0x7f)
        switch (key) {
            case Qt::Key_Down:
                sci_key = QsciScintillaBase::SCK_DOWN;
                break;
            case Qt::Key_Up:
                sci_key = QsciScintillaBase::SCK_UP;
                break;
            case Qt::Key_Left:
                sci_key = QsciScintillaBase::SCK_LEFT;
                break;
            case Qt::Key_Right:
                sci_key = QsciScintillaBase::SCK_RIGHT;
                break;
            case Qt::Key_Home:
                sci_key = QsciScintillaBase::SCK_HOME;
                break;
            case Qt::Key_End:
                sci_key = QsciScintillaBase::SCK_END;
                break;
            case Qt::Key_PageUp:
                sci_key = QsciScintillaBase::SCK_PRIOR;
                break;
            case Qt::Key_PageDown:
                sci_key = QsciScintillaBase::SCK_NEXT;
                break;
            case Qt::Key_Delete:
                sci_key = QsciScintillaBase::SCK_DELETE;
                break;
            case Qt::Key_Insert:
                sci_key = QsciScintillaBase::SCK_INSERT;
                break;
            case Qt::Key_Escape:
                sci_key = QsciScintillaBase::SCK_ESCAPE;
                break;
            case Qt::Key_Backspace:
                sci_key = QsciScintillaBase::SCK_BACK;
                break;
            case Qt::Key_Tab:
                sci_key = QsciScintillaBase::SCK_TAB;
                break;
            case Qt::Key_Return:
                sci_key = QsciScintillaBase::SCK_RETURN;
                break;
            default:
                sci_key = 0;
        }
    else {
        sci_key = key;
    }
    if (sci_key) {
        sci_key |= (sci_mod << 16);
    }
    return sci_key;
}

ScintillaCommandPrivate::ScintillaCommandPrivate(QsciScintilla *sci)
: _sci(sci) {
    // Starting with Qscintilla v2.8

    struct sci_cmd {
        QsciCommand::Command cmd;
        int key;
        int alkey;
        const char *desc;
    };

    static struct sci_cmd cmd_table[] = {
        {QsciCommand::LineDown, Qt::Key_Down,
#if defined(USING_OSX_KEYS)
         Qt::Key_N | Qt::META,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Move down one line")},
        {QsciCommand::LineDownExtend, Qt::Key_Down | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
         Qt::Key_N | Qt::META | Qt::SHIFT,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Extend selection down one line")},
        {QsciCommand::LineScrollDown, Qt::Key_Down | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Scroll view down one line")},
        {QsciCommand::LineUp, Qt::Key_Up,
#if defined(USING_OSX_KEYS)
         Qt::Key_P | Qt::META,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Move up one line")},
        {QsciCommand::LineUpExtend, Qt::Key_Up | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
         Qt::Key_P | Qt::META | Qt::SHIFT,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Extend selection up one line")},
        {QsciCommand::LineScrollUp, Qt::Key_Up | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Scroll view up one line")},
        {QsciCommand::ScrollToStart,
#if defined(USING_OSX_KEYS)
         Qt::Key_Home,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Scroll to start of document")},
        {QsciCommand::ScrollToEnd,
#if defined(USING_OSX_KEYS)
         Qt::Key_End,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Scroll to end of document")},
        {QsciCommand::VerticalCentreCaret,
#if defined(USING_OSX_KEYS)
         Qt::Key_L | Qt::META,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Scroll vertically to centre current line")},
        {QsciCommand::ParaDown, Qt::Key_BracketRight | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Move down one paragraph")},
        {QsciCommand::ParaDownExtend,
         Qt::Key_BracketRight | Qt::CTRL | Qt::SHIFT, 0,
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Extend selection down one paragraph")},
        {QsciCommand::ParaUp, Qt::Key_BracketLeft | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Move up one paragraph")},
        {QsciCommand::ParaUpExtend, Qt::Key_BracketLeft | Qt::CTRL | Qt::SHIFT,
         0,
         QT_TRANSLATE_NOOP("QsciCommand", "Extend selection up one paragraph")},
        {QsciCommand::CharLeft, Qt::Key_Left,
#if defined(USING_OSX_KEYS)
         Qt::Key_B | Qt::META,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Move left one character")},
        {QsciCommand::CharLeftExtend, Qt::Key_Left | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
         Qt::Key_B | Qt::META | Qt::SHIFT,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Extend selection left one character")},
        {QsciCommand::CharLeftRectExtend, Qt::Key_Left | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
         Qt::Key_B | Qt::META | Qt::ALT | Qt::SHIFT,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Extend rectangular selection left one character")},
        {QsciCommand::CharRight, Qt::Key_Right,
#if defined(USING_OSX_KEYS)
         Qt::Key_F | Qt::META,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Move right one character")},
        {QsciCommand::CharRightExtend, Qt::Key_Right | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
         Qt::Key_F | Qt::META | Qt::SHIFT,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Extend selection right one character")},
        {QsciCommand::CharRightRectExtend, Qt::Key_Right | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
         Qt::Key_F | Qt::META | Qt::ALT | Qt::SHIFT,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Extend rectangular selection right one character")},
        {QsciCommand::WordLeft,
#if defined(USING_OSX_KEYS)
         Qt::Key_Left | Qt::ALT,
#else
         Qt::Key_Left | Qt::CTRL,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move left one word")},
        {QsciCommand::WordLeftExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_Left | Qt::ALT | Qt::SHIFT,
#else
         Qt::Key_Left | Qt::CTRL | Qt::SHIFT,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Extend selection left one word")},
        {QsciCommand::WordRight,
#if defined(USING_OSX_KEYS)
         0,
#else
         Qt::Key_Right | Qt::CTRL,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move right one word")},
        {QsciCommand::WordRightExtend, Qt::Key_Right | Qt::CTRL | Qt::SHIFT, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Extend selection right one word")},
        {QsciCommand::WordLeftEnd, 0, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Move to end of previous word")},
        {QsciCommand::WordLeftEndExtend, 0, 0,
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Extend selection to end of previous word")},
        {QsciCommand::WordRightEnd,
#if defined(USING_OSX_KEYS)
         Qt::Key_Right | Qt::ALT,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move to end of next word")},
        {QsciCommand::WordRightEndExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_Right | Qt::ALT | Qt::SHIFT,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Extend selection to end of next word")},
        {QsciCommand::WordPartLeft, Qt::Key_Slash | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Move left one word part")},
        {QsciCommand::WordPartLeftExtend, Qt::Key_Slash | Qt::CTRL | Qt::SHIFT,
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Extend selection left one word part")},
        {QsciCommand::WordPartRight, Qt::Key_Backslash | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Move right one word part")},
        {QsciCommand::WordPartRightExtend,
         Qt::Key_Backslash | Qt::CTRL | Qt::SHIFT, 0,
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Extend selection right one word part")},
        {QsciCommand::Home,
#if defined(USING_OSX_KEYS)
         Qt::Key_A | Qt::META,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move to start of document line")},
        {QsciCommand::HomeExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_A | Qt::META | Qt::SHIFT,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Extend selection to start of document line")},
        {QsciCommand::HomeDisplay,
#if defined(USING_OSX_KEYS)
         Qt::Key_Left | Qt::CTRL,
#else
         Qt::Key_Home | Qt::ALT,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move to start of display line")},
        {QsciCommand::HomeDisplayExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_Left | Qt::CTRL | Qt::SHIFT,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Extend selection to start of display line")},
        {QsciCommand::HomeWrap, 0, 0,
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Move to start of display or document line")},
        {QsciCommand::HomeWrapExtend, 0, 0,
         QT_TRANSLATE_NOOP(
             "QsciCommand",
             "Extend selection to start of display or document line")},
        {QsciCommand::VCHome,
#if defined(USING_OSX_KEYS)
         0,
#else
         Qt::Key_Home,
#endif
         0,
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Move to first visible character in document line")},
        {QsciCommand::VCHomeExtend,
#if defined(USING_OSX_KEYS)
         0,
#else
         Qt::Key_Home | Qt::SHIFT,
#endif
         0,
         QT_TRANSLATE_NOOP(
             "QsciCommand",
             "Extend selection to first visible character in document line")},
        {QsciCommand::VCHomeRectExtend,
#if defined(USING_OSX_KEYS)
         0,
#else
         Qt::Key_Home | Qt::ALT | Qt::SHIFT,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Extend rectangular selection to "
                                             "first visible character in "
                                             "document line")},
        {QsciCommand::VCHomeWrap, 0, 0,
         QT_TRANSLATE_NOOP(
             "QsciCommand",
             "Move to first visible character of display in document line")},
        {QsciCommand::VCHomeWrapExtend, 0, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Extend selection to first visible "
                                          "character in display or document "
                                          "line")},
        {QsciCommand::LineEnd,
#if defined(USING_OSX_KEYS)
         Qt::Key_E | Qt::META,
#else
         Qt::Key_End,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move to end of document line")},
        {QsciCommand::LineEndExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_E | Qt::META | Qt::SHIFT,
#else
         Qt::Key_End | Qt::SHIFT,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Extend selection to end of document line")},
        {QsciCommand::LineEndRectExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_E | Qt::META | Qt::ALT | Qt::SHIFT,
#else
         Qt::Key_End | Qt::ALT | Qt::SHIFT,
#endif
         0, QT_TRANSLATE_NOOP(
                "QsciCommand",
                "Extend rectangular selection to end of document line")},
        {QsciCommand::LineEndDisplay,
#if defined(USING_OSX_KEYS)
         Qt::Key_Right | Qt::CTRL,
#else
         Qt::Key_End | Qt::ALT,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move to end of display line")},
        {QsciCommand::LineEndDisplayExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_Right | Qt::CTRL | Qt::SHIFT,
#else
         0,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Extend selection to end of display line")},
        {QsciCommand::LineEndWrap, 0, 0,
         QT_TRANSLATE_NOOP("QsciCommand",
                           "Move to end of display or document line")},
        {QsciCommand::LineEndWrapExtend, 0, 0,
         QT_TRANSLATE_NOOP(
             "QsciCommand",
             "Extend selection to end of display or document line")},
        {QsciCommand::DocumentStart,
#if defined(USING_OSX_KEYS)
         Qt::Key_Up | Qt::CTRL,
#else
         Qt::Key_Home | Qt::CTRL,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move to start of document")},
        {QsciCommand::DocumentStartExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_Up | Qt::CTRL | Qt::SHIFT,
#else
         Qt::Key_Home | Qt::CTRL | Qt::SHIFT,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Extend selection to start of document")},
        {QsciCommand::DocumentEnd,
#if defined(USING_OSX_KEYS)
         Qt::Key_Down | Qt::CTRL,
#else
         Qt::Key_End | Qt::CTRL,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Move to end of document")},
        {QsciCommand::DocumentEndExtend,
#if defined(USING_OSX_KEYS)
         Qt::Key_Down | Qt::CTRL | Qt::SHIFT,
#else
         Qt::Key_End | Qt::CTRL | Qt::SHIFT,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand",
                              "Extend selection to end of document")},
        {QsciCommand::PageUp, Qt::Key_PageUp, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Move up one page")},
        {QsciCommand::PageUpExtend, Qt::Key_PageUp | Qt::SHIFT, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Extend selection up one page")},
        {QsciCommand::PageDown, Qt::Key_PageDown,
#if defined(USING_OSX_KEYS)
         Qt::Key_V | Qt::META,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Move down one page")},
        {QsciCommand::PageDownExtend, Qt::Key_PageDown | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
         Qt::Key_V | Qt::META | Qt::SHIFT,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Extend selection down one page")},
        {QsciCommand::Delete, Qt::Key_Delete,
#if defined(USING_OSX_KEYS)
         Qt::Key_D | Qt::META,
#else
         0,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Delete current character")},
        {QsciCommand::DeleteBack, Qt::Key_Backspace,
#if defined(USING_OSX_KEYS)
         Qt::Key_H | Qt::META,
#else
         Qt::Key_Backspace | Qt::SHIFT,
#endif
         QT_TRANSLATE_NOOP("QsciCommand", "Delete previous character")},
        {QsciCommand::DeleteBackNotLine, 0, 0,
         QT_TRANSLATE_NOOP(
             "QsciCommand",
             "Delete previous character if not at start of line")},
        {QsciCommand::DeleteWordLeft, Qt::Key_Backspace | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Delete word to left")},
        {QsciCommand::DeleteWordRight, Qt::Key_Delete | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Delete word to right")},
        {QsciCommand::DeleteWordRightEnd,
#if defined(USING_OSX_KEYS)
         Qt::Key_Delete | Qt::ALT,
#else
         0,
#endif
         0,
         QT_TRANSLATE_NOOP("QsciCommand", "Delete right to end of next word")},
        {QsciCommand::DeleteLineLeft, Qt::Key_Backspace | Qt::CTRL | Qt::SHIFT,
         0, QT_TRANSLATE_NOOP("QsciCommand", "Delete line to left")},
        {QsciCommand::DeleteLineRight,
#if defined(USING_OSX_KEYS)
         Qt::Key_K | Qt::META,
#else
         Qt::Key_Delete | Qt::CTRL | Qt::SHIFT,
#endif
         0, QT_TRANSLATE_NOOP("QsciCommand", "Delete line to right")},
        {QsciCommand::EditToggleOvertype, Qt::Key_Insert, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Toggle insert/overtype")},
        {QsciCommand::Newline, Qt::Key_Return, Qt::Key_Return | Qt::SHIFT,
         QT_TRANSLATE_NOOP("QsciCommand", "Insert newline")},
        {QsciCommand::Formfeed, 0, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Formfeed")},
        {QsciCommand::Tab, Qt::Key_Tab, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Indent one level")},
        {QsciCommand::Backtab, Qt::Key_Tab | Qt::SHIFT, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "De-indent one level")},
        {QsciCommand::Cancel, Qt::Key_Escape, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Cancel")},
        {QsciCommand::ZoomIn, Qt::Key_Plus | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Zoom in")},
        {QsciCommand::ZoomOut, Qt::Key_Minus | Qt::CTRL, 0,
         QT_TRANSLATE_NOOP("QsciCommand", "Zoom out")},
    };

    // Clear the default map.
    _sci->SendScintilla(QsciScintillaBase::SCI_CLEARALLCMDKEYS);

    // By default control characters don't do anything (rather than insert the
    // control character into the text).
    for (int k = 'A'; k <= 'Z'; ++k) {
        _sci->SendScintilla(QsciScintillaBase::SCI_ASSIGNCMDKEY,
                            k + (QsciScintillaBase::SCMOD_CTRL << 16),
                            QsciScintillaBase::SCI_NULL);
    }
    for (int i = 0; i < sizeof(cmd_table) / sizeof(cmd_table[0]); ++i) {
        const int scikey = convert(cmd_table[i].key);
        if (scikey) {
            _sci->SendScintilla(QsciScintillaBase::SCI_ASSIGNCMDKEY, scikey,
                                cmd_table[i].cmd);
        }
        const int scialtkey = convert(cmd_table[i].alkey);
        if (scialtkey) {
            _sci->SendScintilla(QsciScintillaBase::SCI_ASSIGNCMDKEY, scialtkey,
                                cmd_table[i].cmd);
        }
        // this, cmd_table[i].cmd, cmd_table[i].key,
        //                  cmd_table[i].altkey, cmd_table[i].desc);
    }
}
