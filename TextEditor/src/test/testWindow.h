
#ifndef __WINDOW_TEST_H__
#define __WINDOW_TEST_H__

#include <qlist.h>
#include <qtextdocument.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qmainwindow.h>
#include <QHBoxLayout>
#include <qdebug.h>
#include <qfiledialog.h>
#include <qvector.h>
#include <QPlainTextDocumentLayout>
#include <QVBoxLayout>

#include <qshortcut.h>
#include "../TextEditor/WdEditor.h"

//#define ENABLE_DOUBLE_TEST

class WindowTest : public QMainWindow
{
    Q_OBJECT
public:

    WindowTest() {
        QWidget *cont = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout();
        cont->setLayout(layout);

        layout->setContentsMargins(0, 0, 0, 0);
        cont->setContentsMargins(0, 0, 0, 0);

        editor = new WdEditor(cont);
        layout->addWidget(editor);
#if defined ENABLE_DOUBLE_TEST
        editor2 = new WdEditor(cont);
        layout->addWidget(editor2);
#endif

        this->setCentralWidget(cont);

        QVBoxLayout *buttons = new QVBoxLayout();

        add = new QPushButton("add a new doc");
        next = new QPushButton("go to next doc");
        col = new QPushButton("add color red");
        buttons->addWidget(add);
        buttons->addWidget(next);
        buttons->addWidget(col);
        layout->addLayout(buttons);

        idx = 0;
        connect(add, SIGNAL(clicked()), this, SLOT(addClicked()));
        connect(next, SIGNAL(clicked()), this, SLOT(nextClicked()));
        connect(col, SIGNAL(clicked()), this, SLOT(colClicked()));

        connect(editor, &WdEditor::cursorPositionChanged, this, 
            [this] { 
                qDebug() << "Cursor changed";
                editor->hideCallTipAutoCompletion();
        });
        connect(editor, &WdEditor::selectionChanged, this, 
            [this] {
                qDebug() << "new seelction:" << editor->getSelectedText();
        });
        connect(editor, &WdEditor::textAdded, this, [] (const int pos, QString const &txt) {
            qDebug() << "text added:" << txt;
        });

        

        QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+Space"), this, SLOT(showAutoComplete()));

    }

    WdEditor *editor;
#if defined ENABLE_DOUBLE_TEST
    WdEditor *editor2;
#endif
    QPushButton *add;
    QPushButton *next;
    QPushButton *col;

    public slots:
        void showAutoComplete() {
            editor->showCallTipAutoCompletion();
        }

        void addClicked() {
            qDebug() << "we add a doc";
            QString file = QFileDialog::getOpenFileName();
            if (file.isEmpty() == false) {
                _documents.push_back(editor->openFile(file));
            }
        }

        void nextClicked() {
            ++idx;
            qDebug() << "Go to next idx=" << idx;
            if (idx >= _documents.size()) {
                qDebug() << "reset index";
                idx = 0;
            }
            editor->setDocument(_documents[idx]);
#if defined ENABLE_DOUBLE_TEST
            editor2->setDocument(_documents[idx]);
#endif
            qDebug() << "text 4, 9=" << editor->getText(3343, 10);
            qDebug() << "pos=" << editor->positionFromLine(1, 5);
        }

        void colClicked() {
            const int pos = editor->getCurrentPosition();
            qDebug() << "pos=" << pos;
            editor->setColor(pos, 8, QColor("red"));
        }

public:
    QList<QTextDocument *> _documents;
    int idx;
};

#endif
