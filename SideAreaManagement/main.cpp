
#include <QtWidgets/QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>

#include "Area/Area.h"
#include "Area/SideAreaTabs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow w;
    QWidget *wi = new QWidget();
    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    w.setCentralWidget(wi);
    wi->setLayout(layout);

    QWidget *areaWidget = new QTextEdit();
    Area *area = new Area(areaWidget);
    layout->addWidget(area);


    SideArea *leftArea = area->getSideArea(Area::LEFT_SIDE);
    for (int i = 0; i < 10; ++i) {
        SideAreaComponent *component = new SideAreaComponent(QString("FIRST_%1").arg(i),
            QString("coucou_%1").arg(i));
        leftArea->addComponent(component);
    }

    for (int i = 0; i < 5; ++i) {
        SideAreaComponent *component = new SideAreaComponent(QString("INSERT_%1").arg(i),
            QString("insert_%1").arg(i));

        auto before = leftArea->getComponent(i * 2);
        leftArea->insertComponent(before, component);
    }

    SideAreaComponent *component = new SideAreaComponent(QString("INSERT_FIRST"),
        QString("first_insert"));
    leftArea->insertComponent(nullptr, component);
    w.show();

    return a.exec();
}
