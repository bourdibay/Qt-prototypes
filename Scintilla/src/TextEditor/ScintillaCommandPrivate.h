//
//   ScintillaCommandPrivate.h
//   Widev
//
//   Created by bourdi_b on 09 mars 2014 23:17:23
//
//
//

#ifndef __SCINTILLA_COMMAND_PRIVATE_H__
#define __SCINTILLA_COMMAND_PRIVATE_H__

#include <Qsci/qsciscintilla.h>

class ScintillaCommandPrivate {
public:
    ScintillaCommandPrivate(QsciScintilla *sci);

private:
    QsciScintilla *_sci;
};

#endif
