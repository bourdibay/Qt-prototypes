
#include <QVBoxLayout>

#include "Area/SideAreaTabs.h"

SideAreaTabs::SideAreaTabs(QWidget *parent)
: SideArea(parent), _tabbar(new QTabBar()) {
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
    layout->addWidget(_tabbar);
    layout->addWidget(_stackComponents);

    _tabbar->setUsesScrollButtons(true);
    _tabbar->setElideMode(Qt::ElideRight);
}

bool SideAreaTabs::addComponent(SideAreaComponent *component) {
    const bool ret = SideArea::addComponent(component);
    if (ret) {
        _tabbar->addTab(component->getIcon(), component->getName());
    }
    return ret;
}

void SideAreaTabs::insertComponent(SideAreaComponent *before,
                                   SideAreaComponent *component) {
    int indexBefore = _components.indexOf(before);
    if (indexBefore < 0) {
        indexBefore = 0;
    }
    const int index = _components.indexOf(component);
    if (index < 0) {
        _stackComponents->addWidget(component);
    } else {
        _components.takeAt(index);
        _tabbar->removeTab(index);
    }
    _components.insert(indexBefore, component);
    _tabbar->insertTab(indexBefore, component->getIcon(), component->getName());
}

bool SideAreaTabs::takeComponent(SideAreaComponent *component) {
    const int index = _stackComponents->indexOf(component);
    if (index >= 0) {
        SideArea::takeComponent(component);
        _tabbar->removeTab(index);
        return true;
    }
    return false;
}

void SideAreaTabs::setCurrentComponent(SideAreaComponent *component) {
    SideArea::setCurrentComponent(component);
    const int index = _components.indexOf(getCurrentComponent());
    _tabbar->setCurrentIndex(index);
}
