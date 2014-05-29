
#include <QtWidgets/QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>

#include "Area/Area.h"
#include "ComponentTest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow w;
    QWidget *wi = new QWidget();
    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    w.setCentralWidget(wi);
    wi->setLayout(layout);

    QWidget *areaWidget = new QWidget();
    Area *area = new Area(areaWidget);
    layout->addWidget(area);

    SideArea *leftArea = area->getSideArea(Area::SidePosition::LEFT_SIDE);
    ComponentTest *testLeft = new ComponentTest("TEST_CMP", "a test comopnent");
    leftArea->addComponent(testLeft);
    SideArea *rightArea = area->getSideArea(Area::SidePosition::RIGHT_SIDE);
    ComponentTest *testRight = new ComponentTest(*testLeft);
    rightArea->addComponent(testRight);

    w.show();

    return a.exec();
}
