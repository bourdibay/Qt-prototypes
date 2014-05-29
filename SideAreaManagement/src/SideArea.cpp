
#include "Area/SideArea.h"

SideArea::SideArea(QWidget *parent)
    : QWidget(parent),
    _stackComponents(new QStackedWidget())
{
}

bool SideArea::addComponent(SideAreaComponent *component)
{
    const int index = _stackComponents->indexOf(component);
    if (index < 0) {
        _stackComponents->addWidget(component);
        _components.push_back(component);
        return true;
    }
    return false;
}

void SideArea::insertComponent(SideAreaComponent *before,
                               SideAreaComponent *component)
{
    int indexBefore = _components.indexOf(before);
    if (indexBefore < 0) {
        indexBefore = 0;
    }
    const int index = _components.indexOf(component);
    if (index < 0) {
        _stackComponents->insertWidget(indexBefore, component);
        _components.insert(indexBefore, component);
    } else {
        _components.takeAt(index);
        _components.insert(indexBefore, component);
    }
}

bool SideArea::takeComponent(SideAreaComponent *component)
{
    const int idx = _components.indexOf(component);
    if (idx >= 0) {
        SideAreaComponent *cmp = _components.takeAt(idx);
        _stackComponents->removeWidget(component);
        return true;
    }
    return false;
}

void SideArea::setCurrentComponent(SideAreaComponent *component)
{
    const int index = _components.indexOf(component);
    if (index >= 0) {
        _stackComponents->setCurrentWidget(component);
    }
}

SideAreaComponent *SideArea::getComponent(QString const &key) const
{
    const auto &end = std::end(_components);
    const auto &it = std::find_if(std::begin(_components), end, 
        [&key] (SideAreaComponent *cmp) {
            return cmp->getKey() == key;
    });
    return it != end ? *it : nullptr;
}
