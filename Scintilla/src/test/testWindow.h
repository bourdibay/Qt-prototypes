
#ifndef __WINDOW_TEST_H__
#define __WINDOW_TEST_H__

#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QShortcut>

#include "../TextEditor/WdScintilla.h"
#include "TestScintilla.h"

class WindowTest : public QMainWindow
{
    Q_OBJECT
public:
    WindowTest() 
        : QMainWindow() 
    {
        QWidget *cont = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout();
        cont->setLayout(layout);

        layout->setContentsMargins(0, 0, 0, 0);
        cont->setContentsMargins(0, 0, 0, 0);

        editor = new WdScintilla(cont);
        layout->addWidget(editor);

        this->setCentralWidget(cont);

        QVBoxLayout *buttons = new QVBoxLayout();

        add = new QPushButton("Open a file");
        buttons->addWidget(add);
        layout->addLayout(buttons);

        connect(add, SIGNAL(clicked()), this, SLOT(addClicked()));

        QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+Space"), this, SLOT(showAutoComplete()));

        // enable tests in the menu bar
        TestScintilla *scintilla = new TestScintilla(this, editor);
    }

    WdScintilla *editor;
    QPushButton *add;

    public slots:
        void showAutoComplete() {
            editor->showCalltipCurrentPosition();
        }

        void addClicked() {
            QString file = QFileDialog::getOpenFileName();
            if (file.isEmpty() == false) {
                editor->openFile(file);
            }
        }
};

#endif
