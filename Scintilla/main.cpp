
// qt classes
#include <QApplication>
#include <QDebug>

#include "test/testWindow.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    WindowTest mainWindow;
    mainWindow.show();

    const int exitValue = app.exec();

    return exitValue;
}
